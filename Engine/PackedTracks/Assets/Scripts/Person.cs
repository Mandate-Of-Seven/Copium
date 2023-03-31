/*!***************************************************************************************
\file			Person.cs
\project
\author			Sean Ngo
\co-author		Zacharie Hong

\par			Course: GAM250
\par			Section:
\date			10/03/2023

\brief
	Contains the data struct to store data for a crewmate

All content � 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
using CopiumEngine;
using System;

public enum HEALTH_STATE
{
    DEAD,
    CRITICAL,
    INJURED,
    HEALTHY,
    NONE = -1
}
public enum MENTAL_STATE
{
    SUICIDAL,
    INSANE,
    IRRATIONAL,
    CALM,
    NONE = -1
}
public enum HUNGER_STATE
{
    FAMISHED,
    HUNGRY,
    FULL,
    NONE = -1
}
public class Person
{
	public static int scramblingConcurrencies = 3;
	public static int scramblingIterations = 4;
	public static float scramblingIntervals = 0.05f;

	static StringScramblerEffect CreateScrambler(string before, string after)
	{
		return new StringScramblerEffect
		(
			before, after, 
			scramblingConcurrencies, 
			scramblingIterations, 
			scramblingIntervals
		);
	}

    public bool alive { get { return health != HEALTH_STATE.DEAD; } }

    string _name;
    HEALTH_STATE _health;
    MENTAL_STATE _mental;
    HUNGER_STATE _hunger;
    float _timer;
    string _resultText;
    public StringScramblerEffect healthScrambler;
    public StringScramblerEffect mentalScrambler;
    public StringScramblerEffect hungerScrambler;

	public Color targetColor = Color.white;

    /*******************************************************************************
    /*!
        \brief
            Gets and sets name
    */
    /*******************************************************************************/
    public string name
    {
        get {  return _name; }
        set {  _name = value; }
    }

    /*******************************************************************************
    /*!
        \brief
            Gets and sets health
    */
    /*******************************************************************************/
    public HEALTH_STATE health
    {
        get { return _health; }
        set 
        {
            if (value < 0)
                value = 0;
            if (_health != value)
            {
				Color tmp = Color.white;
				float x = ((float)value/3.0f);
				tmp.r = x;
				tmp.g = x;
				tmp.b = x;
				targetColor = tmp;

                if (healthScrambler != null)
                    healthScrambler = CreateScrambler(_health.ToString(), value.ToString());
                else
                    healthScrambler = CreateScrambler("", value.ToString());
                _health = value;
            }
        }
    }

    /*******************************************************************************
    /*!
        \brief
            Gets and sets mental state
    */
    /*******************************************************************************/
    public MENTAL_STATE mental
    {
        get { return _mental; }
        set 
        { 
            if (health == HEALTH_STATE.DEAD)
            {

                if (_mental != MENTAL_STATE.NONE)
                {
                    mentalScrambler = CreateScrambler(_mental.ToString(), "");
                    _mental = MENTAL_STATE.NONE;
                }
                return;
            }

            if (_mental != value)
            {
                if (mentalScrambler != null)
                    mentalScrambler = CreateScrambler(_mental.ToString(), value.ToString());
                else
                    mentalScrambler = CreateScrambler("", value.ToString());
                _mental = value;
            }

        }
    }

    /*******************************************************************************
    /*!
        \brief
            Gets and sets hunger state
    */
    /*******************************************************************************/
    public HUNGER_STATE hunger
    {
        get { return _hunger; }
        set 
		{
            if (health == HEALTH_STATE.DEAD)
            {
                if (_hunger != HUNGER_STATE.NONE)
                {
                    hungerScrambler = CreateScrambler(_hunger.ToString(), "");
                    _hunger = HUNGER_STATE.NONE;
                }
                return;
            }
			
            if (_hunger != value)
            {

                if(value > HUNGER_STATE.FULL)
                    value = HUNGER_STATE.FULL;

                if(hungerScrambler != null)
                    hungerScrambler = CreateScrambler(_hunger.ToString(), value.ToString());
                else
                    hungerScrambler = CreateScrambler("", value.ToString());
                _hunger = value;
            }
		}
    }

    /*******************************************************************************
    /*!
        \brief
            Gets and sets timer
    */
    /*******************************************************************************/
    public float timer
    {
        get { return _timer; }
        set { _timer = value; }
    }

    /*******************************************************************************
    /*!
        \brief
            Gets and sets result text
    */
    /*******************************************************************************/
    public string resultText
    {
        get { return _resultText; }
        set { _resultText = value; }
    }

    public Person(string new_name)
    {
        _name = new_name;
        health = HEALTH_STATE.HEALTHY;
        mental = MENTAL_STATE.CALM;
        hunger = HUNGER_STATE.FULL;
        _timer = 0.0f;
        _resultText = _name + " is just chilling in the back";
    }
}
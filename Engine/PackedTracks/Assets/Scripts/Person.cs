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

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
using CopiumEngine;
using System;

    
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

    string _name;
    bool _alive = true;
    int _health;
    int _mental;
    int _hunger;
    float _timer;
    string _resultText;
    public StringScramblerEffect healthScrambler;
    public StringScramblerEffect mentalScrambler;
    public StringScramblerEffect hungerScrambler;
    public string healthString; 
    public string mentalString;
    public string hungerString; 
    Crew.HEALTH_STATE healthState = Crew.HEALTH_STATE.NONE;
    Crew.MENTAL_STATE mentalState = Crew.MENTAL_STATE.NONE;
    Crew.HUNGER_STATE hungerState = Crew.HUNGER_STATE.NONE;
	public Crew crewScript;

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
            Gets and sets alive state
    */
    /*******************************************************************************/
    public bool alive
    {
        get { return _alive; }
        set { _alive = value; }
    }

    /*******************************************************************************
    /*!
        \brief
            Gets and sets health
    */
    /*******************************************************************************/
    public int health
    {
        get { return _health; }
        set 
        {
            if (_health != value)
            {
				Color tmp = Color.white;
				float x = ((float)value/3.0f);
				tmp.r = x;
				tmp.g = x;
				tmp.b = x;
				targetColor = tmp;
				if (value > 2)
                {
                    if (healthState != Crew.HEALTH_STATE.HEALTHY)
						healthScrambler = CreateScrambler(healthString, "Healthy");
                    healthState = Crew.HEALTH_STATE.HEALTHY;
                }
				else if(value > 1)
                {
                    if (healthState != Crew.HEALTH_STATE.INJURED)
						healthScrambler = CreateScrambler(healthString, "Injured");
                    healthState = Crew.HEALTH_STATE.INJURED;
                }
				else if (value > 0)
                {
                    if (healthState != Crew.HEALTH_STATE.CRITICAL)
						healthScrambler = CreateScrambler(healthString, "Critical");
                    healthState = Crew.HEALTH_STATE.CRITICAL;
                }
				else
                {    
                    if (healthState != Crew.HEALTH_STATE.DEAD)
                        healthScrambler = CreateScrambler(healthString, "Dead");
                    resultText = name + " is dead...";
                    crewScript.selectBtnWrapper.failureText = name + " is dead...";
                    _alive = false; 
                    healthState = Crew.HEALTH_STATE.DEAD;
                }
					
            }
                _health = value; 
        }
    }

    /*******************************************************************************
    /*!
        \brief
            Gets and sets mental state
    */
    /*******************************************************************************/
    public int mental
    {
        get { return _mental; }
        set 
        { 
            if (!_alive)
            {
                mentalScrambler = CreateScrambler(mentalString, "Dead");   
            }
            else if (_mental != value)
            {
                if (value > 2)
                {
                    if (mentalState != Crew.MENTAL_STATE.CALM)
                        mentalScrambler = CreateScrambler(mentalString, "Calm");
                    mentalState = Crew.MENTAL_STATE.CALM;
                }
                else if (value > 1)
                {

                    if (mentalState != Crew.MENTAL_STATE.IRRATIONAL)
                        mentalScrambler = CreateScrambler(mentalString, "Irrational");
                    mentalState = Crew.MENTAL_STATE.IRRATIONAL;
                }
                else if (value > 0)
                {
                    if (mentalState != Crew.MENTAL_STATE.INSANE)
                        mentalScrambler = CreateScrambler(mentalString, "Insane");
                    mentalState = Crew.MENTAL_STATE.INSANE;
                }
                else
                {
                    if (mentalState != Crew.MENTAL_STATE.SUICIDAL)
                        mentalScrambler = CreateScrambler(mentalString, "Suicidal");
                    mentalState = Crew.MENTAL_STATE.SUICIDAL;
                }
            }
            _mental = value; 
                
        }
    }

    /*******************************************************************************
    /*!
        \brief
            Gets and sets hunger state
    */
    /*******************************************************************************/
    public int hunger
    {
        get { return _hunger; }
        set 
		{
            if (!_alive)
            {
                hungerScrambler = CreateScrambler(hungerString, "Dead");
            }
			else if (_hunger != value)
            {
				if (value > 2)
                {
                    if (hungerState != Crew.HUNGER_STATE.FULL)
						hungerScrambler = CreateScrambler(hungerString, "Full");
                    hungerState = Crew.HUNGER_STATE.FULL;
                }
				else if (value > 1)
                {
                    if (hungerState != Crew.HUNGER_STATE.HUNGRY)
						hungerScrambler = CreateScrambler(hungerString, "Hungry");
                    hungerState = Crew.HUNGER_STATE.HUNGRY;
                }
				else
                {
                    if (hungerState != Crew.HUNGER_STATE.FAMISHED)
						hungerScrambler = CreateScrambler(hungerString, "Famished");
                    hungerState = Crew.HUNGER_STATE.FAMISHED;
                }
			}
				_hunger = value; 
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
        _alive = true;
        health = 3;
        mental = 3;
        hunger = 3;
        _timer = 0.0f;
        _resultText = _name + " is just chilling in the back";
    }
}
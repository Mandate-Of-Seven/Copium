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
        int healthState = -1;
        int mentalState = -1;
        int hungerState = -1;
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
					float x = ((float)value/15.0f);
					tmp.r = x;
					tmp.g = x;
					tmp.b = x;
					targetColor = tmp;
					if (value > 10)
                    {
                        if (healthState != 0)
						    healthScrambler = CreateScrambler(healthString, "Healthy");
                        healthState = 0;
                    }
					else if(value > 5)
                    {
                        if (healthState != 1)
						    healthScrambler = CreateScrambler(healthString, "Injured");
                        healthState = 1;
                    }
					else if (value > 0)
                    {
                        if (healthState != 2)
						    healthScrambler = CreateScrambler(healthString, "Critical");
                        healthState = 2;
                    }
					else
                    {    
                        if (healthState != 3)
                        healthScrambler = CreateScrambler(healthString, "Dead");
                        _alive = false; 
                        healthState = 3;
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
                    if (value > 10)
                    {
                        if (mentalState != 0)
                            mentalScrambler = CreateScrambler(mentalString, "Calm");
                        mentalState =0;
                    }
                    else if (value > 5)
                    {

                        if (mentalState != 1)
                            mentalScrambler = CreateScrambler(mentalString, "Irrational");
                        mentalState = 1;
                    }
                    else if (value > 0)
                    {
                        if (mentalState != 2)
                            mentalScrambler = CreateScrambler(mentalString, "Insane");
                        mentalState = 2;
                    }
                    else
                    {
                        if (mentalState != 3)
                            mentalScrambler = CreateScrambler(mentalString, "Suicidal");
                        mentalState = 3;
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
					if (value > 5)
                    {
                        if (hungerState != 0)
						    hungerScrambler = CreateScrambler(hungerString, "Full");
                        hungerState = 0;
                    }
					else if (value > 0)
                    {
                        if (hungerState != 1)
						    hungerScrambler = CreateScrambler(hungerString, "Hungry");
                        hungerState = 1;
                    }
					else
                    {
                        if (hungerState != 2)
						    hungerScrambler = CreateScrambler(hungerString, "Famished");
                        hungerState = 2;
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
            health = 15;
            mental = 15;
            hunger = 10;
            _timer = 0.0f;
            _resultText = _name + " is just chilling in the back";
        }
    }
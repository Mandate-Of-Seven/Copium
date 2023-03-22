/*!***************************************************************************************
\file			CrewMenu.cs
\project
\author			Sean Ngo
\co-author		Zacharie Hong
\co-author		Shawn Tanary

\par			Course: GAM250
\par			Section:
\date			10/03/2023

\brief
	Contains functions needed for the crew menu of the left screen

All content � 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
using CopiumEngine;
using System.Collections.Generic;
using System;
using System.Linq;

public class CrewMenu: CopiumScript
{
    public static CrewMenu Instance;

    public Text suppliesText;
    public Text titleText;

    string titleString;

    public Button prepareButton;
    public Button deployButton;
    ButtonWrapper prepareBtnWrapper;
    ButtonWrapper deployBtnWrapper;

    public CrewStatusManager crewStatusManager; 
    public ReportScreenManager reportScreenManager; 
    public ResultManager resultManager; 

    public Fade fader;

    public int supplies;

    public Crew harris;
    public Crew bronson;
    public Crew chuck;
    public Crew danton;

    public bool preparing = false;
    public bool deploying = false;

    float timer = 0.0f;

    float healthInterval = 5.0f; // The interval in seconds in which the crew members lose health
    float hungerInterval = 5.0f; // The interval in seconds in which the crew members get hungry

    public bool storageComparment = true; // For the food storage

    //to record who deploys
    public Prepare prepareManager;
    public bool hDeploy, bDeploy, cDeploy, dDeploy;

    public float timeElasped = 0;
    public float transitionDuration = 2;


    public enum STAT_TYPES
    {
        ALIVE,
        HEALTH,
        MENTAL,
        HUNGER,
    }

    public Dictionary<string, Person> crewMembers = new Dictionary<string, Person>();

    void Awake()
    {
        Instance = this;
        crewMembers.Add("Harris",new Person("Harris"));
        crewMembers.Add("Bronson", new Person("Bronson"));
        crewMembers.Add("Chuck", new Person("Chuck"));
        crewMembers.Add("Danton", new Person("Danton"));
    }

    void Start()
	{
        prepareBtnWrapper = new ButtonWrapper(prepareButton);
        prepareBtnWrapper.SetText(prepareButton.GetComponent<Text>());
        prepareBtnWrapper.SetImage(prepareButton.GetComponent<Image>());
        prepareBtnWrapper.failureText = "The train needs to be stopped first!";
        deployBtnWrapper = new ButtonWrapper(deployButton);
        deployBtnWrapper.SetText(deployButton.GetComponent<Text>());
        deployBtnWrapper.SetImage(deployButton.GetComponent<Image>());
        deployBtnWrapper.failureText = "You need to be preparing your crew members first!";
        deployBtnWrapper.SetInteractable(false);
        titleString = titleText.text;
    }

	void Update()
	{
        //if prepare button is pressed
        //show what event happened
        //update values based on event that happened
        //have condition for when certain values hit 0??
        if (prepareBtnWrapper.GetState() == ButtonState.OnClick)
        {
            SetPrepare(!preparing);
            hDeploy = harris.isDeployed;
            bDeploy = bronson.isDeployed;
            cDeploy = chuck.isDeployed;
            dDeploy = danton.isDeployed;
        }

        bool selectedCrewForDeployment = harris.isDeployed || bronson.isDeployed || chuck.isDeployed || danton.isDeployed;
        deployBtnWrapper.SetInteractable(selectedCrewForDeployment);
        if (deployBtnWrapper.GetState() == ButtonState.OnClick)
        {
            preparing = false;
            deploying = true;
            hDeploy = harris.isDeployed;
            bDeploy = bronson.isDeployed;
            cDeploy = chuck.isDeployed;
            dDeploy = danton.isDeployed;
            deployBtnWrapper.SetInteractable(false);
            StartPrepare();
        }
       
       if (crewStatusManager.isCrewStatusOn)
       {
            UpdateEffects();
            UpdateTexts();
            timeElasped += Time.deltaTime;
       }
    }

    /*******************************************************************************
	/*!
	    \brief
		    Updates the text scramler effect for the different health,mental and 
            hunger states
	*/
    /*******************************************************************************/
    void UpdateEffects()
    {
        foreach (var pair in crewMembers)
        {
            Person person = pair.Value;
            if (!person.healthScrambler.Done())
            {
                person.crewScript.healthT.text = person.healthScrambler.Scramble();
            }
            if (!person.mentalScrambler.Done())
            {
                person.crewScript.mentalT.text = person.mentalScrambler.Scramble();
            }
            if (!person.hungerScrambler.Done())
            {
                person.crewScript.hungerT.text = person.hungerScrambler.Scramble();
            }
            person.crewScript.sprite.color = Color.Lerp(person.crewScript.sprite.color,person.targetColor,Time.deltaTime);
        }
    }

    /*******************************************************************************
	/*!
	    \brief
		    Updates the stats of a crew member
	*/
    /*******************************************************************************/
    public void UpdateAllStats()
    {
        CheckCrewHealth();
    }

    /*******************************************************************************
	/*!
	    \brief
		    Checks if all the crew is alive
	*/
    /*******************************************************************************/
    public bool CheckAllCrewAlive()
    {
        foreach (Person person in crewMembers.Values)
        {
            if (person.alive)
                return true;
        }

        return false;
    }

    /*******************************************************************************
	/*!
	    \brief
		    Reduce crew hunger if supplies is 0
	*/
    /*******************************************************************************/
    public void UpdateHunger()
    {
        if(supplies == 0)
        {
            //if (timer >= hungerInterval)
            //{
            //    for (int i = 0; i < crew.Length; i++)
            //    {
            //        if (person.hunger > 0)
            //            person.hunger -= 1;
            //    }

            //    timer = 0.0f;
            //}

            //timer += Time.deltaTime;
            foreach (Person person in crewMembers.Values)
            {
                if (person.hunger > 0)
                    person.hunger -= 1;
            }
        }
    }

    /*******************************************************************************
	/*!
	    \brief
		    Reduce health if hunger is 0
	*/
    /*******************************************************************************/
    void CheckCrewHealth()
    {
        foreach (Person person in crewMembers.Values)
        {
            if (person.hunger <= 0)
            {
                if(person.timer >= healthInterval)
                {
                    person.health -= 1;
                    person.timer = 0.0f;
                }

                person.timer += Time.deltaTime;
            }
        }
    }

    /*******************************************************************************
	/*!
	    \brief
		    Updates the supplies text
	*/
    /*******************************************************************************/
    void UpdateTexts()
    {
        suppliesText.text = "Supplies: " + supplies;
    }

    /*******************************************************************************
	/*!
	    \brief
		    Sets the supplies amount
	*/
    /*******************************************************************************/
    public void SetSupplies(int amount)
    {
        // Bean: Visually show the supply reducing

        if (!storageComparment)
            return;

        supplies = amount;
    }

    /*******************************************************************************
	/*!
	    \brief
		    Changes the supplies amount
	*/
    /*******************************************************************************/
    public void ChangeSupplies(int amount)
    {
        // Bean: Visually show the supply reducing
        prepareManager.suppliesChangedAmount = amount;

        if (!storageComparment)
        {
            supplies += amount;
            if (supplies < 0)
                supplies = 0;
            else if (supplies > 5)
                supplies = 5;

            return;
        }

        supplies += amount;

        if (supplies < 0)
            supplies = 0;
    }

    /*******************************************************************************
	/*!
	    \brief
		    Sets one stat of all the crewmates to an amount
	*/
    /*******************************************************************************/
    public void SetAllCrew(STAT_TYPES types, int amount)
    {
        foreach (Person person in crewMembers.Values)
        {
            switch (types)
            {
                case STAT_TYPES.ALIVE:
                    person.alive = (amount > 0) ? true : false;
                    break;
                case STAT_TYPES.HEALTH:
                    person.health = amount;
                    break;
                case STAT_TYPES.MENTAL:
                    person.mental = amount;
                    break;
                case STAT_TYPES.HUNGER:
                    person.hunger = amount;
                    break;
            }
        }
    }

    /*******************************************************************************
	/*!
	    \brief
		    Changes one stat of all the crewmates to an amount if its still more 
            than 0
	*/
    /*******************************************************************************/
    public void ChangeAllCrew(STAT_TYPES types, int amount)
    {
        foreach (Person person in crewMembers.Values)
        {
            switch (types)
            {
                case STAT_TYPES.HEALTH:
                    if (person.health > 0)
                        person.health += amount;

                    if (person.health > 3)
                        person.health = 3;
                    break;
                case STAT_TYPES.MENTAL:
                    if (person.mental > 0)
                        person.mental += amount;

                    if (person.mental > 3)
                        person.mental = 3;
                    break;
                case STAT_TYPES.HUNGER:
                    if(person.hunger > 0)
                        person.hunger += amount;

                    if (person.hunger > 3)
                        person.hunger = 3;
                    break;
            }
        }
    }

    /*******************************************************************************
       /*!
           \brief
               Sets a certain crew members stat to an amount
       */
    /*******************************************************************************/
    public void SetCrew(STAT_TYPES types, string name, int amount)
    {
        switch (types)
        {
            case STAT_TYPES.ALIVE:
                crewMembers[name].alive = (amount > 0) ? true : false;
                break;
            case STAT_TYPES.HEALTH:
                crewMembers[name].health = amount;
                break;
            case STAT_TYPES.MENTAL:
                crewMembers[name].mental = amount;
                break;
            case STAT_TYPES.HUNGER:
                crewMembers[name].hunger = amount;
                break;
        }
    }

    /*******************************************************************************
	/*!
	    \brief
		    Changes one stat of a crewmate to an amount if its still more 
            than 0
	*/
    /*******************************************************************************/
    public void ChangeCrew(STAT_TYPES types, string name, int amount)
    {
        switch (types)
        {
            case STAT_TYPES.HEALTH:
                if (crewMembers[name].health > 0)
                    crewMembers[name].health += amount;

                if (crewMembers[name].health > 3)
                    crewMembers[name].health = 3;
                break;
            case STAT_TYPES.MENTAL:
                if (crewMembers[name].mental > 0)
                    crewMembers[name].mental += amount;

                if (crewMembers[name].mental > 3)
                    crewMembers[name].mental = 3;
                break;
            case STAT_TYPES.HUNGER:
                if(crewMembers[name].hunger > 0)
                    crewMembers[name].hunger += amount;

                if (crewMembers[name].hunger > 3)
                    crewMembers[name].hunger = 3;
                break;
        }
    }

    /*******************************************************************************
    /*!
           \brief
               Generates a random event for each deployed crew member
    */
    /*******************************************************************************/
    public void StartPrepare()
    {
        if (!resultManager.isResultOn)
        {
            crewStatusManager.ClosePanel(true);
            reportScreenManager.ClosePanel();
            resultManager.OpenPanel();
        }

        int index = 0;
        Person[] temp = new Person[4];

        if (harris.isDeployed)
        {
            ChangeCrew(STAT_TYPES.HUNGER, "Harris", -1);
            //prepareManager.GenerateEvents(crew[0]);
            temp[index++] = crewMembers["Harris"];
        }
        if (bronson.isDeployed)
        {
            ChangeCrew(STAT_TYPES.HUNGER, "Bronson", -1);
            //prepareManager.GenerateEvents(crew[1]);
            temp[index++] = crewMembers["Bronson"];
        }
        if (chuck.isDeployed)
        {
            ChangeCrew(STAT_TYPES.HUNGER, "Chuck", -1);
            //prepareManager.GenerateEvents(crew[2]);
            temp[index++] = crewMembers["Chuck"];
        }
        if (danton.isDeployed)
        {
            ChangeCrew(STAT_TYPES.HUNGER, "Danton", -1);
            //prepareManager.GenerateEvents(crew[3]);
            temp[index++] = crewMembers["Danton"];
        }

        prepareManager.GenerateEvents(temp);
    }

    /*******************************************************************************
    /*!
        \brief
            Toggles wheter a button is clickable
    */
    /*******************************************************************************/
    public void SetClickable(bool clickable)
    {
        prepareBtnWrapper.SetInteractable(clickable);
    }

    /*******************************************************************************
    /*!
        \brief
            Toggles whether or not you can select crew member to send out
    */
    /*******************************************************************************/
    public void SetPrepare(bool _preparing)
    {
        preparing = _preparing;
        if (preparing)
        {
            titleText.text = "Selecting Members";
            harris.Enable();
            chuck.Enable();
            danton.Enable();
            bronson.Enable();
        }
        else
        {
            titleText.text = titleString;
            harris.Disable();
            chuck.Disable();
            danton.Disable();
            bronson.Disable();
        }
    }
}

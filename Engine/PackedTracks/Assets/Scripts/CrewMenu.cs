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
    public CabinInteractions interactionManager;

    public Fade fader;

    public int supplies;

    public Crew harris;
    public Crew bronson;
    public Crew chuck;
    public Crew danton;

    public bool preparing = false;
    public bool deploying = false;
    public bool interacting = false; 

    //float timer = 0.0f;

    float healthInterval = 5.0f; // The interval in seconds in which the crew members lose health

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

    public void DetachStorageCompartment()
    {
        storageComparment = false;
    }

    public Dictionary<string, Person> crewMembers = new Dictionary<string, Person>()
    {
        { "Harris",new Person("Harris")},
        { "Bronson", new Person("Bronson")},
        { "Chuck", new Person("Chuck")},
        { "Danton", new Person("Danton")}
    };

    void Awake()
    {
        Instance = this;
        harris.person = crewMembers["Harris"];
        bronson.person = crewMembers["Bronson"];
        chuck.person = crewMembers["Chuck"];
        danton.person = crewMembers["Danton"];
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
            interactionManager.SetInteractBtnsActive(false);
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

       }else if(crewStatusManager.isCabinOn)
       {
            //UpdateEffects();
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
        harris.UpdateEffects();
        bronson.UpdateEffects();
        chuck.UpdateEffects();
        danton.UpdateEffects();
    }
    // void UpdateStatusScreenEffects()
    // {

    // }

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
               Generates a random event for each deployed crew member
    */
    /*******************************************************************************/
    public void StartPrepare()
    {
        if (!resultManager.isResultOn)
        {
            crewStatusManager.ReturnToCockpit(true);
            reportScreenManager.ClosePanel();
            resultManager.OpenPanel();
        }
        Person[] deployedMembers = new Person[4];
        if (harris.isDeployed)
        {
            crewMembers["Harris"].hunger -= 1;
            //prepareManager.GenerateEvents(crew[0]);
            deployedMembers[0] = crewMembers["Harris"];
        }
        if (bronson.isDeployed)
        {
            crewMembers["Bronson"].hunger -= 1;
            //prepareManager.GenerateEvents(crew[1]);
            deployedMembers[1] = crewMembers["Bronson"];
        }
        if (chuck.isDeployed)
        {
            crewMembers["Chuck"].hunger -= 1;
            //prepareManager.GenerateEvents(crew[2]);
            deployedMembers[2] = crewMembers["Chuck"];
        }
        if (danton.isDeployed)
        {
            crewMembers["Danton"].hunger -= 1;
            //prepareManager.GenerateEvents(crew[3]);
            deployedMembers[3] = crewMembers["Danton"];
        }

        prepareManager.GenerateEvents(deployedMembers);
    }

    public void ChangeHealth(string name, int amount)
    {
        crewMembers[name].health += amount;
    }

    public void ChangeMental(string name, int amount)
    {
        crewMembers[name].mental += amount;
    }

    public void ChangeHunger(string name, int amount)
    {
        crewMembers[name].hunger += amount;
    }

    public void SetStat(string name, HEALTH_STATE amount)
    {
        crewMembers[name].health = amount;
    }

    public void SetStat(string name, MENTAL_STATE amount)
    {
        crewMembers[name].mental = amount;
    }

    public void SetStat(string name, HUNGER_STATE amount)
    {
        crewMembers[name].hunger = amount;
    }

    public void ChangeAllHealth(int amount)
    {
        foreach (Person person in crewMembers.Values)
        {
            person.health += amount;
        }
    }

    public void ChangeAllMental(int amount)
    {
        foreach (Person person in crewMembers.Values)
        {
            person.mental += amount;
        }
    }

    public void ChangeAllHunger(int amount)
    {
        foreach (Person person in crewMembers.Values)
        {
            person.hunger += amount;
        }
    }

    public void SetAllStats(HEALTH_STATE amount)
    {
        foreach (Person person in crewMembers.Values)
        {
            person.health = amount;
        }
    }

    public void SetAllStats(MENTAL_STATE amount)
    {
        foreach (Person person in crewMembers.Values)
        {
            person.mental = amount;
        }
    }

    public void SetAllStats(HUNGER_STATE amount)
    {
        foreach (Person person in crewMembers.Values)
        {
            person.hunger = amount;
        }
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

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

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
using CopiumEngine;
using System;
using System.Linq;

public class CrewMenu: CopiumScript
{
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

    public AudioManager audioManager;

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
    float hungerInterval = 1.0f; // The interval in seconds in which the crew members get hungry

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

    public Person[] crew = new Person[4]
    {
        new Person("Harris"),
        new Person("Bronson"),
        new Person("Chuck"),
        new Person("Danton")
    };

    void Start()
	{
        prepareBtnWrapper = new ButtonWrapper(prepareButton,audioManager);
        prepareBtnWrapper.SetText(prepareButton.GetComponent<Text>());
        prepareBtnWrapper.SetImage(prepareButton.GetComponent<Image>());
        deployBtnWrapper = new ButtonWrapper(deployButton,audioManager);
        deployBtnWrapper.SetText(deployButton.GetComponent<Text>());
        deployBtnWrapper.SetImage(deployButton.GetComponent<Image>());
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
            //fader.fadeIn = true;
            //fader.shouldFade = true;
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

        CheckCrewStatus();
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
        foreach (Person person in crew)
        {
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
        CheckSupplies();
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
        for (int i = 0; i < crew.Length; i++)
        {
            if (crew[i].alive)
                return true;
        }

        return false;
    }

    /*******************************************************************************
	/*!
	    \brief
		    Checks if the crew member is dead and set the values accordingly
	*/
    /*******************************************************************************/
    void CheckCrewStatus()
    {
        for (int i = 0; i < crew.Length; ++i)
        {
            if (!crew[i].alive)
            {
                crew[i].health = 0;
                crew[i].mental = 0;
                crew[i].hunger = 0;
            }
        }
    }

    /*******************************************************************************
	/*!
	    \brief
		    Reduce crew hunger if supplies is 0
	*/
    /*******************************************************************************/
    void CheckSupplies()
    {
        if(supplies == 0)
        {
            if (timer >= hungerInterval)
            {
                for (int i = 0; i < crew.Length; i++)
                {
                    if (crew[i].hunger > 0)
                        crew[i].hunger -= 1;
                }

                timer = 0.0f;
            }

            timer += Time.deltaTime;
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
        for (int i = 0; i < crew.Length; i++)
        {
            if (crew[i].hunger <= 0)
            {
                if(crew[i].timer >= healthInterval)
                {
                    crew[i].health -= 1;
                    crew[i].timer = 0.0f;
                }

                crew[i].timer += Time.deltaTime;
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

        if (!storageComparment)
            return;

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
        for (int i = 0; i < crew.Length; i++)
        {
            switch (types)
            {
                case STAT_TYPES.ALIVE:
                    crew[i].alive = (amount > 0) ? true : false;
                    break;
                case STAT_TYPES.HEALTH:
                    if(crew[i].health > amount)
                        crew[i].health = amount;
                    break;
                case STAT_TYPES.MENTAL:
                    if (crew[i].mental > amount)
                        crew[i].mental = amount;
                    break;
                case STAT_TYPES.HUNGER:
                    if (crew[i].hunger > amount)
                        crew[i].hunger = amount;
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
        for (int i = 0; i < crew.Length; i++)
        {
            switch (types)
            {
                case STAT_TYPES.HEALTH:
                    if (crew[i].health > 0)
                        crew[i].health += amount;
                    break;
                case STAT_TYPES.MENTAL:
                    if (crew[i].mental > 0)
                        crew[i].mental += amount;
                    break;
                case STAT_TYPES.HUNGER:
                    crew[i].hunger += amount;

                    if (crew[i].hunger > 10)
                        crew[i].hunger = 10;
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
    public void SetCrew(STAT_TYPES types, int index, int amount)
    {
        switch (types)
        {
            case STAT_TYPES.ALIVE:
                crew[index].alive = (amount > 0) ? true : false;
                break;
            case STAT_TYPES.HEALTH:
                if (crew[index].health > amount)
                    crew[index].health = amount;
                break;
            case STAT_TYPES.MENTAL:
                if (crew[index].mental > amount)
                    crew[index].mental = amount;
                break;
            case STAT_TYPES.HUNGER:
                if (crew[index].hunger > amount)
                    crew[index].hunger = amount;
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

        if (harris.isDeployed)
        {
            prepareManager.GenerateEvents(crew[0]);
        }
        else if (bronson.isDeployed)
        {
            prepareManager.GenerateEvents(crew[1]);
        }
        else if (chuck.isDeployed)
        {
            prepareManager.GenerateEvents(crew[2]);
        }
        else if (danton.isDeployed)
        {
            prepareManager.GenerateEvents(crew[3]);
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

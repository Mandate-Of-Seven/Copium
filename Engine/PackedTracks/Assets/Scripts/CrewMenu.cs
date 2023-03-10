/*!***************************************************************************************
\file			CrewMenu.cs
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			10/03/2023

\brief
	This script manages the crew menu where the player is able to deploy crew members
    out to scavenge for resources. It also checks their statistics including their
    health, mental and hunger stats.

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/
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

    // Scramble text effect
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
            person.crewScript.sprite.color = Color.Lerp(Color.white,person.targetColor,timeElasped/transitionDuration);
        }
    }


    // Update the stats of all the crew members
    public void UpdateAllStats()
    {
        CheckSupplies();
        CheckCrewHealth();
    }

    // Check if all the crew members are alive
    public bool CheckAllCrewAlive()
    {
        for (int i = 0; i < crew.Length; i++)
        {
            if (crew[i].alive)
                return true;
        }

        return false;
    }
    
    // Update the crew status if the individual crew is not alive
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

    // Reduce crew hunger if supplies is 0
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

    // Reduce health if hunger is 0
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

            if (crew[i].health <= 0)
                crew[i].alive = false;
        }
    }

    // Update the text that are related to the crew
    void UpdateTexts()
    {
        suppliesText.text = "Supplies: " + supplies;
    }

    // Set the supply amount
    public void SetSupplies(int amount)
    {
        // Bean: Visually show the supply reducing

        if (!storageComparment)
            return;

        supplies = amount;
    }

    // Change the supply amount

    public void ChangeSupplies(int amount)
    {
        // Bean: Visually show the supply reducing

        if (!storageComparment)
            return;

        supplies += amount;

        if (supplies < 0)
            supplies = 0;
    }

    // Set all crew members stats to be of certain amount
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

    // Change all crew members stats by a certain amount
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

    // Set individual crew specific stat to be of an amount
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


    // Generate a random event for each deployed
    public void StartPrepare()
    {
        crewStatusManager.ClosePanel(true);
        reportScreenManager.ClosePanel();
        resultManager.OpenPanel();

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

    // Set whether the button is clickable
    public void SetClickable(bool clickable)
    {
        prepareBtnWrapper.SetInteractable(clickable);
    }

    // Prepare the scavenging members to be deployed
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

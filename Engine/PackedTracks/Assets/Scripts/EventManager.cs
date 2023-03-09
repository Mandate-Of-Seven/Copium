using CopiumEngine;
using System;
using System.Runtime.InteropServices;
using System.Runtime.Remoting.Channels;

public class EventManager: CopiumScript
{
    public GameManager GameManager;
    public Event_Intro eventIntro;
    public Event_01 event01;
    public Event_02 event02;
    public Event_03 event03;
    public Event_Ending eventEnding;
    public CrewMenu crewMenu;

    public Option Option_01;
    public Option Option_02;
    public Option Option_03;

    public Text Body;

    public int EventSequence = 0;
    int choice = 0;

    bool ShowingResolution = false;
    bool SelectingChoice = true;
    bool ShowingMainEvent = true;

    float timer = 0.0f;

    void Start()
	{
        EventSequence = 0;
    }

	void Update()
    {
        if (GameManager.gameEnd)
        {
            if(timer > 3.0f)
            {
                ShowEnding();
            }

            timer += Time.deltaTime;
            return;
        }

        if(GameManager.distanceLeft < 0.99f)
        {
            EventSequence = -1;
            OverrideEvent();
        }    

        if (!crewMenu.CheckAllCrewAlive())
        {
            EventSequence = -2;
            OverrideEvent();
        }

        if (!ShowingResolution && ShowingMainEvent)
            CheckCurrentEvent();

        if (!ShowingResolution && SelectingChoice)
            SelectChoice();

        if (ShowingResolution && !SelectingChoice)
            ShowResolution();

        if (Input.GetKeyDown(KeyCode.Enter))
            UpdateEventSequence();

        crewMenu.UpdateAllStats();
    }

    public void OverrideEvent()
    {
        ShowingResolution = false;
        SelectingChoice = true;
        ShowingMainEvent = true;
    }

    public void UpdateEventSequence()
    {
        if (ShowingMainEvent && SelectingChoice)
            return;

        EventSequence++;
        OverrideEvent();
    }

    void CheckCurrentEvent()
    {
        //Console.WriteLine("Checking Current Event");
        switch (EventSequence)
        {
            case -3:
                eventEnding.Ending(3); // Mid game endings
                break;
            case -2:
                eventEnding.Ending(2); // All dead
                break;
            case -1:
                eventEnding.Ending(1); // Not all dead or all alive
                break;
            case 0:
                ShowingMainEvent = false;
                eventIntro.Event();
                break;
            case 1:
                event01.Event(!crewMenu.crew[0].alive);
                break;
            case 2:
                bool alive = false;
                if (crewMenu.crew[1].alive && crewMenu.crew[2].alive)
                    alive = true;
                event02.Event(alive);
                break;
            case 3:
                int requirement = 0;
                if (crewMenu.crew[0].alive && crewMenu.crew[2].alive 
                    && crewMenu.crew[2].health > 5)
                    requirement = 1;
                else if(crewMenu.crew[0].health < 5 && crewMenu.crew[1].health < 5 
                    && crewMenu.crew[2].health < 5 && crewMenu.crew[0].alive 
                    && crewMenu.crew[1].alive && crewMenu.crew[2].alive 
                    && crewMenu.crew[3].alive)
                    requirement = 2;
                event03.Event(requirement);
                break;
            case 4:
                //Event04();
                break;
            default:
                break;
        }
    }

    public void SelectDefaultChoice()
    {
        if (!SelectingChoice)
            return;

        ShowingResolution = true;
        SelectingChoice = false;
        choice = 1;
    }

    void SelectChoice()
    {
        if (Option_01.btn.state == ButtonState.OnClick)
        {
            ShowingResolution = true;
            SelectingChoice = false;
            choice = 1;
        }
        else if(Option_02.btn.state == ButtonState.OnClick)
        {
            ShowingResolution = true;
            SelectingChoice = false;
            choice = 2;
        }
        else if(Option_03.btn.state == ButtonState.OnClick)
        {
            ShowingResolution = true;
            SelectingChoice = false;
            choice = 3;
        }
        else
        {
            choice = 0;
        }
        
    }

    void ShowResolution()
    {
        ShowingMainEvent = false;
        Option_01.ResetOption();
        Option_02.ResetOption();
        Option_03.ResetOption();

        switch (EventSequence)
        {
            case 1:
                event01.Result(choice);
                break;
            case 2:
                event02.Result(choice);
                break;
            case 3:
                event03.Result(choice);
                break;
        }

        ShowingResolution = false;
    }

    void ShowEnding()
    {
        Option_01.ResetOption();
        Option_02.ResetOption();
        Option_03.ResetOption();

        Option_01.Enable();
        Option_02.Enable();
        Option_03.Enable();

        Option_01.txt.text = "Restart Game";
        Option_02.txt.text = "Back to Main Menu";
        Option_03.txt.text = "Quit Game";

        if (Option_01.btn.state == ButtonState.OnClick)
        {
            SceneManager.LoadScene("Demo");
        }
        else if (Option_02.btn.state == ButtonState.OnClick)
        {
            SceneManager.LoadScene("MainMenu");
        }
        else if (Option_03.btn.state == ButtonState.OnClick)
        {
            Application.Quit();
        }
    }
}

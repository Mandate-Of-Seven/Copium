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

    public GameObject Option_01;
    public GameObject Option_02;
    public GameObject Option_03;
    public GameObject Next_Event;

    public Text Body;

    public Text Option_01_Text;
    public Text Option_02_Text;
    public Text Option_03_Text;

    public int EventSequence = 0;
    int choice = 0;

    bool ShowingResolution = false;
    bool SelectingChoice = true;
    bool ShowingMainEvent = true;

    Button option01_btn;
    Button option02_btn;
    Button option03_btn;
    Button next_btn;

    void Start()
	{
        Console.WriteLine("EVENT MANAGER START");
        EventSequence = 0;
        option01_btn = Option_01.GetComponent<Button>();
        option02_btn = Option_02.GetComponent<Button>();
        option03_btn = Option_03.GetComponent<Button>();
        next_btn = Next_Event.GetComponent<Button>();
    }

	void Update()
    {
        if (!crewMenu.CheckAllCrewAlive())
        {
            EventSequence = -2;
            OverideEvent();
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

    public void OverideEvent()
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
        OverideEvent();
    }

    void CheckCurrentEvent()
    {
        switch (EventSequence)
        {
            case -2:
                eventEnding.Ending(true); // All dead
                break;
            case -1:
                eventEnding.Ending(false); // Not all dead
                break;
            case 0:
                ShowingMainEvent = false;
                eventIntro.Event();
                break;
            case 1:
                event01.Event(crewMenu.crew[0].alive);
                break;
            case 2:
                bool alive = true;
                if (crewMenu.crew[1].alive && crewMenu.crew[2].alive)
                    alive = true;
                event02.Event(alive);
                break;
            case 3:
                bool chuckAndHarrisAlive = true;
                event03.Event(chuckAndHarrisAlive);
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

        if (Next_Event.activeSelf)
            Next_Event.SetActive(false);

        ShowingResolution = true;
        SelectingChoice = false;
        choice = 1;
    }

    void SelectChoice()
    {
        if (Next_Event.activeSelf)
            Next_Event.SetActive(false);

        if (option01_btn.state == ButtonState.OnClick)
        {
            ShowingResolution = true;
            SelectingChoice = false;
            choice = 1;
        }
        else if(option02_btn.state == ButtonState.OnClick)
        {
            ShowingResolution = true;
            SelectingChoice = false;
            choice = 2;
        }
        else if(option03_btn.state == ButtonState.OnClick)
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

        if (Option_01.activeSelf || Option_02.activeSelf || Option_03.activeSelf)
        {
            Option_01.SetActive(false);
            Option_02.SetActive(false);
            Option_03.SetActive(false);
        }

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

        //if (!Next_Event.activeSelf)
        //    Next_Event.SetActive(true);

        //if(next_btn.state == ButtonState.OnClick)
        //{
        //    Console.WriteLine("Test");
        //    ShowingResolution = false;
        //    Next_Event.SetActive(false);
        //}
        ShowingResolution = false;
    }
}

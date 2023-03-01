using CopiumEngine;
using System;
using System.Runtime.Remoting.Channels;

public class EventManager: CopiumScript
{
    public GameManager GameManager;
    public Event_Intro eventIntro;
    public Event_01 event01;
    public Event_02 event02;
    public Event_03 event03;
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
    bool ShowingMainEvent = true;

    Button option01_btn;
    Button option02_btn;
    Button option03_btn;
    Button next_btn;

    void Start()
	{
        EventSequence = 0;
        option01_btn = Option_01.GetComponent<Button>();
        option02_btn = Option_02.GetComponent<Button>();
        option03_btn = Option_03.GetComponent<Button>();
        next_btn = Next_Event.GetComponent<Button>();
    }

	void Update()
    {
        if(!ShowingResolution && ShowingMainEvent)
            CheckCurrentEvent();

        if(!ShowingResolution)
            SelectChoice();

        if(ShowingResolution)
            ShowResolution();

        if (Input.GetKeyDown(KeyCode.Enter))
            UpdateEventSequence();
    }

    public void UpdateEventSequence()
    {
        EventSequence++;
        ShowingMainEvent = true;
    }

    void CheckCurrentEvent()
    {
        // Update eventsequence after every button click
        switch (EventSequence)
        {
            case -1:
                Body.text = "Ending~";
                break;
            case 0:
                eventIntro.Event();
                break;
            case 1:
                bool healthy = false;
                // if (crewMenu.health1 != 0)
                //     healthy = true;

                event01.Event(healthy);
                break;
            case 2:
                bool alive = false;
                // if (crewMenu.health2 != 0 && crewMenu.health3 != 0)
                //     alive = true;

                event02.Event(alive);
                break;
            case 3:
                //event03.Event(false);
                break;
            case 4:
                //Event04();
                break;
            case 5:
                //Event05();
                break;
            default:
                break;
        }
    }

    void SelectChoice()
    {
        if (Next_Event.activeSelf)
            Next_Event.SetActive(false);

        if (option01_btn.state == ButtonState.OnClick)
        {
            ShowingResolution = true;
            choice = 1;
        }
        else if(option02_btn.state == ButtonState.OnClick)
        {
            ShowingResolution = true;
            choice = 2;
        }
        else if(option03_btn.state == ButtonState.OnClick)
        {
            ShowingResolution = true;
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
        }

        if (!Next_Event.activeSelf)
            Next_Event.SetActive(true);

        if(next_btn.state == ButtonState.OnClick)
        {
            ShowingResolution = false;
            Next_Event.SetActive(false);
        }
    }
}

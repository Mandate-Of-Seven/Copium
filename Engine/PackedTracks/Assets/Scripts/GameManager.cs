using CopiumEngine;
using System;
using System.Collections.Generic;

public class GameManager: CopiumScript
{
    public EventManager EventManager;
    public AudioManager audioManager;

	public GameObject TrainCanvas;
    public GameObject PauseCanvas;

    public GameObject ManualPopUp;
    public GameObject MainPage;
    public GameObject Page2;
    public GameObject prevButtonObject;


    public Button ManualBtn;
    public Button ManualPopUpBtn;

    public Button PauseResumeBtn;
    public Button PauseQuitBtn;

    public CrewMenu crewMenuScript;
    public TrainManager trainManager;
    public ReportScreenManager reportScreenManager;
    public CrewStatusManager crewStatusManager;
    public ResultManager resultManager;
    public PauseMenu pauseMenu;

    public float distanceLeft = 200;

    public float distanceInterval = 1.0f;
    float foodTimer = 0.0f;
    float hungerTimer = 0.0f;
    float timer = 0.0f;
    float distancePerEvent = 10.0f;

    bool updateEvent = false;
    public bool gameEnd = false;

    bool moving = false;

    void Start()
	{
        //UpdateCanvases();
    }

    void OpenReportScreen()
    {
        audioManager.clickSFX.Play();
    }

	void Update()
    {
        ButtonInputs();
        if (pauseMenu.isPaused)
            return;

        if (Input.GetKeyDown(KeyCode.C))
            crewMenuScript.SetCrew(CrewMenu.STAT_TYPES.ALIVE, 2, 0);

        if (Input.GetKeyDown(KeyCode.B))
            crewMenuScript.SetCrew(CrewMenu.STAT_TYPES.ALIVE, 1, 0);

        if (Input.GetKeyDown(KeyCode.D))
            crewMenuScript.SetCrew(CrewMenu.STAT_TYPES.ALIVE, 3, 0);

        // Toggle canvases
        //CanvasManager();

        // Game ends
        if (CheckForGameEndCondition())
            return;

        // Cant deploy if the train is moving
        if ((moving && trainManager.currentSpeed <= 0f) || (!moving && trainManager.currentSpeed > 0f))
        {
            ToggleMoving();
        }

        // When train is moving and can still travel
        MoveTrain();

        // Reducing supplies for crew to consume
        SupplyCounter();
    }

    bool CheckForGameEndCondition()
    {
        if(!gameEnd && EventManager.EventSequence < 0)
        {
            gameEnd = true;
            trainManager.FlickLever(false);
            crewStatusManager.ClosePanel();
            reportScreenManager.OpenPanel();
        }

        return gameEnd;
    }

    void CanvasManager()
    {
        reportScreenManager.UpdateCanvas();
        crewStatusManager.UpdateCanvas();
        resultManager.UpdateCanvas();
    }

    public void ToggleMoving()
    {
        moving = !moving;
        crewMenuScript.SetClickable(!moving);
    }

    void MoveTrain()
    {
        if (trainManager.accelerate && distanceLeft > 0)
        {
            if (timer >= distanceInterval) // Every few distance interval
            {
                distanceLeft -= trainManager.currentSpeed / 3.0f; // Reduce the distance left

                // Only update event if distance per event is activated
                if (distanceLeft % distancePerEvent < 1.0f && !updateEvent)
                {
                    // Show notifications (Visual & Audio)
                    reportScreenManager.alert.enabled = true;
                    crewStatusManager.alert.enabled = true;

                    updateEvent = true; // Trigger only once
                    EventManager.UpdateEventSequence();
                }
                // Right now if the distance left to the next event is reseted, we can update event again
                else if (distanceLeft % distancePerEvent > 1.0f)
                    updateEvent = false;

                // Close to the next event and has yet to select a choice, select default choice
                if (distanceLeft % distancePerEvent < 5.0f && EventManager.EventSequence > 0 && !updateEvent)
                    EventManager.SelectDefaultChoice();

                timer = 0.0f;
            }

            timer += Time.deltaTime;
        }
    }

    void SupplyCounter()
    {
        if (trainManager.accelerate && distanceLeft < 200.0f)
        {
            if (foodTimer >= 5.0f && crewMenuScript.supplies != 0)
            {
                crewMenuScript.ChangeSupplies(-1);
                crewMenuScript.ChangeAllCrew(CrewMenu.STAT_TYPES.HUNGER, 1);

                foodTimer = 0.0f;
            }
            foodTimer += Time.deltaTime;
        }

        // Game ends
        if(EventManager.EventSequence == -2 && !gameEnd)
        {
            gameEnd = true;
            trainManager.FlickLever();
            distanceLeft = 0.0f;
        }
        else if (EventManager.EventSequence == -3 && !gameEnd)
        {
            gameEnd = true;
            crewMenuScript.fader.shouldFade = true;
            trainManager.FlickLever();
        }
        else if (distanceLeft <= 0.99f && !gameEnd)
        {
            gameEnd = true;
            trainManager.FlickLever();
            EventManager.EventSequence = -1;
            EventManager.OverrideEvent();
        }

        //KeyInputs();
    }

    void ButtonInputs()
    {
        if (ManualBtn.state == ButtonState.OnRelease)
        {
            audioManager.paperSFX.Play();
            ManualPopUp.SetActive(true);
        }
        if (ManualPopUpBtn.state == ButtonState.OnRelease && ManualPopUp.activeSelf)
        {
            Console.WriteLine("HELLO");
            audioManager.paperSFX.Play();
            ManualPopUp.SetActive(false);

            MainPage.SetActive(true);
            Page2.SetActive(false);
            prevButtonObject.SetActive(false);
        }
    }

    // void KeyInputs()
    // {
    //     if (Input.GetKeyDown(KeyCode.P))
    //     {
    //         isPaused = !isPaused;
    //         PauseCanvas.SetActive(isPaused);
    //         if (isPaused)
    //         {
    //             InternalCalls.PauseAllAnimation();
    //         }
    //         else
    //         {
    //             InternalCalls.PlayAllAnimation();
    //         }
    //     }

    //     if (!isPaused)
    //     {
    //         if (Input.GetKeyDown(KeyCode.Escape))
    //         {
    //             isPaused = true;
    //         }
    //     }
    //     else
    //     {
    //         if (PauseResumeBtn.state == ButtonState.OnClick)
    //         {
    //             isPaused = false;
    //             PauseCanvas.SetActive(false);
    //             InternalCalls.PlayAllAnimation();
    //         }

    //         if (PauseQuitBtn.state == ButtonState.OnClick)
    //         {
    //             Application.Quit();
    //         }
    //     }

    //     if (Input.GetKey(KeyCode.P))
    //     {
    //         Application.Quit();
    //     }
    // }
}

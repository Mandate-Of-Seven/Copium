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

    public Button ManualBtn;
    public Button ManualPopUpBtn;

    public Button PauseResumeBtn;
    public Button PauseQuitBtn;

    public CrewMenu crewMenuScript;
    public TrainManager trainManager;
    public ReportScreenManager reportScreenManager;
    public CrewStatusManager crewStatusManager;
    public ResultManager resultManager;

    public Text tracker;

    public bool isPaused = false;
    public float distanceLeft = 200;

    public float distanceInterval = 1.0f;
    float foodTimer = 0.0f;
    float hungerTimer = 0.0f;
    float timer = 0.0f;
    float distancePerEvent = 10.0f;

    bool updateEvent = false;
    public bool gameEnd = false;

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
        KeyInputs();

        if (isPaused)
            return;

        ButtonInputs();

        // Game ends
        CheckForGameEndCondition();

        // Cant deploy if the train is moving
        if (trainManager.currentSpeed > 0)
        {
            crewMenuScript.prepareButton.gameObject.SetActive(false);
            crewMenuScript.deployButton.gameObject.SetActive(false);
        }
        else
        {
            crewMenuScript.prepareButton.gameObject.SetActive(true);
            crewMenuScript.deployButton.gameObject.SetActive(true);
        }

        //Stop travelling
        if (trainManager.currentSpeed > 0 && distanceLeft > 0)
        {
            if (timer >= distanceInterval)
            {
                distanceLeft -= trainManager.currentSpeed/3.0f;
                if (distanceLeft % distancePerEvent < 1.0f && !updateEvent)
                {
                    reportScreenManager.alert.enabled = true;
                    crewStatusManager.alert.enabled = true;
                    updateEvent = true;
                    EventManager.UpdateEventSequence();
                }
                else if(distanceLeft % distancePerEvent > 1.0f)
                    updateEvent = false;

                // Close to the next event and has yet to select a choice
                if (distanceLeft % distancePerEvent < 5.0f && EventManager.EventSequence > 0 && !updateEvent) 
                    EventManager.SelectDefaultChoice();

                timer = 0.0f;
            }
            
            timer += Time.deltaTime;
        }
        tracker.text =  ((int)distanceLeft).ToString() + "KM";

        // Reducing supplies for crew to consume
        if(trainManager.currentSpeed > 0 && distanceLeft < 200.0f)
        {
            if (foodTimer >= 5.0f && crewMenuScript.supplies != 0)
            {
                crewMenuScript.ChangeSupplies(-1);
                crewMenuScript.ChangeAllCrew(CrewMenu.STAT_TYPES.HUNGER, 1);

                foodTimer = 0.0f;
            }
            foodTimer += Time.deltaTime;
        }
    }

    void CheckForGameEndCondition()
    {
        if (EventManager.EventSequence == -2 && !gameEnd)
        {
            gameEnd = true;
            if (trainManager.trainLeverActivated.gameObject.activeSelf)
                trainManager.FlickLever();
            distanceLeft = 0.0f;
            crewStatusManager.ClosePanel();
            reportScreenManager.OpenPanel();
        }
        else if (EventManager.EventSequence == -3 && !gameEnd)
        {
            gameEnd = true;
            crewMenuScript.fader.shouldFade = true;
            if (trainManager.trainLeverActivated.gameObject.activeSelf)
                trainManager.FlickLever();
            crewStatusManager.ClosePanel();
            reportScreenManager.OpenPanel();
        }
        else if (distanceLeft <= 0.99f && !gameEnd)
        {
            gameEnd = true;
            trainManager.FlickLever();
            EventManager.EventSequence = -1;
            EventManager.OverideEvent();
            crewStatusManager.ClosePanel();
            reportScreenManager.OpenPanel();
        }
    }

    void ButtonInputs()
    {
        if (ManualBtn.state == ButtonState.OnRelease)
        {
            audioManager.paperSFX.Play();
            ManualPopUp.SetActive(true);
        }
        if (ManualPopUpBtn.state == ButtonState.OnRelease)
        {
            audioManager.paperSFX.Play();
            ManualPopUpBtn.gameObject.SetActive(false);
        }
    }

    void KeyInputs()
    {
        if (!isPaused)
        {
            if (Input.GetKeyDown(KeyCode.Escape))
            {
                isPaused = true;
            }
        }
        else
        {
            if (PauseResumeBtn.state == ButtonState.OnClick)
            {
                isPaused = false;
                PauseCanvas.SetActive(false);
                InternalCalls.PlayAllAnimation();
            }

            if (PauseQuitBtn.state == ButtonState.OnClick)
            {
                Application.Quit();
            }
        }

        if (Input.GetKeyDown(KeyCode.P))
        {
            isPaused = !isPaused;
            PauseCanvas.SetActive(isPaused);
            if (isPaused)
            {
                InternalCalls.PauseAllAnimation();
            }
            else
            {
                InternalCalls.PlayAllAnimation();
            }
        }
    }
}

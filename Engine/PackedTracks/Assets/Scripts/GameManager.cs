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

    public GameObject CrewTab;
    public GameObject ReportTab;

    public Button ReportScreenBtn;
    public Button ManualBtn;
    public Button ManualPopUpBtn;

    public Button PauseResumeBtn;
    public Button PauseQuitBtn;
    public Button CrewTabBtn;

    public CrewMenu crewMenuScript;
    public TrainManager trainManager;
    
    public Button CloseReportBtn;

    public Text tracker;

    public bool isPaused = false;
    public float distanceLeft = 200;

    public float distanceInterval = 1.0f;
    float foodTimer = 0.0f;
    float hungerTimer = 0.0f;
    float timer = 0.0f;

    bool updateEvent = false;
    bool gameEnd = false;

    Vector3 reportScreenTargetScale = new Vector3(4.0f,4.0f,0);

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
        if (ReportScreenBtn.state == ButtonState.OnRelease)
        {
            isReportScreenOn = true;
            audioManager.clickSFX.Play();
        }
        if(CloseReportBtn.state == ButtonState.OnRelease)
        {
            isReportScreenOn = false;
            audioManager.clickSFX.Play();
        }
        if(CrewTabBtn.state == ButtonState.OnRelease)
        {
            audioManager.clickSFX.Play();
            CrewTab.SetActive(true);
        }
        if(ManualBtn.state == ButtonState.OnRelease)
        {
            audioManager.paperSFX.Play();
            ManualPopUp.SetActive(true);
        }
        if(ManualPopUpBtn.state == ButtonState.OnRelease)
        {
            audioManager.paperSFX.Play();
            ManualPopUpBtn.gameObject.SetActive(false);
        }

        if (isReportScreenOn)
        {
            ReportTab.transform.localScale = Vector3.Lerp(ReportTab.transform.localScale,reportScreenTargetScale,Time.deltaTime);
        }
        else
        {
            ReportTab.transform.localScale = Vector3.Lerp(ReportTab.transform.localScale,Vector3.one,Time.deltaTime);
        }


        //Stop travelling

        if (trainManager.currentSpeed > 0 && distanceLeft > 0)
        {
            if (timer >= distanceInterval)
            {
                distanceLeft -= trainManager.currentSpeed/3.0f;
                if (distanceLeft % 50 < 1.0f && !updateEvent)
                {
                    updateEvent = true;
                    EventManager.UpdateEventSequence();
                }
                else if(distanceLeft % 50 > 1.0f)
                    updateEvent = false;

                // Close to the next event and has yet to select a choice
                if (distanceLeft % 50 < 5.0f && EventManager.EventSequence > 0 && !updateEvent) 
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

        // Game ends
        if(EventManager.EventSequence == -2 && !gameEnd)
        {
            gameEnd = true;
            trainManager.FlickLever();
            distanceLeft = 0.0f;
        }
        else if (distanceLeft <= 0.99f && !gameEnd)
        {
            gameEnd = true;
            trainManager.FlickLever();
            EventManager.EventSequence = -1;
            EventManager.OverideEvent();
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

        if (Input.GetKey(KeyCode.P))
        {
            Application.Quit();
        }
    }
}

using CopiumEngine;
using System;
using System.Collections.Generic;

public class GameManager: CopiumScript
{
    public EventManager EventManager;

	public GameObject TrainCanvas;
    public GameObject PauseCanvas;

    public GameObject ManualPopUp;

    public GameObject CrewTab;
    public GameObject ReportTab;

    public Button LeverFar;
    public Button LeverNear;

    public Button ReportScreenBtn;
    public Button ManualBtn;
    public Button ManualPopUpBtn;

    public Button PauseResumeBtn;
    public Button PauseQuitBtn;
    public Button CrewTabBtn;

    public CrewMenu crewMenuScript;
    
    public Button CloseReportBtn;

    public Text tracker;

    bool isReportScreenOn = false;
    public bool isPaused = false;
    public int distanceLeft = 200;

    public float distanceInterval = 1.0f;
    float foodTimer = 0.0f;
    float hungerTimer = 0.0f;
    float timer = 0.0f;

    int state = 0;

    void Start()
	{
        isReportScreenOn = false;
        //UpdateCanvases();
    }
	void Update()
    {   
        if(ReportScreenBtn.state == ButtonState.OnClick)
        {
            ReportTab.SetActive(true);
        }
        if(CloseReportBtn.state == ButtonState.OnClick)
        {
            ReportTab.SetActive(false);
        }
        if(CrewTabBtn.state == ButtonState.OnClick)
        {
            CrewTab.SetActive(true);
        }
        if(ManualBtn.state == ButtonState.OnClick)
        {
            ManualPopUp.SetActive(true);
        }
        if(ManualPopUpBtn.state == ButtonState.OnClick)
        {
            ManualPopUpBtn.gameObject.SetActive(false);
        }

        if(LeverNear.state == ButtonState.OnClick)
        {
            LeverFar.gameObject.SetActive(true);
            LeverNear.gameObject.SetActive(false);
        }
        //Stop travlling
        else if(LeverFar.state == ButtonState.OnClick || distanceLeft == 0)
        {
            timer = 0.0f;
            LeverFar.gameObject.SetActive(false);
            LeverNear.gameObject.SetActive(true);
        }

        if (LeverFar.gameObject.activeSelf && distanceLeft > 0)
        {
            if (timer >= 0.2f)
            {
                distanceLeft -= 1;
                if (distanceLeft%50 == 0)
                {
                    EventManager.UpdateEventSequence();
                }
                timer = 0.0f;
            }

            if(foodTimer >= 4.0f && crewMenuScript.food != 0)
            {
                crewMenuScript.food -= 1;
                foodTimer = 0.0f;
            }

            foodTimer += Time.deltaTime;
            timer += Time.deltaTime;
        }
        tracker.text =  distanceLeft.ToString() + "KM";

        if (crewMenuScript.food == 0)
        {
            if(hungerTimer >= 1.0f)
            {
                crewMenuScript.hunger1 -= 1;
                crewMenuScript.hunger2 -= 1;
                crewMenuScript.hunger3 -= 1;
                crewMenuScript.hunger4 -= 1;
                hungerTimer = 0.0f;
            }

            hungerTimer += Time.deltaTime;
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

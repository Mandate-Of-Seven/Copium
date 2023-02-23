using CopiumEngine;
using System;
using System.Collections.Generic;

public class GameManager: CopiumScript
{
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

    public Text tracker;

    bool isReportScreenOn = false;
    public bool isPaused = false;
    public int distanceLeft = 200;

    public float distanceInterval = 1.0f;
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
        if(LeverFar.state == ButtonState.OnClick)
        {
            timer = 0.0f;
            LeverFar.gameObject.SetActive(false);
            LeverNear.gameObject.SetActive(true);
        }

        if (LeverFar.gameObject.activeSelf)
        {
            if (timer >= 1.0f)
            {
                distanceLeft -= 1;
                if (distanceLeft%50 == 0)
                {
                    Console.WriteLine("Trigger Event");
                }
                timer = 0.0f;
            }
            timer += Time.deltaTime;
        }
        tracker.text =  distanceLeft.ToString() + "KM";

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

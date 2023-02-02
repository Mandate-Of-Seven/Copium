using CopiumEngine;
using System;
using System.Collections.Generic;

public class GameManager: CopiumScript
{
	public GameObject TrainCanvas;
	public GameObject MainScreenCanvas;
	public GameObject CombatCanvas;
    public GameObject PauseCanvas;

    public GameObject ReportTab;
    public GameObject MessageTab;
    public GameObject CrewTab;

    public Button ReportScreenBtn;
    public Button CombatScreenBtn;
    public Button OtherScreenBtn;

    public Button ReportTabBtn;
    public Button MessageTabBtn;
    public Button CrewTabBtn;

    public AudioSource audio;

    bool isReportScreenOn = false;
    public bool isPaused = false;

    public ShooterBehaviour ally1;
    public ShooterBehaviour ally2;
    public ShooterBehaviour ally3;
    public ShooterBehaviour ally4;

    public ShooterBehaviour enemy1;
    public ShooterBehaviour enemy2;
    public ShooterBehaviour enemy3;
    public ShooterBehaviour enemy4;

    void Start()
	{
        isReportScreenOn = false;
        PauseCanvas.SetActive(false);
        audio.Play();

        UpdateCanvases();
    }
	void Update()
    {
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
            if (ReportScreenBtn.state == ButtonState.OnClick)
            {
                isReportScreenOn = true;
                UpdateCanvases();
            }

            if (Input.GetKeyDown(KeyCode.Escape))
            {
                if (isReportScreenOn)
                {
                    isReportScreenOn = false;
                    UpdateCanvases();
                }
            }

            if (isReportScreenOn)
            {
                UpdateTabs();
            }
        }
    }

    void UpdateTabs()
    {
        if (ReportTabBtn.state == ButtonState.OnClick)
        {
            ReportTab.SetActive(true);
            MessageTab.SetActive(false);
            CrewTab.SetActive(false);
        }
        else if(MessageTabBtn.state == ButtonState.OnClick)
        {
            ReportTab.SetActive(false);
            MessageTab.SetActive(true);
            CrewTab.SetActive(false);
        }
        else if (CrewTabBtn.state == ButtonState.OnClick)
        {
            ReportTab.SetActive(false);
            MessageTab.SetActive(false);
            CrewTab.SetActive(true);
        }
    }

    void UpdateCanvases()
    {
        if(isReportScreenOn)
        {
            if(!MainScreenCanvas.activeSelf)
                MainScreenCanvas.SetActive(true);

            if(CombatCanvas.activeSelf)
                CombatCanvas.SetActive(false);
        }
        else
        {
            if (!TrainCanvas.activeSelf)
                TrainCanvas.SetActive(true);

            if (MainScreenCanvas.activeSelf)
                MainScreenCanvas.SetActive(false);

            if (CombatCanvas.activeSelf)
                CombatCanvas.SetActive(false);
        }
    }
}

using CopiumEngine;
using System;
using System.Collections.Generic;

public class GameManager: CopiumScript
{
    public GameObject MainMenuCanvas;
	public GameObject TrainCanvas;
	public GameObject MainScreenCanvas;
	public GameObject CombatCanvas;
    public GameObject PauseCanvas;

    public GameObject MainMenuStartGameObject;
    Button MainMenuStartBtn;

    public GameObject ReportTab;
    public GameObject MessageTab;
    public GameObject CrewTab;

    public GameObject ReportScreenGameObject;
    public Button ReportScreenBtn;
    public Button CombatScreenBtn;
    public Button OtherScreenBtn;
    public Button ResumeBtn;

    public Button ReportTabBtn;
    public Button MessageTabBtn;
    public Button CrewTabBtn;

    bool isReportScreenOn = false;
    int state = 0;
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

        ReportScreenBtn = ReportScreenGameObject.GetComponent<Button>();
        MainMenuStartBtn = MainMenuStartGameObject.GetComponent<Button>();
    }
	void Update()
    {
        if (MainMenuStartBtn.state == ButtonState.OnClick)
        {
            state = 1;
            UpdateCanvases();
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
        else
        {
            if (ResumeBtn.state == ButtonState.OnClick)
            {
                isPaused = false;
                PauseCanvas.SetActive(false);
                InternalCalls.PlayAllAnimation();
            }
        }

        if (Input.GetKey(KeyCode.P))
        {
            Application.Quit();
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
        if (state == 0)
        {
            if (!MainMenuCanvas.activeSelf)
                MainMenuCanvas.SetActive(true);

            if (TrainCanvas.activeSelf)
                TrainCanvas.SetActive(false);

            if (MainScreenCanvas.activeSelf)
                MainScreenCanvas.SetActive(false);

            if (CombatCanvas.activeSelf)
                CombatCanvas.SetActive(false);
        }
        else if (isReportScreenOn)
        {
            if(!MainScreenCanvas.activeSelf)
                MainScreenCanvas.SetActive(true);

            if(CombatCanvas.activeSelf)
                CombatCanvas.SetActive(false);
        }
        else if (state == 1)
        {
            if (MainMenuCanvas.activeSelf)
                MainMenuCanvas.SetActive(false);

            if (!TrainCanvas.activeSelf)
                TrainCanvas.SetActive(true);

            if (MainScreenCanvas.activeSelf)
                MainScreenCanvas.SetActive(false);

            if (CombatCanvas.activeSelf)
                CombatCanvas.SetActive(false);
        }
    }
}

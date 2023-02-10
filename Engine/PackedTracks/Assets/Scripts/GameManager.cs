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

    public GameObject ManualPopUp;
    

    public GameObject MainMenuStartGameObject;
    public GameObject MainMenuQuitGameObject;
    Button MainMenuStartBtn;
    Button MainMenuQuitBtn;

    public GameObject ReportTab;
    public GameObject MessageTab;
    public GameObject CrewTab;

    public GameObject ReportScreenGameObject;
    public Button ReportScreenBtn;
    public Button CombatScreenBtn;
    public Button OtherScreenBtn;
    public Button ManualBtn;
    public Button PauseResumeBtn;
    public Button PauseQuitBtn;

    public Button ReportTabBtn;
    public Button MessageTabBtn;
    public Button CrewTabBtn;

    bool isReportScreenOn = false;
    public bool isPaused = false;
    int state = 0;

    void Start()
	{
        isReportScreenOn = false;
        PauseCanvas.SetActive(false);
        Console.WriteLine("CRASH1");
        UpdateCanvases();

        Console.WriteLine("CRASH2");
        ReportScreenBtn = ReportScreenGameObject.GetComponent<Button>();
        Console.WriteLine("CRASH3");
        MainMenuStartBtn = MainMenuStartGameObject.GetComponent<Button>();
        Console.WriteLine("CRASH4");
        MainMenuQuitBtn = MainMenuQuitGameObject.GetComponent<Button>();
    }
	void Update()
    {
        if (MainMenuStartBtn.state == ButtonState.OnClick)
        {
            state = 1;
            UpdateCanvases();
        }

        if (MainMenuQuitBtn.state == ButtonState.OnClick)
        {
            Application.Quit();
        }

        if(ManualBtn.state == ButtonState.OnClick)
        {
            ManualPopUp.SetActive(true);
            ReportScreenGameObject.SetActive(false);
        }



        if (Input.GetKeyDown(KeyCode.P))
        {
            isPaused = !isPaused;
            PauseCanvas.SetActive(isPaused);
            InternalCalls.PauseAudio(isPaused);
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
            if (!ReportScreenGameObject.activeSelf && !ManualPopUp.activeSelf)
                ReportScreenGameObject.SetActive(true);

            if (ReportScreenBtn.state == ButtonState.OnClick)
            {
                isReportScreenOn = true;
                UpdateCanvases();
                ManualBtn.gameObject.SetActive(false);
            }

            if (Input.GetKeyDown(KeyCode.Escape))
            {
                if (ManualPopUp.activeSelf)
                    ManualPopUp.SetActive(false);
                else if (isReportScreenOn)
                {
                    isReportScreenOn = false;
                    UpdateCanvases();
                    ManualBtn.gameObject.SetActive(true);
                }
            }

            if (isReportScreenOn)
            {
                UpdateTabs();
            }
        }
        else
        {
            if(ReportScreenGameObject.activeSelf)
                ReportScreenGameObject.SetActive(false);

            if (PauseResumeBtn.state == ButtonState.OnClick)
            {
                isPaused = false;
                PauseCanvas.SetActive(false);
                InternalCalls.PlayAllAnimation();
                InternalCalls.PauseAudio(false);
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

            if (CombatCanvas.activeSelf)
                CombatCanvas.SetActive(false);
        }
        else if (state == 1)
        {
            if (MainMenuCanvas.activeSelf)
                MainMenuCanvas.SetActive(false);

            if (!TrainCanvas.activeSelf)
                TrainCanvas.SetActive(true);

            if (ManualPopUp.activeSelf)
                ManualPopUp.SetActive(false);

            if (MainScreenCanvas.activeSelf)
                MainScreenCanvas.SetActive(false);

            if (CombatCanvas.activeSelf)
                CombatCanvas.SetActive(false);
        }
    }
}

using CopiumEngine;
using System;

public class GameManager: CopiumScript
{
	public GameObject TrainCanvas;
	public GameObject MainScreenCanvas;
	public GameObject CombatCanvas;

    public GameObject ReportTab;
    public GameObject MessageTab;
    public GameObject CrewTab;

    public Button ReportScreenBtn;
    public Button CombatScreenBtn;
    public Button OtherScreenBtn;

    public Button ReportTabBtn;
    public Button MessageTabBtn;
    public Button CrewTabBtn;

    bool isReportScreenOn = false;

	void Start()
	{
        isReportScreenOn = false;

        UpdateCanvases();
    }
	void Update()
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

        Console.WriteLine("Game manager");
        Console.WriteLine(ReportTab.activeSelf);

        if (isReportScreenOn)
        {
            UpdateTabs();
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

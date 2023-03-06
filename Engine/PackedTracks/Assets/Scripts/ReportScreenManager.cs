using CopiumEngine;
using System;

public class ReportScreenManager: CopiumScript
{
    public bool isReportScreenOn = false;
	public AudioManager audioManager;
    
    public Button CloseReportBtn;
	public Button ReportScreenBtn;
    public GameObject ReportTab;

	public Image alert;

    public ResultManager resultManager;

    Vector3 reportScreenTargetScale = new Vector3(4.0f,4.0f,0);

    bool closeHover = false;
	bool openHover = false;

	public GameObject parent;
	
	public float transitionSpeed = 5.0f;

	void Start()
	{

	}
	void Update()
	{
        if (!openHover && ReportScreenBtn.state == ButtonState.OnHover)
        {
            openHover = true;
            audioManager.hoverSFX.Play();
        }
        else if (ReportScreenBtn.state == ButtonState.OnRelease)
        {
            audioManager.clickSFX.Play();
            OpenPanel();
        }
        else if (ReportScreenBtn.state == ButtonState.None)
        {
            openHover = false;
        }

        if (!closeHover && CloseReportBtn.state == ButtonState.OnHover)
        {
            closeHover = true;
            audioManager.hoverSFX.Play();
        }
		else if(CloseReportBtn.state == ButtonState.OnRelease)
        {
            audioManager.clickSFX.Play();
            ClosePanel();
        }        
        else if (CloseReportBtn.state == ButtonState.None)
        {
            closeHover = false;
        }

		
        if (isReportScreenOn)
        {
            ReportTab.transform.localScale = Vector3.Lerp(ReportTab.transform.localScale,reportScreenTargetScale,Time.deltaTime * transitionSpeed);
        }
        else
        {
            ReportTab.transform.localScale = Vector3.Lerp(ReportTab.transform.localScale,Vector3.one,Time.deltaTime * transitionSpeed);
        }
	}

    public void OpenPanel()
    {
        alert.enabled = false;
        isReportScreenOn = true;
        ReportScreenBtn.gameObject.SetActive(false);
        ReportTab.transform.parent = null;
    }

    public void ClosePanel()
    {
        isReportScreenOn = false;
        ReportScreenBtn.gameObject.SetActive(true);
        ReportTab.transform.parent = parent.transform;
    }
}

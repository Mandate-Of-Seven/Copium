using CopiumEngine;
using System;

public class ResultManager: CopiumScript
{
	public bool isResultOn = false;
	public AudioManager audioManager;

    public GameObject manualPanel;
    public CrewStatusManager crewStatus;
    public ReportScreenManager reportScreen;
    
    public Button CloseResultBtn;
	public Button ResultBtn;
    public GameObject ResultTab;

    Vector3 resultTargetScale = new Vector3(5.8f,5.8f,0);

    bool closeHover = false;
	bool openHover = false;

	public GameObject parent;
	
	public float transitionSpeed = 5.0f;

	void Start()
	{

	}
	void Update()
	{

        if (!openHover && ResultBtn.state == ButtonState.OnHover)
        {
            openHover = true;
            audioManager.hoverSFX.Play();
        }
        else if (ResultBtn.state == ButtonState.OnRelease)
        {
			OpenPanel();
            audioManager.clickSFX.Play();
        }
        else if (ResultBtn.state == ButtonState.None)
        {
            openHover = false;
        }

        if (!closeHover && CloseResultBtn.state == ButtonState.OnHover)
        {
            closeHover = true;
            audioManager.hoverSFX.Play();
        }
		else if(CloseResultBtn.state == ButtonState.OnRelease)
        {
            isResultOn = false;
            audioManager.clickSFX.Play();
			ResultBtn.gameObject.SetActive(true);
			ResultTab.transform.parent = parent.transform;
        }        
        else if (CloseResultBtn.state == ButtonState.None)
        {
            closeHover = false;
        }

		
        if (isResultOn)
        {
            ResultTab.transform.localScale = Vector3.Lerp(ResultTab.transform.localScale,resultTargetScale,Time.deltaTime * transitionSpeed);
        }
        else
        {
            ResultTab.transform.localScale = Vector3.Lerp(ResultTab.transform.localScale,Vector3.one,Time.deltaTime * transitionSpeed);
        }
	}

	public void OpenPanel()
	{
		isResultOn = true;
		ResultBtn.gameObject.SetActive(false);
		ResultTab.transform.parent = null;
	}

	public void Disable()
	{
		ResultTab.SetActive(false);
		ResultBtn.gameObject.SetActive(false);
	}

	public void Enable()
	{
        if (reportScreen.isReportScreenOn)
        {
            Console.WriteLine("WHERE UR REPORT");
            return;
        }
        if (crewStatus.isCrewStatusOn)
        {
            Console.WriteLine("WHERE UR CREW");
            return;
        }
		ResultTab.SetActive(true);
		ResultBtn.gameObject.SetActive(true);
	}
}

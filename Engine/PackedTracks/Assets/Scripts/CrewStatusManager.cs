using CopiumEngine;
using System;

public class CrewStatusManager: CopiumScript
{
	public bool isCrewStatusOn = false;
	public AudioManager audioManager;
    
    public Button CloseCrewStatusBtn;
	public Button CrewStatusBtn;
    public ResultManager resultManager;

	public TooltipBehaviour tooltip;


    ButtonWrapper CloseCrewStatusBtnWrapper;
	ButtonWrapper CrewStatusBtnWrapper;

    public GameObject CrewStatusTab;

	public Image alert;

    Vector3 crewStatusTargetScale = new Vector3(5f,5f,0);

	public GameObject parent;
	public CrewMenu crewMenu;
	
	public float transitionSpeed = 5.0f;

	void Start()
	{
		CloseCrewStatusBtnWrapper = new ButtonWrapper(CloseCrewStatusBtn,audioManager, tooltip);
		CloseCrewStatusBtnWrapper.SetImage(CloseCrewStatusBtn.GetComponent<Image>());
		CrewStatusBtnWrapper = new ButtonWrapper(CrewStatusBtn,audioManager,tooltip	);
		CrewStatusBtnWrapper.SetImage(CrewStatusBtn.GetComponent<Image>());
	}

	void Update()
	{
		UpdateCanvas();

    }
	public void UpdateCanvas()
	{   
		if (CrewStatusBtnWrapper.GetState() == ButtonState.OnRelease)
        {
			resultManager.Disable();
            OpenPanel();
        }
		if(CloseCrewStatusBtnWrapper.GetState() == ButtonState.OnRelease)
        {
            ClosePanel(false);
        }
		
        if (isCrewStatusOn)
        {
            CrewStatusTab.transform.localScale = Vector3.Lerp(CrewStatusTab.transform.localScale,crewStatusTargetScale,Time.deltaTime * transitionSpeed);
        }
        else
        {
            CrewStatusTab.transform.localScale = Vector3.Lerp(CrewStatusTab.transform.localScale,Vector3.one,Time.deltaTime * transitionSpeed);
        }
	}

    public void OpenPanel()
    {
		if (isCrewStatusOn)
			return;

		Debug.Log( "DISABLED!");
        alert.enabled = false;
        isCrewStatusOn = true;
        CrewStatusBtn.gameObject.SetActive(false);
        CrewStatusTab.transform.parent = null;
    }
    public void ClosePanel(bool prepared)
	{
		crewMenu.timeElasped = 0;
		//Prevent the crew buttons from being pressed
		if (!crewMenu.deploying && !prepared)
		{
			crewMenu.SetPrepare(false);
		}
		else
		{
			crewMenu.deploying = false;
		}
		isCrewStatusOn = false;
        resultManager.Enable();
        CrewStatusBtn.gameObject.SetActive(true);
		CrewStatusTab.transform.parent = parent.transform;
	}
}

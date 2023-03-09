using CopiumEngine;
using System;

public class CrewStatusManager: CopiumScript
{
	public bool isCrewStatusOn = false;
	public AudioManager audioManager;
    
    public Button CloseCrewStatusBtn;
	public Button CrewStatusBtn;
    public GameObject CrewStatusTab;
    public ResultManager resultManager;


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
		CloseCrewStatusBtnWrapper = new ButtonWrapper(CloseCrewStatusBtn,audioManager);
		CloseCrewStatusBtnWrapper.SetImage(CloseCrewStatusBtn.GetComponent<Image>());
		CrewStatusBtnWrapper = new ButtonWrapper(CrewStatusBtn,audioManager);
		CrewStatusBtnWrapper.SetImage(CrewStatusBtn.GetComponent<Image>());
	}
	void Update()
	{   
		if (CrewStatusBtnWrapper.GetState() == ButtonState.OnRelease)
        {
			resultManager.Disable();
			alert.enabled = false;
            isCrewStatusOn = true;
            OpenPanel();
        }
		if(CloseCrewStatusBtnWrapper.GetState() == ButtonState.OnRelease)
        {
            ClosePanel();
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

        resultManager.Disable();
        alert.enabled = false;
        isCrewStatusOn = true;
        CrewStatusBtn.gameObject.SetActive(false);
        CrewStatusTab.transform.parent = null;
		
    }
    public void ClosePanel()
	{
		//Prevent the crew buttons from being pressed
		crewMenu.SetPrepare(false);
		isCrewStatusOn = false;
		resultManager.Enable();
		CrewStatusBtn.gameObject.SetActive(true);
		CrewStatusTab.transform.parent = parent.transform;
	}
}

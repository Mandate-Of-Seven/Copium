using CopiumEngine;
using System;

public class CrewStatusManager: CopiumScript
{
	public bool isCrewStatusOn = false;
	public AudioManager audioManager;
	public ResultManager resultManager;
    
    public Button CloseCrewStatusBtn;
	public Button CrewStatusBtn;
    public GameObject CrewStatusTab;

	bool closeHover = false;
	bool openHover = false;

	public Image alert;

    Vector3 crewStatusTargetScale = new Vector3(5f,5f,0);

	public GameObject parent;
	
	public float transitionSpeed = 5.0f;

	void Start()
	{

	}
	void Update()
	{   
		if (!openHover && CrewStatusBtn.state == ButtonState.OnHover)
        {
			openHover = true;
			audioManager.hoverSFX.Play();
        }
		else if (CrewStatusBtn.state == ButtonState.OnRelease)
        {
			resultManager.Disable();
			alert.enabled = false;
            isCrewStatusOn = true;
            audioManager.clickSFX.Play();
            OpenPanel();
        }
		else if (CrewStatusBtn.state == ButtonState.None)
		{
			openHover = false;
		}

		if(!closeHover && CloseCrewStatusBtn.state == ButtonState.OnHover)
        {
			closeHover = true;
			audioManager.hoverSFX.Play();
        }
		else if(CloseCrewStatusBtn.state == ButtonState.OnRelease)
        {
            audioManager.clickSFX.Play();
            ClosePanel();
        }
		else if (CloseCrewStatusBtn.state == ButtonState.None)
		{
			closeHover = false;
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
        alert.enabled = false;
        isCrewStatusOn = true;
        CrewStatusBtn.gameObject.SetActive(false);
        CrewStatusTab.transform.parent = null;
    }
    public void ClosePanel()
	{
		isCrewStatusOn = false;
		resultManager.Enable();
		CrewStatusBtn.gameObject.SetActive(true);
		CrewStatusTab.transform.parent = parent.transform;
	}
}

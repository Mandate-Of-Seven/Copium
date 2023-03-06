using CopiumEngine;
using System;

public class CrewStatusManager: CopiumScript
{
	public bool isCrewStatusOn = false;
	public AudioManager audioManager;
    
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
			alert.enabled = false;
            isCrewStatusOn = true;
            audioManager.clickSFX.Play();
			CrewStatusBtn.gameObject.SetActive(false);
			CrewStatusTab.transform.parent = null;
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

	public void ClosePanel()
	{
		isCrewStatusOn = false;
		audioManager.clickSFX.Play();
		CrewStatusBtn.gameObject.SetActive(true);
		Console.WriteLine("CLOSED PANEL");
		CrewStatusTab.transform.parent = parent.transform;
	}
}

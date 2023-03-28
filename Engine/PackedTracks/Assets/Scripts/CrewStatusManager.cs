/*!***************************************************************************************
\file			CrewStatusManager.cs
\project
\author			Zacharie Hong

\par			Course: GAM250
\par			Section:
\date			10/03/2023

\brief
	Handles feedback and opening of crew status panel

All content � 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

using CopiumEngine;
using System;

public class CrewStatusManager: CopiumScript
{
	public static CrewStatusManager Instance;
	public bool isCrewStatusOn = false;
    public Button CloseCrewStatusBtn;
	public Button CrewStatusBtn;
    public ResultManager resultManager;

    ButtonWrapper CloseCrewStatusBtnWrapper;
	ButtonWrapper CrewStatusBtnWrapper;

    public GameObject CrewStatusTab;

	public Image alert;

    Vector3 crewStatusTargetScale = new Vector3(5f,5f,0);

	public GameObject parent;
	
	public float transitionSpeed = 5.0f;


	// Status Screen Stuff-----
	public TrainManager trainManagerScript;
	public bool isCabinOn = false;
	public GameObject StatusScreen;
	public Text statusScreenSuppliesText;



	public Animator supplySpriteSheet;
	public int exlowThreshold; // 0 - low
	public int lowThreshold; // low - med
	public int medThreshold; // med - lot
	public int lotThreshold; // lot - inf
	int supplyState;

	//Button Wrappers
	public Button CloseStatusScreenBtn;
	ButtonWrapper CloseStatusScreenBtnWrapper;

	public GameObject cam;
	public Button CabinBtn;
	ButtonWrapper CabinBtnWrapper;
	Vector3 cabinTargetPosition = new Vector3(18.15f, 0, 0);
	//-----

	void Awake()
    {
		Instance = this;
	}

	void Start()
	{
		CloseCrewStatusBtnWrapper = new ButtonWrapper(CloseCrewStatusBtn);
		CloseCrewStatusBtnWrapper.SetImage(CloseCrewStatusBtn.GetComponent<Image>());
		CrewStatusBtnWrapper = new ButtonWrapper(CrewStatusBtn);
		CrewStatusBtnWrapper.SetImage(CrewStatusBtn.GetComponent<Image>());
		CrewStatusBtnWrapper.failureText = Messages.ErrorMainEvent;
		CabinBtnWrapper = new ButtonWrapper(CabinBtn);
		CabinBtnWrapper.SetImage(CabinBtn.GetComponent<Image>());
		CloseStatusScreenBtnWrapper = new ButtonWrapper(CloseStatusScreenBtn);
		CloseStatusScreenBtnWrapper.SetImage(CloseStatusScreenBtn.GetComponent<Image>());
		// Cabin set false
		//CrewStatusTab.SetActive(false);
		supplyState = 1;
		supplySpriteSheet.stop();
		supplySpriteSheet.setFrame(1);

	}

	void Update()
	{
		UpdateCanvas();

    }

	public void UpdateCanvas()
	{
		if (CrewStatusBtnWrapper.GetState() == ButtonState.OnRelease)
        {
			CrewMenu.Instance.SetPrepare(false);
			resultManager.Disable();
            //OpenPanel();
			OpenStatusPanel();
		}
		if (CloseStatusScreenBtnWrapper.GetState() == ButtonState.OnRelease && isCrewStatusOn)
		{
			CrewMenu.Instance.deploying = false;
			//ClosePanel(false);
			CloseStatusPanel();
        }

		if (isCrewStatusOn)
        {
            StatusScreen.transform.localScale = Vector3.Lerp(StatusScreen.transform.localScale,crewStatusTargetScale,Time.deltaTime * transitionSpeed);
			UpdateStatusScreen();
			// Check for Close Button Input
        }
        else
        {
            StatusScreen.transform.localScale = Vector3.Lerp(StatusScreen.transform.localScale,Vector3.one,Time.deltaTime * transitionSpeed);
        }

		if (trainManagerScript.accelerate == false && CabinBtnWrapper.GetState() == ButtonState.OnRelease)
		{
			GoToCabin();
		}
		if(isCabinOn)
		{
			cam.transform.localPosition = Vector3.Lerp(cam.transform.localPosition, cabinTargetPosition, Time.deltaTime * transitionSpeed);
			UpdateStatusScreen();
		}else
		{
			cam.transform.localPosition = Vector3.Lerp(cam.transform.localPosition, Vector3.zero, Time.deltaTime * transitionSpeed);

		}
		if(CloseCrewStatusBtnWrapper.GetState() == ButtonState.OnRelease && isCabinOn)
		{
			CrewMenu.Instance.deploying = false;
			ReturnToCockpit(false);
		}



	}

    public void OpenPanel()
    {
		if (isCrewStatusOn)
			return;
        alert.enabled = false;
        isCrewStatusOn = true;
		isCabinOn = true;
        CrewStatusBtn.gameObject.SetActive(false);
        CrewStatusTab.transform.parent = null;
		CrewStatusTab.SetActive(true);
    }

    public void ClosePanel(bool prepared)
	{
		CrewMenu.Instance.timeElasped = 0;
		//Prevent the crew buttons from being pressed
		if (!CrewMenu.Instance.deploying && !prepared)
		{
			CrewMenu.Instance.SetPrepare(false);
		}
		else
		{
			CrewMenu.Instance.deploying = false;
		}

		isCrewStatusOn = false;
		isCabinOn = false;
        resultManager.Enable();
        CrewStatusBtn.gameObject.SetActive(true);
		CrewStatusTab.SetActive(false);

		CrewStatusTab.transform.parent = parent.transform;
	}

	public void GoToCabin()
	{
		if(isCabinOn)
			return;
		//CrewStatusTab.SetActive(true);
		isCabinOn = true;
		CrewMenu.Instance.SetPrepare(false);

		// Switch to cabin view
		//cam.transform.localPosition = new Vector3(18.15f, 0, 0);
	}
	public void ReturnToCockpit(bool prepared)
	{
		isCabinOn = false;
		//cam.transform.localPosition = new Vector3(0,0,0);
		CrewMenu.Instance.timeElasped = 0;
		//Prevent the crew buttons from being pressed
		if (!CrewMenu.Instance.deploying && !prepared)
		{
			CrewMenu.Instance.SetPrepare(false);
		}
		else
		{
			CrewMenu.Instance.deploying = false;
		}
		//CrewStatusTab.SetActive(false);
	}
	


	public void OpenStatusPanel()
	{
		if(isCrewStatusOn)
			return;
		isCrewStatusOn = true;
		isCabinOn = false;
		alert.enabled = false;
		CrewStatusBtn.gameObject.SetActive(false);
		StatusScreen.transform.parent = null;
	}
	public void CloseStatusPanel()
	{
		isCrewStatusOn = false;
		CrewStatusBtn.gameObject.SetActive(true);
        resultManager.Enable();
        alert.enabled = true;
		StatusScreen.transform.parent = parent.transform;
		
	}

	public void UpdateStatusScreen()
	{
		// Update Supplies Text
		statusScreenSuppliesText.text = "Supplies: " + CrewMenu.Instance.supplies;

		// Bean: Temporary commented because images are not assigned
		// Update Supplies Sprite
		if(CrewMenu.Instance.supplies >= lotThreshold){
			if(supplyState != 3)
				ToggleSuppliesSprite(3);
		}else if(CrewMenu.Instance.supplies >= medThreshold){
			if(supplyState != 2)
				ToggleSuppliesSprite(2);
		}else if(CrewMenu.Instance.supplies >= lowThreshold){
			if(supplyState != 1)
				ToggleSuppliesSprite(1);
		}else{
			if(supplyState != 0)
				ToggleSuppliesSprite(0);
		}

	}
	public void ToggleSuppliesSprite(int state){
		supplyState = state;
		supplySpriteSheet.setFrame(state);
	}

	public void DisableInteractions()
	{
		ClosePanel(false);
		CloseStatusPanel();
		CrewStatusBtnWrapper.SetInteractable(false);
	}
	public void EnableInteractions()
	{
		CrewStatusBtnWrapper.SetInteractable(true);
	}

}

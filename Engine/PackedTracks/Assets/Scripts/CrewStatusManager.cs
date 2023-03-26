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
	public CrewMenu crewMenu;
	
	public float transitionSpeed = 5.0f;


	// Status Screen Stuff-----
	public TrainManager trainManagerScript;
	public bool isCabinOn = false;
	public GameObject StatusScreen;
	public Text statusScreenSuppliesText;

	public GameObject exlowSupplies;
	public GameObject lowSupplies;
	public GameObject medSupplies;
	public GameObject lotSupplies;

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

	void Start()
	{
		CloseCrewStatusBtnWrapper = new ButtonWrapper(CloseCrewStatusBtn);
		CloseCrewStatusBtnWrapper.SetImage(CloseCrewStatusBtn.GetComponent<Image>());
		CrewStatusBtnWrapper = new ButtonWrapper(CrewStatusBtn);
		CrewStatusBtnWrapper.SetImage(CrewStatusBtn.GetComponent<Image>());
		CrewStatusBtnWrapper.failureText = Messages.ErrorMainEvent;
		CabinBtnWrapper = new ButtonWrapper(CabinBtn);
		CabinBtnWrapper.SetImage(CabinBtn.GetComponent<Image>());
		// Cabin set false
		//CrewStatusTab.SetActive(false);
		supplyState = 1;

	}

	void Update()
	{
		UpdateCanvas();

    }

	public void UpdateCanvas()
	{   
		if (CrewStatusBtnWrapper.GetState() == ButtonState.OnRelease)
        {
			crewMenu.SetPrepare(false);
			resultManager.Disable();
            //OpenPanel();
			OpenStatusPanel();
        }
		if(CloseStatusScreenBtnWrapper.GetState() == ButtonState.OnRelease && isCrewStatusOn)
		{
			crewMenu.deploying = false;
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

		if(trainManagerScript.accelerate == false && CabinBtnWrapper.GetState() == ButtonState.OnRelease)
		{
			Console.WriteLine("go to cabin");
			GoToCabin();
		}
		if(isCabinOn)
		{
			cam.transform.localPosition = Vector3.Lerp(cam.transform.localPosition, cabinTargetPosition, Time.deltaTime * transitionSpeed);
		}else
		{
			cam.transform.localPosition = Vector3.Lerp(cam.transform.localPosition, Vector3.zero, Time.deltaTime * transitionSpeed);

		}
		if(CloseCrewStatusBtnWrapper.GetState() == ButtonState.OnRelease && isCabinOn)
		{
			crewMenu.deploying = false;
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
        //CrewStatusTab.transform.parent = null;
		CrewStatusTab.SetActive(true);
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
		isCabinOn = false;
        resultManager.Enable();
        CrewStatusBtn.gameObject.SetActive(true);
		CrewStatusTab.SetActive(false);

		//CrewStatusTab.transform.parent = parent.transform;
	}

	public void GoToCabin()
	{
		if(isCabinOn)
			return;
		//CrewStatusTab.SetActive(true);
		isCabinOn = true;
		crewMenu.SetPrepare(false);

		// Switch to cabin view
		//cam.transform.localPosition = new Vector3(18.15f, 0, 0);
	}
	public void ReturnToCockpit(bool prepared)
	{
		isCabinOn = false;
		//cam.transform.localPosition = new Vector3(0,0,0);
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
		//CrewStatusTab.SetActive(false);
	}
	


	public void OpenStatusPanel()
	{
		if(isCrewStatusOn)
			return;
		Console.WriteLine("status open");
		isCrewStatusOn = true;
		isCabinOn = false;
		alert.enabled = false;
		CrewStatusBtn.gameObject.SetActive(false);
		StatusScreen.transform.parent = null;
	}
	public void CloseStatusPanel()
	{
		Console.WriteLine("close status panel");
		isCrewStatusOn = false;
		CrewStatusBtn.gameObject.SetActive(true);
        resultManager.Enable();
        alert.enabled = true;
		StatusScreen.transform.parent = parent.transform;
		
	}
	public void UpdateStatusScreen()
	{
		// Update Supplies Text
		statusScreenSuppliesText.text = "Supplies: " + crewMenu.supplies;

		// Bean: Temporary commented because images are not assigned
		//if(crewMenu.supplies >= lotThreshold){
		//	if(supplyState != 3)
		//		ToggleSuppliesSprite(3);
		//}else if(crewMenu.supplies >= medThreshold){
		//	if(supplyState != 2)
		//		ToggleSuppliesSprite(2);
		//}else if(crewMenu.supplies >= lowThreshold){
		//	if(supplyState != 1)
		//		ToggleSuppliesSprite(1);
		//}else{
		//	if(supplyState != 0)
		//		ToggleSuppliesSprite(0);
		//}

	}
	public void ToggleSuppliesSprite(int state){
		if(state == 0){
			lotSupplies.SetActive(false);
			medSupplies.SetActive(false);
			lowSupplies.SetActive(false);
			exlowSupplies.SetActive(true);
			supplyState = state;
		}else if(state == 1){
			lotSupplies.SetActive(false);
			medSupplies.SetActive(false);
			lowSupplies.SetActive(true);
			exlowSupplies.SetActive(false);
			supplyState = state;

		}else if(state == 2){
			lotSupplies.SetActive(false);
			medSupplies.SetActive(true);
			lowSupplies.SetActive(false);
			exlowSupplies.SetActive(false);
			supplyState = state;

		}else if(state == 3){
			lotSupplies.SetActive(true);
			medSupplies.SetActive(false);
			lowSupplies.SetActive(false);
			exlowSupplies.SetActive(false);
			supplyState = state;

		}
	}

	public void DisableInteractions()
	{
		ClosePanel(false);
		CrewStatusBtnWrapper.SetInteractable(false);
	}
	public void EnableInteractions()
	{
		CrewStatusBtnWrapper.SetInteractable(true);
	}

}

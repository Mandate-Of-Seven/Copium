/*!***************************************************************************************
\file			CrewStatusManager.cs
\project
\author			Zacharie Hong

\par			Course: GAM250
\par			Section:
\date			10/03/2023

\brief
	Handles feedback and opening of crew status panel

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
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
	public CabinInteractions cabinInteractions;

	public Image alert;

    Vector3 crewStatusTargetScale = new Vector3(5f,5f,0);

	public GameObject parent;
	
	public float transitionSpeed = 5.0f;


	// Status Screen Stuff-----
	public TrainManager trainManagerScript;
	public bool isCabinOn = false;
	public GameObject StatusScreen;

	public Animator supplySpriteSheet;
	public int exlowThreshold; // 0 - low
	public int lowThreshold; // low - med
	public int medThreshold; // med - lot
	public int lotThreshold; // lot - inf
	int supplyState;
	public Image supplyLightDiode;
	bool supplyLow = false;
	//Button Wrappers
	public Button CloseStatusScreenBtn;
	ButtonWrapper CloseStatusScreenBtnWrapper;

	public GameObject cam;
	public Button CabinBtn;
	ButtonWrapper CabinBtnWrapper;
	Vector3 cabinTargetPosition = new Vector3(18.15f, 0, 0);
	//-----

	public GameObject crewStatusBackground;
	public GameObject crewStatusMember;

	public Vector3 crewStatusButtonScale = Vector3.one;
	public Vector3 crewStatusBackgroundScale = Vector3.one;
	public Vector3 crewStatusCloseScale = Vector3.one;
	public Vector3 crewStatusButtonTutTextPos = Vector3.zero;
	public Vector3 crewStatusBackgroundTutTextPos = Vector3.zero;
	public Vector3 crewStatusCloseTutTextPos = Vector3.zero;

	public Vector3 crewStatusMemberScale = Vector3.one;
	public Vector3 crewStatusHealthScale = Vector3.one;
	public Vector3 crewStatusMentalScale = Vector3.one;
	public Vector3 crewStatusHungerScale = Vector3.one;
	public Vector3 cabinButtonScale = Vector3.one;

	public Vector3 crewStatusMemberTutTextPos = Vector3.zero;
	public Vector3 crewStatusHealthTutTextPos = Vector3.zero;
	public Vector3 crewStatusMentalTutTextPos = Vector3.zero;
	public Vector3 crewStatusHungerTutTextPos = Vector3.zero;
	public Vector3 cabinButtonTutTextPos = Vector3.zero;
	
	public Vector3 suppliesLowScale = Vector3.one;
	public Vector3 suppliesTutTextPos = Vector3.zero;


	void Awake()
    {
		Instance = this;
	}

	void Start()
	{
		CloseCrewStatusBtnWrapper = new ButtonWrapper(CloseCrewStatusBtn);
		CloseCrewStatusBtnWrapper.SetImage(CloseCrewStatusBtn.GetComponent<Image>());
		CloseCrewStatusBtnWrapper.clickedSFX = AudioManager.Instance.autoDoorSFX;
		CrewStatusBtnWrapper = new ButtonWrapper(CrewStatusBtn);
		CrewStatusBtnWrapper.SetImage(CrewStatusBtn.GetComponent<Image>());
		CrewStatusBtnWrapper.failureText = Messages.ErrorMainEvent;
		CabinBtnWrapper = new ButtonWrapper(CabinBtn);
		CabinBtnWrapper.SetImage(CabinBtn.GetComponent<Image>());
		CabinBtnWrapper.failureText = Messages.ErrorMainEvent;
        CabinBtnWrapper.clickedSFX = AudioManager.Instance.autoDoorSFX;
		CloseStatusScreenBtnWrapper = new ButtonWrapper(CloseStatusScreenBtn);
		CloseStatusScreenBtnWrapper.SetImage(CloseStatusScreenBtn.GetComponent<Image>());
		// Cabin set false
		//CrewStatusTab.SetActive(false);
		supplyState = 1;
		supplySpriteSheet.stop();
		supplySpriteSheet.setFrame(1);

        new TutorialComponent
        (
            "CrewStatusButton",
            crewStatusButtonScale,
            CrewStatusBtn.transform,
            Messages.Tutorial.crewStatusButton,
            crewStatusButtonTutTextPos,
            delegate ()
            {
                if (CrewStatusBtnWrapper.GetState() == ButtonState.OnRelease)
                {
                    return true;
                }
                return false;
            }
        );

        new TutorialComponent
        (
			"CrewStatusDisplay",
            crewStatusBackgroundScale,
            crewStatusBackground.transform,
            Messages.Tutorial.crewStatusDisplay,
            crewStatusBackgroundTutTextPos,
            delegate ()
            {
                if (TutorialText.Instance.Done())
                    return true;
                return false;
            }
        );

		new TutorialComponent
		(
			"CrewStatusMember",
			crewStatusMemberScale,
			crewStatusMember.transform,
			Messages.Tutorial.crewStatusMember,
			crewStatusMemberTutTextPos,
			delegate ()
			{
				if (TutorialText.Instance.Done())
					return true;
				return false;
			}
		);

        new TutorialComponent
        (
			"CrewStatusHealth",
            crewStatusHealthScale,
			CrewMenu.Instance.harris.healthT.transform,
            Messages.Tutorial.crewStatusHealth,
            crewStatusHealthTutTextPos,
            delegate ()
            {
                if (TutorialText.Instance.Done())
                {
                    return true;
                }
                return false;
            }
        );

		new TutorialComponent
		(
			"CrewStatusMental",
			crewStatusMentalScale,
			CrewMenu.Instance.harris.mentalT.transform,
			Messages.Tutorial.crewStatusMental,
			crewStatusMentalTutTextPos,
			delegate ()
			{
				if (TutorialText.Instance.Done())
				{
					return true;
				}
				return false;
			}
		);

		new TutorialComponent
		(
			"CrewStatusHunger",
			crewStatusHungerScale,
			CrewMenu.Instance.harris.hungerT.transform,
			Messages.Tutorial.crewStatusHunger,
			crewStatusHungerTutTextPos,
			delegate ()
			{
				if (TutorialText.Instance.Done())
				{
					return true;
				}
				return false;
			}
		); 

		new TutorialComponent
		(
			"CrewStatusClose",
			crewStatusCloseScale,
			crewStatusBackground.transform,
			Messages.Tutorial.crewStatusEnd,
			crewStatusCloseTutTextPos,
			delegate ()
			{
				if (!isCrewStatusOn)
				{
					return true;
				}
				return false;
			}
		);

		new TutorialComponent
		(
			"CabinButton",
			cabinButtonScale,
			CabinBtn.transform,
			Messages.Tutorial.cabinButton,
			cabinButtonTutTextPos,
			delegate ()
			{
				if (CabinBtnWrapper.GetState() == ButtonState.OnRelease)
				{
					return true;
				}
				return false;
			}
		);


		new TutorialComponent
		(
			"SuppliesLow",
			suppliesLowScale,
			supplyLightDiode.transform,
            Messages.Tutorial.suppliesLow,
			suppliesTutTextPos,
			delegate ()
			{
				if (TutorialText.Instance.Done())
				{
					return true;
				}
				return false;
			}
		);
	}

	void Update()
	{
		if (!PauseMenu.Instance.isPaused)
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
        }
        else
        {
            StatusScreen.transform.localScale = Vector3.Lerp(StatusScreen.transform.localScale,Vector3.one,Time.deltaTime * transitionSpeed);
        }

		if (CabinBtnWrapper.GetState() == ButtonState.OnRelease)
		{
			GoToCabin();
		}
		if(isCabinOn)
		{
			cam.transform.localPosition = Vector3.Lerp(cam.transform.localPosition, cabinTargetPosition, Time.deltaTime * transitionSpeed);
            UpdateCabinSupply();
		}
		else
		{
			cam.transform.localPosition = Vector3.Lerp(cam.transform.localPosition, Vector3.zero, Time.deltaTime * transitionSpeed);
		}
		if(CloseCrewStatusBtnWrapper.GetState() == ButtonState.OnRelease && isCabinOn)
		{
			CrewMenu.Instance.deploying = false;
			ReturnToCockpit(false);
		}

		if(CrewMenu.Instance.supplies >= exlowThreshold && CrewMenu.Instance.supplies < lowThreshold)
		{
			if(!supplyLow)
			{
				supplyLow = true;
				supplyLightDiode.color = new Color(1.0f, 0.75f, 0.20f, 1.0f);
			}
		}
		if(CrewMenu.Instance.supplies > lowThreshold)
		{
			if(supplyLow)
			{
				supplyLow = false;
				supplyLightDiode.color = new Color(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}
	}

	public void GoToCabin()
	{
		if(isCabinOn)
			return;
		//CrewStatusTab.SetActive(true);
		isCabinOn = true;
		CrewMenu.Instance.SetPrepare(false);

		UpdateDeath();
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
		//AudioManager.Instance.autoDoorSFX.Play();

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
		StatusScreen.transform.parent = parent.transform;
	}

	public void UpdateCabinSupply()
	{
		// Update Supplies Sprite
		if(CrewMenu.Instance.supplies >= lotThreshold)
		{
			if(supplyState != 3)
				ToggleSuppliesSprite(3);
		}
		else if(CrewMenu.Instance.supplies >= medThreshold)
		{
			if(supplyState != 2)
				ToggleSuppliesSprite(2);
		}
		else if(CrewMenu.Instance.supplies >= lowThreshold)
		{
			if(supplyState != 1)
				ToggleSuppliesSprite(1);
		}
        else if (CrewMenu.Instance.supplies >= exlowThreshold)
        {
            if (supplyState != 0)
                ToggleSuppliesSprite(0);
        }
        else if (supplyState != -1)
        {
            ToggleSuppliesSprite(-1);
        }

		if (supplyState == -1)
			supplySpriteSheet.enabled = false;
		else
            supplySpriteSheet.enabled = true;
    }
	public void ToggleSuppliesSprite(int state){
		supplyState = state;
		supplySpriteSheet.setFrame(state);
	}

	public void DisableInteractions()
	{
		//ClosePanel(false);
		ReturnToCockpit(false);
		CrewStatusBtnWrapper.SetInteractable(false);
		CabinBtnWrapper.SetInteractable(false);
	}
	public void EnableInteractions()
	{
		CrewStatusBtnWrapper.SetInteractable(true);
        CabinBtnWrapper.SetInteractable(true);
    }

	public void UpdateDeath()
	{
		if(!CrewMenu.Instance.harris.person.alive)
		{
			cabinInteractions.harrisBtn.SetActive(false);
		}

		if(!CrewMenu.Instance.bronson.person.alive)
		{
			cabinInteractions.bronsonBtn.SetActive(false);

		}	

		if(!CrewMenu.Instance.chuck.person.alive)
		{
			cabinInteractions.chuckBtn.SetActive(false);

		}

		if(!CrewMenu.Instance.danton.person.alive)
		{
			cabinInteractions.dantonBtn.SetActive(false);

		}


	}

}

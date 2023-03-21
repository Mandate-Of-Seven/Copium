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

	void Start()
	{
		CloseCrewStatusBtnWrapper = new ButtonWrapper(CloseCrewStatusBtn);
		CloseCrewStatusBtnWrapper.SetImage(CloseCrewStatusBtn.GetComponent<Image>());
		CrewStatusBtnWrapper = new ButtonWrapper(CrewStatusBtn);
		CrewStatusBtnWrapper.SetImage(CrewStatusBtn.GetComponent<Image>());
		CrewStatusBtnWrapper.failureText = Messages.ErrorMainEvent;
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
            OpenPanel();
        }
		if(CloseCrewStatusBtnWrapper.GetState() == ButtonState.OnRelease)
		{
			crewMenu.deploying = false;
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

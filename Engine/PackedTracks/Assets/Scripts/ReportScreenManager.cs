/*!***************************************************************************************
\file			ReportScreenManager.cs
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			5/03/2023

\brief
	Manages the report screen, and plays the effect of opening up the the screen along
    with sound effects

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
using CopiumEngine;
using System;

public class ReportScreenManager: CopiumScript
{
    public static ReportScreenManager Instance;

    public bool isReportScreenOn = false;
	public AudioManager audioManager;
    
    public Button CloseReportBtn;
	public Button ReportScreenBtn;
    public ButtonWrapper closeBtnWrapper;
    public ButtonWrapper reportBtnWrapper;

    
    public GameObject ReportTab;
    public ResultManager resultManager;

	public Image alert;

    Vector3 reportScreenTargetScale = new Vector3(4.0f,4.0f,0);

    bool closeHover = false;
	bool openHover = false;

	public GameObject parent;
	
	public float transitionSpeed = 5.0f;

    void Awake()
    {
        Instance = this;
    }

	void Start()
	{
        closeBtnWrapper = new ButtonWrapper(CloseReportBtn);
        closeBtnWrapper.SetImage(CloseReportBtn.GetComponent<Image>());
        reportBtnWrapper = new ButtonWrapper(ReportScreenBtn);
        reportBtnWrapper.SetImage(ReportScreenBtn.GetComponent<Image>());
        //Unable to close menu when main event is up
		closeBtnWrapper.failureText = Messages.ErrorMainEvent;
	}
    void Update()
    {
        UpdateCanvas();
    }

    /**************************************************************************/
	/*!
	    \brief
		    Update the canvases related to the report screen
	*/
	/**************************************************************************/
    public void UpdateCanvas()
    {
        if (reportBtnWrapper.GetState() == ButtonState.OnRelease)
        {
            OpenPanel();
        }

        if(closeBtnWrapper.GetState() == ButtonState.OnClick)
        {
            ClosePanel();
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

	/**************************************************************************/
	/*!
	    \brief
		    Open the report screen
	*/
	/**************************************************************************/
    public void OpenPanel()
    {
        if (isReportScreenOn)
            return;

        resultManager.Disable();
        alert.enabled = false;
        isReportScreenOn = true;
        ReportScreenBtn.gameObject.SetActive(false);
        ReportTab.transform.parent = null;
    }

	/**************************************************************************/
	/*!
	    \brief
		    Close the report screen
	*/
	/**************************************************************************/
    public void ClosePanel()
    {
        if (!isReportScreenOn)
            return;

        isReportScreenOn = false;
        resultManager.Enable();
        ReportScreenBtn.gameObject.SetActive(true);
        ReportTab.transform.parent = parent.transform;
    }

    public void DisableInteractions()
	{
        OpenPanel();
		closeBtnWrapper.SetInteractable(false);
	}

    public void EnableInteractions()
    {
        closeBtnWrapper.SetInteractable(true);
    }
}

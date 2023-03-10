/*!***************************************************************************************
\file			ReportScreenManager.cs
\project
\author			Zacharie Hong
\co-author		Sean Ngo

\par			Course: GAM200
\par			Section:
\date			5/03/2023

\brief
	Manages the report screen

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
using CopiumEngine;
using System;

public class ReportScreenManager: CopiumScript
{
    public bool isReportScreenOn = false;
	public AudioManager audioManager;
    
    public Button CloseReportBtn;
	public Button ReportScreenBtn;
    public GameObject ReportTab;
    public ResultManager resultManager;

	public Image alert;

    Vector3 reportScreenTargetScale = new Vector3(4.0f,4.0f,0);

    bool closeHover = false;
	bool openHover = false;

	public GameObject parent;
	
	public float transitionSpeed = 5.0f;

	void Start()
	{

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
        if (!openHover && ReportScreenBtn.state == ButtonState.OnHover)
        {
            openHover = true;
            audioManager.hoverSFX.Play();
        }
        else if (ReportScreenBtn.state == ButtonState.OnRelease)
        {
            audioManager.clickSFX.Play();
            OpenPanel();
        }
        else if (ReportScreenBtn.state == ButtonState.None)
        {
            openHover = false;
        }

        if (!closeHover && CloseReportBtn.state == ButtonState.OnHover)
        {
            closeHover = true;
            audioManager.hoverSFX.Play();
        }
		else if(CloseReportBtn.state == ButtonState.OnRelease)
        {
            audioManager.clickSFX.Play();
            ClosePanel();
        }        
        else if (CloseReportBtn.state == ButtonState.None)
        {
            closeHover = false;
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
}

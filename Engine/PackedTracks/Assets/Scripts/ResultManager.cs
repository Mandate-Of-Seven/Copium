/*!***************************************************************************************
\file			ResultManager.cs
\project
\author			Zacharie Hong

\par			Course: GAM250
\par			Section:
\date			07/03/2023

\brief
	Manages the result screen, and plays the effect of opening up the the screen along
    with sound effects

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
using CopiumEngine;
using System;

public class ResultManager : CopiumScript
{
    public static ResultManager Instance;
    public bool isResultOn = false;

    public Button CloseResultBtn;
    public Button ResultBtn;
    public ButtonWrapper closeBtnWrapper;
    public ButtonWrapper resultBtnWrapper;

    
    public GameObject ResultTab;
    public GameObject ResultBG;

    public CrewStatusManager crewStatusManager;
    public ReportScreenManager reportScreenManager;

    Vector3 resultTargetScale = new Vector3(5.0f, 5.0f, 0);

    bool closeHover = false;
    bool openHover = false;

    public GameObject parent;

    public float transitionSpeed = 5.0f;

    public Vector3 resultsDisplayScale = Vector3.one;
    public Vector3 resultsCloseScale = Vector3.one;
    public Vector3 resultsChoicesScale = Vector3.one;
    public Vector3 resultsOutcomeScale = Vector3.one;
    public Vector3 resultsDisplayTutTextPos = Vector3.zero;
    public Vector3 resultsChoicesTutTextPos = Vector3.zero;
    public Vector3 resultsOutcomeTutTextPos = Vector3.zero;
    public Vector3 resultsEndTutTextPos = Vector3.zero;
    void Awake()
    {
        Instance = this;
    }
    void Start()
    {
        closeBtnWrapper = new ButtonWrapper(CloseResultBtn);
        closeBtnWrapper.SetImage(CloseResultBtn.GetComponent<Image>());
        resultBtnWrapper = new ButtonWrapper(ResultBtn);
        resultBtnWrapper.SetImage(ResultBtn.GetComponent<Image>());
		resultBtnWrapper.failureText = Messages.ErrorMainEvent;

        new TutorialComponent
        (
            "ResultDisplay",
            resultsDisplayScale,
            Prepare.Instance.resultText.transform,
            Messages.Tutorial.resultDisplay,
            resultsDisplayTutTextPos,
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
            "ResultChoices",
            resultsChoicesScale,
            Prepare.Instance.prepareOption2.transform,
            Messages.Tutorial.resultChoices,
            resultsChoicesTutTextPos,
            delegate ()
            {
                if (Prepare.Instance.resultText.text.Length != 0)
                {
                    return true;
                }
                return false;
            }
        );

        new TutorialComponent
        (
            "ResultOutcome",
            resultsOutcomeScale,
            ResultBG.transform,
            Messages.Tutorial.resultOutcome,
            resultsOutcomeTutTextPos,
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
            "ResultEnd",
            resultsCloseScale,
            ResultBG.transform,
            Messages.Tutorial.resultEnd,
            resultsEndTutTextPos,
            delegate ()
            {
                if (isResultOn == false)
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

    /**************************************************************************/
	/*!
	    \brief
		    Updates all canvases related to results
	*/
	/**************************************************************************/
    public void UpdateCanvas()
    {

        if (resultBtnWrapper.GetState() == ButtonState.OnClick)
        {
            OpenPanel();
        }

        if (closeBtnWrapper.GetState() == ButtonState.OnRelease)
        {
            ClosePanel();
        }


        if (isResultOn)
        {
            ResultTab.transform.localScale = Vector3.Lerp(ResultTab.transform.localScale, resultTargetScale, Time.deltaTime * transitionSpeed);
        }
        else
        {
            ResultTab.transform.localScale = Vector3.Lerp(ResultTab.transform.localScale, Vector3.one, Time.deltaTime * transitionSpeed);
        }
    }

	/**************************************************************************/
	/*!
	    \brief
		    Open the result panel 
	*/
	/**************************************************************************/
    public void OpenPanel()
    {
        if (isResultOn)
            return;

        isResultOn = true;
        ResultBtn.gameObject.SetActive(false);
        ResultTab.SetActive(true);
        ResultTab.transform.parent = null;
    }

	/**************************************************************************/
	/*!
	    \brief
		    Close results panel
	*/
	/**************************************************************************/
    public void ClosePanel()
    {
        if (!isResultOn)
            return;

        isResultOn = false;
        ResultBtn.gameObject.SetActive(true);
        ResultTab.transform.parent = parent.transform;
    }

	/**************************************************************************/
	/*!
	    \brief
		    Disable the results panel
	*/
	/**************************************************************************/
    public void Disable()
    {
        ResultTab.SetActive(false);
        ResultBtn.gameObject.SetActive(false);
    }

	/**************************************************************************/
	/*!
	    \brief
		    Enable the results panel
	*/
	/**************************************************************************/
    public void Enable()
    {
        if (reportScreenManager.isReportScreenOn || crewStatusManager.isCrewStatusOn)
            return;
        ResultTab.SetActive(true);
        ResultBtn.gameObject.SetActive(true);
    }

    public void DisableInteractions()
	{
        ClosePanel();
        resultBtnWrapper.SetInteractable(false);
    }
    public void EnableInteractions()
    {
        ClosePanel();
        resultBtnWrapper.SetInteractable(true);
    }
}

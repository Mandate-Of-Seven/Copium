using CopiumEngine;
using System;

public class ResultManager : CopiumScript
{
    public bool isResultOn = false;
    public AudioManager audioManager;

    public Button CloseResultBtn;
    public Button ResultBtn;
    public GameObject ResultTab;

    public CrewStatusManager crewStatusManager;
    public ReportScreenManager reportScreenManager;

    Vector3 resultTargetScale = new Vector3(5.8f, 5.8f, 0);

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
    public void UpdateCanvas()
    {

        if (!openHover && ResultBtn.state == ButtonState.OnHover)
        {
            openHover = true;
            audioManager.hoverSFX.Play();
        }
        else if (ResultBtn.state == ButtonState.OnRelease)
        {
            audioManager.clickSFX.Play();
            OpenPanel();
        }
        else if (ResultBtn.state == ButtonState.None)
        {
            openHover = false;
        }

        if (!closeHover && CloseResultBtn.state == ButtonState.OnHover)
        {
            closeHover = true;
            audioManager.hoverSFX.Play();
        }
        else if (CloseResultBtn.state == ButtonState.OnRelease)
        {
            audioManager.clickSFX.Play();
            ClosePanel();
        }
        else if (CloseResultBtn.state == ButtonState.None)
        {
            closeHover = false;
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

    public void OpenPanel()
    {
        if (isResultOn)
            return;

        isResultOn = true;
        ResultBtn.gameObject.SetActive(false);
        ResultTab.SetActive(true);
        ResultTab.transform.parent = null;
    }

    public void ClosePanel()
    {
        if (!isResultOn)
            return;

        isResultOn = false;
        ResultBtn.gameObject.SetActive(true);
        ResultTab.transform.parent = parent.transform;
    }

    public void Disable()
    {
        ResultTab.SetActive(false);
        ResultBtn.gameObject.SetActive(false);
    }

    public void Enable()
    {
        if (reportScreenManager.isReportScreenOn || crewStatusManager.isCrewStatusOn)
            return;
        ResultTab.SetActive(true);
        ResultBtn.gameObject.SetActive(true);
    }
}

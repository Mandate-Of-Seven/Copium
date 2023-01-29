using CopiumEngine;
using System;

public class ButtonCalls: CopiumScript
{
	public GameObject objectToToggle;
	public Text text;

    public GameObject reportTab;
    public GameObject messageTab;
    public GameObject crewTab;

    void Start()
	{

	}
	void Update()
	{
        Button button = gameObject.GetComponent<Button>();
        if (button != null && button.state == ButtonState.OnClick)
        {
            Console.WriteLine("Button clicked from C#");
        }
	}

	void UpdateText()
	{
		text.text = "Hello";
	}

	void OpenGameObject()
	{
		objectToToggle.SetActive(true);
	}

	void CloseGameObject()
	{
        objectToToggle.SetActive(false);
    }

	void OpenReportTab()
	{
        reportTab.SetActive(true);
        messageTab.SetActive(false);
        crewTab.SetActive(false);
    }

    void OpenMessageTab()
    {
		reportTab.SetActive(false);
        messageTab.SetActive(true);
        crewTab.SetActive(false);
    }

    void OpenCrewTab()
    {
        reportTab.SetActive(false);
        messageTab.SetActive(false);
        crewTab.SetActive(true);
    }
}

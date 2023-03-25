using CopiumEngine;
using System;
using System.Collections.Generic;


public class EventsManager : CopiumScript
{
	public static EventsManager Instance;
	List<Event> events = new List<Event>();
    [NonSerialized]
	public Event currentEvent;

	public Text bodyText;
	StringTypeWriterEffect bodyTypeWriter;

	public float choiceDuration = 10f;
	float choiceTimer = 0f;

	public float textInterval = 0.1f;

	public Option Option_01;
	public Option Option_02;
	public Option Option_03;
	public enum EventState
	{
		ForeShadow, Run
	};

	public EventState state = EventState.ForeShadow;

	public GameObject choiceTimerObject;

	public void Awake()
	{
		Instance = this;
	}

	public void Start()
    {
		RegisterEvents();
		UpdateCurrentEvent();
    }

	public void Update()
    {
		if (currentEvent == null)
			return;
		if (!bodyTypeWriter.Done())
		{
			if (Input.GetMouseDown(1) && ReportScreenManager.Instance.isReportScreenOn)
			{
				bodyTypeWriter.Skip();
			}
			bodyText.text = bodyTypeWriter.Write();
		}
		//If finished running
		switch (state)
		{
			case EventState.ForeShadow:
			{
				//Finished foreshadowing
				if (bodyTypeWriter.Done() && currentEvent.ForeShadow())
				{
					++state;
					//choiceTimerObject.SetActive(true);
					bodyTypeWriter = new StringTypeWriterEffect(currentEvent.body, textInterval);
					if (currentEvent.choices[0].IsValid())
					{
						Option_01.Enable();
						Option_01.mappedChoice = currentEvent.choices[0];

					}
					if (currentEvent.choices[1].IsValid())
					{
						Option_02.Enable();
						Option_02.mappedChoice = currentEvent.choices[1];

					}
					if (currentEvent.choices[2].IsValid())
					{
						Option_03.Enable();
						Option_03.mappedChoice = currentEvent.choices[2];
					}
				}
				break;
			}
			case EventState.Run:
			{
				//UpdateTimer();
				if (bodyTypeWriter.Done())
				{
					//choiceTimerObject.SetActive(false);
					++state;
					currentEvent = null;
				}
				break;
			}
		}

		if (Option_01.btnWrapper.GetState() == ButtonState.OnClick)
		{
			SelectChoice(Option_01);
		}
		else if (Option_02.btnWrapper.GetState() == ButtonState.OnClick)
		{
			SelectChoice(Option_02);
		} 
		else if (Option_03.btnWrapper.GetState() == ButtonState.OnClick)
		{
			SelectChoice(Option_03);
		}
	}

	public void SelectChoice(Option option)
    {
		bodyTypeWriter = new StringTypeWriterEffect(option.mappedChoice.resultText, textInterval);
		Option_01.Disable();
		Option_02.Disable();
		Option_03.Disable();
	}

	public void UpdateCurrentEvent()
	{
		Debug.Log("UPDATE CURRENT EVENT");
		List<Event> removableEvents = new List<Event>();
		foreach (Event e in events)
		{
			if (e.isTriggered())
			{

				Debug.Log("EVENT TRIGGERED");
				currentEvent = e;
				EventStart();
			}

			//Remove events from list
			if (e.isRemovable())
			{
				removableEvents.Add(e);
			}
		}

		foreach (Event e in removableEvents)
		{
			events.Remove(e);
		}
	}

	public void AddEvent(Event e)
	{
		events.Add(e);
	}

	public void UpdateTimer()
    {
		choiceTimer -= Time.deltaTime;
		if (choiceTimer < 0)
			choiceTimer = 0;
		float percentage = choiceTimer / choiceDuration;
		Vector3 scale = choiceTimerObject.transform.localScale;
		scale.x = percentage;
		choiceTimerObject.transform.localScale = scale;
	}

	public void EventStart()
    {
		choiceTimer = choiceDuration;
		bodyTypeWriter = new StringTypeWriterEffect(currentEvent.preempt, textInterval);
		state = EventState.ForeShadow;
	}

	void RegisterEvents()
    {
		//PUT ENDINGS HERE

		//PUT GENERIC EVENTS HERE

		//PUT SEQUENTIAL EVENTS FROM HERE ONWARDS
		AddEvent(new Event_Intro());
		AddEvent(new Event_Explosion_HarrisAlive());
		AddEvent(new Event_Explosion_HarrisDead());
		AddEvent(new Event_Intruders_ChuckAlive());
		AddEvent(new Event_Intruders_ChuckDead());
		AddEvent(new Event_Bomb_ChuckHealthy());
		AddEvent(new Event_Bomb_CrewInjured());
		AddEvent(new Event_Bomb_Default());
	}

	void ShowEnding()
	{
		Option_01.ResetOption();
		Option_02.ResetOption();
		Option_03.ResetOption();

		Option_01.Enable();
		Option_02.Enable();
		Option_03.Enable();

		Option_01.txt.text = "Restart Game";
		Option_02.txt.text = "Back to Main Menu";
		Option_03.txt.text = "Quit Game";

		if (Option_01.btn.state == ButtonState.OnClick)
		{
			SceneManager.LoadScene("Demo");
		}
		else if (Option_02.btn.state == ButtonState.OnClick)
		{
			SceneManager.LoadScene("MainMenu");
		}
		else if (Option_03.btn.state == ButtonState.OnClick)
		{
			Application.Quit();
		}
	}
}

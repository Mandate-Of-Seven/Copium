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

	public bool Done()
    {
		return bodyTypeWriter == null || bodyTypeWriter.Done();
	}

	public enum EventState
	{
		ForeShadow, Run, Choice, End
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


		StatusUpdate.Instance.gameObject.SetActive(false);
		if (Option_01.Hovered())
        {
			HoverChoice(Option_01);

		}
		else if (Option_01.btnWrapper.GetState() == ButtonState.OnClick)
		{
			SelectChoice(Option_01);
		}
		else if (Option_02.Hovered())
        {
			HoverChoice(Option_02);

		}
		else if (Option_02.btnWrapper.GetState() == ButtonState.OnClick)
		{
			SelectChoice(Option_02);
		}
		else if (Option_03.Hovered())
		{
			HoverChoice(Option_03);
		}
		else if (Option_03.btnWrapper.GetState() == ButtonState.OnClick)
		{
			SelectChoice(Option_03);
		}
		if (!bodyTypeWriter.Done())
		{
			if (Input.GetMouseDown(1) && ReportScreenManager.Instance.isReportScreenOn)
			{
				bodyTypeWriter.Skip();
			}
			bodyText.text = bodyTypeWriter.Write();
		}
		//If finished running
		if (state == EventState.ForeShadow)
		{
			//Finished foreshadowing
			if (bodyTypeWriter.Done() && currentEvent.ForeShadow())
			{
				++state;
				bodyTypeWriter = new StringTypeWriterEffect(currentEvent.body, textInterval);

			}
		}

		// If the game ends set state to be end after the foreshadow
		if (state == EventState.Run && GameManager.Instance.gameEnd)
            state = EventState.End;

        if (state == EventState.Run || state == EventState.End)
		{
			if (bodyTypeWriter.Done())
			{
				if (currentEvent.choices[0].IsValid())
				{
					Option_01.Enable();
					Option_01.AssignChoice(currentEvent.choices[0]);

					if(state == EventState.Run)
						choiceTimerObject.SetActive(true);
				}
				if (currentEvent.choices[1].IsValid())
				{
					Option_02.Enable();
					Option_02.AssignChoice(currentEvent.choices[1]);

				}
				if (currentEvent.choices[2].IsValid())
				{
					Option_03.Enable();
					Option_03.AssignChoice(currentEvent.choices[2]);
				}

				if(state == EventState.Run)
					++state;
			}
		}

		if (state == EventState.Choice && choiceTimerObject.activeSelf)
		{
			UpdateTimer();
		}
    }

	public void HoverChoice(Option option)
    {
		if (GameManager.Instance.gameEnd)
			return;

		StatusUpdate su = StatusUpdate.Instance;
		Choice choice = option.mappedChoice;
		CrewMenu.STAT_TYPES iter = CrewMenu.STAT_TYPES.HEALTH;
		su.gameObject.SetActive(true);
		while (iter <= CrewMenu.STAT_TYPES.HUNGER)
		{
			su.Harris(choice.GetStateChange("Harris", iter), iter);
			su.Bronson(choice.GetStateChange("Bronson", iter), iter);
			su.Danton(choice.GetStateChange("Danton", iter), iter);
			su.Chuck(choice.GetStateChange("Chuck", iter), iter);
			++iter;
		}

		su.Supplies(choice.GetSupplyChange());
	}
	public void SelectChoice(Option option)
    {
		bodyTypeWriter = new StringTypeWriterEffect(option.mappedChoice.resultText, textInterval);
		option.mappedChoice.ApplyEffects();
		Option_01.Disable();
		Option_02.Disable();
		Option_03.Disable();
		choiceTimerObject.SetActive(false);
		GameManager.Instance.EnableInteractions();
	}

	public void SelectDefaultChoice()
    {
		SelectChoice(Option_01);
	}

	public void UpdateCurrentEvent()
	{
		List<Event> removableEvents = new List<Event>();
		bool triggered = false;
		foreach (Event e in events)
		{
			if (e.isTriggered() && !triggered)
			{
				triggered = true;
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
		{
			choiceTimer = 0;
			SelectDefaultChoice();
		}
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
        AddEvent(new Ending_3B());
        AddEvent(new Ending_3C());
        AddEvent(new Ending_2A());
        AddEvent(new Ending_Generic());
        AddEvent(new Ending_Harris());
		AddEvent(new Ending_Bronson());
		AddEvent(new Ending_Chuck());
		AddEvent(new Ending_Danton());
		AddEvent(new Ending_AllAlive());
		AddEvent(new Ending_AllDead());

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
}

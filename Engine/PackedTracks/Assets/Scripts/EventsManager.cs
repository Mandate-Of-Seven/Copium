using CopiumEngine;
using System;
using System.Collections.Generic;

public struct Choice
{
	public string choiceText;
	public string resultText;

	public bool isValid()
	{
		return choiceText.Length > 0;
	}
}

public class EventsManager : CopiumScript
{
	public static EventsManager Instance;
	List<Event> events = new List<Event>();

	public Event currentEvent;

	public void Awake()
	{
		Instance = this;
	}

	public void UpdateCurrentEvent()
	{
		List<Event> removableEvents = new List<Event>();
		foreach (Event e in events)
		{
			if (e.isTriggered())
			{
				currentEvent = e;
			}
	
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
}

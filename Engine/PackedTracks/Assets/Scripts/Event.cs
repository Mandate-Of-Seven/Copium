using System;
using CopiumEngine;

public class Event
{
	enum State
    {
		ForeShadowing,Triggered,Running
    };

	public Event()
    {
		EventsManager.Instance.AddEvent(this);
    }

	public void ForeShadow()
    {

    }

	public void Update()
    {
		
    }

	public virtual bool isTriggered()
	{
		return false;
	}

	public virtual bool isRemovable()
	{
		return true;
	}

}
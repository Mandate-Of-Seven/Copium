using System;
using CopiumEngine;

public abstract class Event
{
	public virtual string body { get; }
	public virtual string preempt { get; }

	public Event()
    {
		EventsManager.Instance.AddEvent(this);
    }
	public virtual bool OnTrigger() { return true; }
	public virtual bool ForeShadow() { return true; }

	public virtual bool isTriggered()
	{
		return false;
	}

	public virtual bool isRemovable()
	{
		return true;
	}

}
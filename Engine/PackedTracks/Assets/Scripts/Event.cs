﻿using System;
using CopiumEngine;
using System.Collections.Generic;

public class Stats
{
	public bool setHealth = false;
	public bool setMental = false;
	public bool setHunger = false;
	public HEALTH_STATE health = 0;
	public MENTAL_STATE mental = 0;
	public HUNGER_STATE hunger = 0;
}
public class Choice
{
	Dictionary<string, Stats> crewChanges = new Dictionary<string, Stats>()
	{
		{"Harris", new Stats()},
		{"Bronson", new Stats()},
		{"Chuck", new Stats()},
		{"Danton", new Stats()}
	};

	public StatusUpdate.STATE GetStateChange(string name,CrewMenu.STAT_TYPES statType)
    {
		if (statType == CrewMenu.STAT_TYPES.HEALTH)
        {
			if (crewChanges[name].setHealth)
			{
				int change = crewChanges[name].health - CrewMenu.Instance.crewMembers[name].health;
				if (change > 0)
					return StatusUpdate.STATE.INCREASE;
				else if (change < 0)
					return StatusUpdate.STATE.DECREASE;
			}
			else
			{
				if (crewChanges[name].health < 0)
					return StatusUpdate.STATE.DECREASE;
				else if (crewChanges[name].health > 0)
					return StatusUpdate.STATE.INCREASE;
			}
		}
		else if (statType == CrewMenu.STAT_TYPES.MENTAL)
        {
			if (crewChanges[name].setMental)
			{
				int change = crewChanges[name].mental - CrewMenu.Instance.crewMembers[name].mental;
				if (change > 0)
					return StatusUpdate.STATE.INCREASE;
				else if (change < 0)
					return StatusUpdate.STATE.DECREASE;
			}
			else
			{
				if (crewChanges[name].mental < 0)
					return StatusUpdate.STATE.DECREASE;
				else if (crewChanges[name].mental > 0)
					return StatusUpdate.STATE.INCREASE;
			}
		}
		else if (statType == CrewMenu.STAT_TYPES.HUNGER)
		{
			if (crewChanges[name].setHunger)
			{
				int change = crewChanges[name].hunger - CrewMenu.Instance.crewMembers[name].hunger;
				if (change > 0)
					return StatusUpdate.STATE.INCREASE;
				else if (change < 0)
					return StatusUpdate.STATE.DECREASE;
			}
			else
			{
				if (crewChanges[name].hunger < 0)
					return StatusUpdate.STATE.DECREASE;
				else if (crewChanges[name].hunger > 0)
					return StatusUpdate.STATE.INCREASE;
			}
		}
		return StatusUpdate.STATE.NEUTRAL;
    }

	int supply = 0;
	bool setSupply = false;

	public delegate void OtherEffects();

	List<OtherEffects> otherEffects = new List<OtherEffects>();

	public string choiceText;
	public string resultText;

	public void ApplyEffects()
    {
		foreach (var effect in crewChanges)
		{
			Debug.Log("HARLO");
			Stats stat = effect.Value;
			string name = effect.Key;
			if (stat.setHealth)
				CrewMenu.Instance.SetStat(name,stat.health);
			else
				CrewMenu.Instance.ChangeHealth(name, (int)stat.health);
			if (stat.setMental)
				CrewMenu.Instance.SetStat(name, stat.mental);
			else
				CrewMenu.Instance.ChangeMental(name, (int)stat.mental);
			if (stat.setHunger)
				CrewMenu.Instance.SetStat(name, stat.hunger);
			else
				CrewMenu.Instance.ChangeHunger(name, (int)stat.hunger);
		}

		if (setSupply)
			CrewMenu.Instance.SetSupplies(supply);
		else
			CrewMenu.Instance.ChangeSupplies(supply);

		foreach (OtherEffects callback in otherEffects)
        {
			callback();
        }
    }

	public void AddOtherEffects(OtherEffects _otherEffects)
    {
		otherEffects.Add(_otherEffects);
    }

	public void ChangeSupply(int _supply)
	{
		supply = _supply;
	}

	public void SetSupply(int _supply)
	{
		setSupply = true;
		supply = _supply;
	}

	public void ChangeMemberStat(string name, int health, int mental = 0, int hunger = 0)
    {
		crewChanges[name].health = (HEALTH_STATE)health;
		crewChanges[name].mental = (MENTAL_STATE)mental;
		crewChanges[name].hunger = (HUNGER_STATE)hunger;
	}

	public void ChangeAllStat(int health, int mental = 0, int hunger = 0)
	{
		foreach (string name in crewChanges.Keys)
		{
			ChangeMemberStat(name, health, mental, hunger);
		}
    }

	public void SetMemberStats(string name, HEALTH_STATE health)
    {
		crewChanges[name].health = health;
		crewChanges[name].setHealth = true;
    }

	public void SetMemberStats(string name, MENTAL_STATE mental)
	{
		crewChanges[name].mental = mental;
		crewChanges[name].setMental = true;
	}

	public void SetMemberStats(string name, HUNGER_STATE hunger)
	{
		crewChanges[name].hunger = hunger;
		crewChanges[name].setHunger = true;
	}

	public void SetAllStats(HEALTH_STATE health)
	{
		foreach (string name in crewChanges.Keys)
		{
			SetMemberStats(name,health);
		}
	}

	public void SetAllStats(MENTAL_STATE mental)
	{
		foreach (string name in crewChanges.Keys)
		{
			SetMemberStats(name, mental);
		}
	}

	public void SetAllStats(HUNGER_STATE hunger)
	{
		foreach (string name in crewChanges.Keys)
		{
			SetMemberStats(name, hunger);
		}
	}

	public bool IsValid()
	{
		return choiceText != null;
	}
}

public abstract class Event
{
	public virtual string body { get; }
	public virtual string preempt { get; }

	public Event()
    {
		EventsManager.Instance.AddEvent(this);
    }

	public virtual bool ForeShadow() { return true; }

	public virtual bool isTriggered()
	{
		return false;
	}

	public virtual bool isRemovable()
	{
		return true;
	}

	public Choice[] choices = new Choice[3]
	{
		new Choice(),
		new Choice(),
		new Choice()
	};
}
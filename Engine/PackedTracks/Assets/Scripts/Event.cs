
/*!***************************************************************************************
\file			Event.cs
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			03/04/2023

\brief
	Base class event that is used by EventManager to play an event

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

using System;
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
    int supply = 0;
    bool setSupply = false;

    public delegate void OtherEffects();

    List<OtherEffects> otherEffects = new List<OtherEffects>();

    public string choiceText;
    public string resultText;

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
            if (!CrewMenu.Instance.crewMembers[name].alive)
                return StatusUpdate.STATE.NEUTRAL;

            if (crewChanges[name].setHealth)
			{
				if (crewChanges[name].health == HEALTH_STATE.DEAD)
					return StatusUpdate.STATE.UNKNOWN;

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
            if (!CrewMenu.Instance.crewMembers[name].alive)
                return StatusUpdate.STATE.NEUTRAL;

            if (crewChanges[name].setMental)
			{
                if (crewChanges[name].mental == MENTAL_STATE.SUICIDAL)
                    return StatusUpdate.STATE.UNKNOWN;

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
            if (!CrewMenu.Instance.crewMembers[name].alive)
                return StatusUpdate.STATE.NEUTRAL;

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

	public StatusUpdate.STATE GetSupplyChange()
	{
		if(setSupply)
		{
			if (supply == 0 && CrewMenu.Instance.supplies != 0)
				return StatusUpdate.STATE.UNKNOWN;

            int change = supply - CrewMenu.Instance.supplies;
            if (change > 0)
                return StatusUpdate.STATE.INCREASE;
            else if (change < 0)
                return StatusUpdate.STATE.DECREASE;
        }
		else
		{
            if (supply < 0)
                return StatusUpdate.STATE.DECREASE;
            else if (supply > 0)
                return StatusUpdate.STATE.INCREASE;
        }
        return StatusUpdate.STATE.NEUTRAL;
    }

	public void ApplyEffects()
    {
		foreach (var effect in crewChanges)
		{
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

    public void EndingChoices()
    {
        choices[0].choiceText = "Restart Game";
        choices[0].AddOtherEffects(delegate () { SceneManager.LoadScene("Demo"); });

        choices[1].choiceText = "Back to Main Menu";
        choices[1].AddOtherEffects(delegate () { SceneManager.LoadScene("MainMenu"); });

        choices[2].choiceText = "Quit Game";
        choices[2].AddOtherEffects(delegate () { Application.Quit(); });
    }

    public Choice[] choices = new Choice[3]
	{
		new Choice(),
		new Choice(),
		new Choice()
	};
}
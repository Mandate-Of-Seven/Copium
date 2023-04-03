/*!***************************************************************************************
\file			StatusUpdate.cs
\project
\author			Sean Ngo

\par			Course: GAM250
\par			Section:
\date			27/04/2023

\brief
	Manages the crew status menu

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
using CopiumEngine;
using System;
using System.Security.Permissions;

public class StatusUpdate: CopiumScript
{
    public static StatusUpdate Instance;
	public enum STATE
	{
		NEUTRAL,
		INCREASE,
		DECREASE,
		UNKNOWN
	}

	public Animator hHealth, hMental, hHunger;
	public Animator bHealth, bMental, cHunger;
	public Animator cHealth, cMental, bHunger;
	public Animator dHealth, dMental, dHunger;
    public Animator hIcon, bIcon, cIcon, dIcon;
    public Animator supplies;
    Color black = new Color(0.0f, 0.0f, 0.0f, 1.0f);

    public void Awake()
    {
        Instance = this;
    }

    void Update()
    {
        hIcon.stop();
        hIcon.setFrame(0);
        bIcon.stop(); 
        bIcon.setFrame(1);
        cIcon.stop(); 
        cIcon.setFrame(2);
        dIcon.stop();
        dIcon.setFrame(3);
    }

    public void ResetAll()
    {
        Harris(STATE.NEUTRAL, CrewMenu.STAT_TYPES.HEALTH);
        Harris(STATE.NEUTRAL, CrewMenu.STAT_TYPES.MENTAL);
        Harris(STATE.NEUTRAL, CrewMenu.STAT_TYPES.HUNGER);

        Bronson(STATE.NEUTRAL, CrewMenu.STAT_TYPES.HEALTH);
        Bronson(STATE.NEUTRAL, CrewMenu.STAT_TYPES.MENTAL);
        Bronson(STATE.NEUTRAL, CrewMenu.STAT_TYPES.HUNGER);

        Chuck(STATE.NEUTRAL, CrewMenu.STAT_TYPES.HEALTH);
        Chuck(STATE.NEUTRAL, CrewMenu.STAT_TYPES.MENTAL);
        Chuck(STATE.NEUTRAL, CrewMenu.STAT_TYPES.HUNGER);

        Danton(STATE.NEUTRAL, CrewMenu.STAT_TYPES.HEALTH);
        Danton(STATE.NEUTRAL, CrewMenu.STAT_TYPES.MENTAL);
        Danton(STATE.NEUTRAL, CrewMenu.STAT_TYPES.HUNGER);

        Supplies(STATE.NEUTRAL);
    }

    public void AllCrew(STATE state, CrewMenu.STAT_TYPES stat_type)
    {
        Harris(state, stat_type);
        Bronson(state, stat_type);
        Chuck(state, stat_type);
        Danton(state, stat_type);
    }

	public void Harris(STATE state, CrewMenu.STAT_TYPES stat_type)
	{
        CheckState(state, stat_type, hHealth, hMental, hHunger);
    }

    public void Bronson(STATE state, CrewMenu.STAT_TYPES stat_type)
    {
        CheckState(state, stat_type, bHealth, bMental, bHunger);
    }

    public void Chuck(STATE state, CrewMenu.STAT_TYPES stat_type)
    {
        CheckState(state, stat_type, cHealth, cMental, cHunger);
    }

    public void Danton(STATE state, CrewMenu.STAT_TYPES stat_type)
    {
        CheckState(state, stat_type, dHealth, dMental, dHunger);
    }

    public void Supplies(STATE state)
	{
        UpdateState(state, supplies);
    }

	void UpdateState(STATE state, Animator anim)
	{
        switch (state)
        {
            case STATE.NEUTRAL:
                anim.color = black;
                break;
            case STATE.INCREASE:
                anim.color = Color.green;
                break;
            case STATE.DECREASE:
                anim.color = Color.red;
                break;
            case STATE.UNKNOWN:
                anim.color = Color.white;
                break;
        }

        anim.setFrame((int)state);
    }

    void CheckState(STATE state, CrewMenu.STAT_TYPES stat_type, Animator health, Animator mental, Animator hunger)
    {
        switch (stat_type)
        {
            case CrewMenu.STAT_TYPES.HEALTH:
                UpdateState(state, health);
                break;
            case CrewMenu.STAT_TYPES.MENTAL:
                UpdateState(state, mental);
                break;
            case CrewMenu.STAT_TYPES.HUNGER:
                UpdateState(state, hunger);
                break;
        }
    }
}

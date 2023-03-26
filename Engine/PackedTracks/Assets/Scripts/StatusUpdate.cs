using CopiumEngine;
using System;
using System.Security.Permissions;

public class StatusUpdate: CopiumScript
{
	public enum STATE
	{
		NEUTRAL,
		INCREASE,
		DECREASE,
		UNKNOWN
	}

	public GameObject hHealth, hMental, hHunger;
	public GameObject bHealth, bMental, cHunger;
	public GameObject cHealth, cMental, bHunger;
	public GameObject dHealth, dMental, dHunger;
	public GameObject supplies;

    Color black = new Color(0.0f, 0.0f, 0.0f, 1.0f);
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
		Text health = hHealth.GetComponent<Text>();
		Text mental = hMental.GetComponent<Text>();
		Text hunger = hHunger.GetComponent<Text>();

        CheckState(state, stat_type, health, mental, hunger);
    }

    public void Bronson(STATE state, CrewMenu.STAT_TYPES stat_type)
    {
        Text health = bHealth.GetComponent<Text>();
        Text mental = bMental.GetComponent<Text>();
        Text hunger = bHunger.GetComponent<Text>();

        CheckState(state, stat_type, health, mental, hunger);
    }

    public void Chuck(STATE state, CrewMenu.STAT_TYPES stat_type)
    {
        Text health = cHealth.GetComponent<Text>();
        Text mental = cMental.GetComponent<Text>();
        Text hunger = cHunger.GetComponent<Text>();

        CheckState(state, stat_type, health, mental, hunger);
    }

    public void Danton(STATE state, CrewMenu.STAT_TYPES stat_type)
    {
        Text health = dHealth.GetComponent<Text>();
        Text mental = dMental.GetComponent<Text>();
        Text hunger = dHunger.GetComponent<Text>();

        CheckState(state, stat_type, health, mental, hunger);
    }

    public void Supplies(STATE state)
	{
		Text txt = supplies.GetComponent<Text>();
        UpdateText(state, txt);
    }

	void UpdateText(STATE state, Text txt)
	{
        switch (state)
        {
            case STATE.NEUTRAL:
                txt.text = "-";
                txt.color = Color.white;
                break;
            case STATE.INCREASE:
                txt.text = "^";
                txt.color = Color.green;
                break;
            case STATE.DECREASE:
                txt.text = "V";
                txt.color = Color.red;
                break;
            case STATE.UNKNOWN:
                txt.text = "?";
                txt.color = black;
                break;
        }
    }

    void CheckState(STATE state, CrewMenu.STAT_TYPES stat_type, Text health, Text mental, Text hunger)
    {
        switch (stat_type)
        {
            case CrewMenu.STAT_TYPES.HEALTH:
                UpdateText(state, health);
                break;
            case CrewMenu.STAT_TYPES.MENTAL:
                UpdateText(state, mental);
                break;
            case CrewMenu.STAT_TYPES.HUNGER:
                UpdateText(state, hunger);
                break;
        }
    }
}

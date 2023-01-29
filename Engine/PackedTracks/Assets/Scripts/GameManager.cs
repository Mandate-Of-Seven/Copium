using CopiumEngine;
using System;

public class GameManager: CopiumScript
{
	public GameObject TrainCanvas;
	public GameObject MainScreenCanvas;
	public GameObject CombatCanvas;
	public GameObject gameObj1;

	public Button Option_01;
	public Button Option_02;
	public Button Next_Event;

	public Text Header;
	public Text Body;

	public Text Option_01_Text;
	public Text Option_02_Text;

	public int EventSequence = 0;

	bool Event_01 = false;

	void Start()
	{
		TrainCanvas.SetActive(true);
		MainScreenCanvas.SetActive(false);
		CombatCanvas.SetActive(false);
		//gameObj1 = Instantiate(gameObject);
	}
	void Update()
	{
        if(Next_Event.state == ButtonState.Released)
        {
            EventSequence++;
        }

		CheckCurrentEvent();
/*		if (Input.GetKeyDown(KeyCode.E))
        {
			Destroy(gameObj1);
        }*/
    }

	void CheckCurrentEvent()
	{
        // Update eventsequence after every button click
        switch (EventSequence)
        {
            case 0:
                Introduction();
                break;
            case 1:
                Event01();
                break;
            default:
                break;
        }
    }

	void Introduction()
	{
		Header.text = "AI assistance Intro";

		Body.text = "Welcome conductor, I am AI your digital assistance for this mission. " +
            "I have a message from Captain Bob.\n\n\"Good day volunteer, \n\nYou are likely " +
            "here due to the large reward offered by the Council should you complete this mission. " +
            "Although tempting, I can't say I'd like to be in your position. The risk is just not " +
            "worth it in my opinion.\n\nYour task is to reach the settlement at the end of this railway. " +
            "Recently we have lost contact with them, and with no way to reestablish communications remotely, " +
            "we are going to send a team there to investigate. As defending the city is a priority, " +
            "we are unable to send the military. \n\nYou might have heard about the 'Crops Curse' that has been happening. " +
            "Thus, we are only able to provide 2 days worth of rations. I'm sure you will figure something out whilst traveling there. " +
            "\n\nI wish you all the best and hope to receive good news.\n\nBest regards, \nCaptain Bob Jones\"\n\nEnd of transmission. ";
    }

	// Luck in a barren wasteland
    void Event01()
    {
		Header.text = "Luck in a barren wasteland";

		Body.text = "\nReport type: Situation\n\n\nHarris spotted a abandoned town not too far off from the main track. " +
			"Crew seem to be in agreement to check it out. Otherwise, nothing out of the ordinary. Train conductor to choose course of action. \n";

		Option_01_Text.text = "Explore abandon town";
		Option_02_Text.text = "Do not explore abandoned town";

        if(Option_01.state == ButtonState.Released)
        {
            Choice01(true);
        }
        else if(Option_02.state == ButtonState.Released)
        {
            Choice01(false);
        }
    }

	void Choice01(bool choice01)
	{
        float chance;
        if (choice01)
        {
            // 80%
            chance = RNG.Range(0, 1);
            if (chance > 0.8)
            {
                Header.text = "No Luck";

                Body.text = "\nReport type: Situation\n\nThe crew had no luck in finding any useful resources. Though only the first day, " +
                    "the crew seem to have let the disappointment get to them. Moral of the crew had decrease significantly since departure.\n";
            }
            // 20%
            else
            {
                Header.text = "Found something useful";

                Body.text = "\nReport type: Situation\n\n\nThrough the thick snow and rubble the crew miraculously found some canned soup that " +
                    "still looks good to eat. This minor victory seem to boost moral of the crew slightly, everyone seem to be in a much cheery mood.";
            }
        }
        else if(!choice01)
        {
            // 50%
            chance = RNG.Range(0, 1);
            if (chance > 0.5)
            {
                Header.text = "What could have been";

                Body.text = "\nReport type: Situation\n\nThe crew seemed to be quite disappointed in your decision, all had hoped that they might " +
                    "be able to find something of use in the town. Moral of the crew seem to be hit slightly, but all still seemed to have positive outlooks on this mission.";
            }
            // 50%
            else
            {
                Header.text = "Dissatisfaction brewing";

                Body.text = "\nReport type: Situation\n\n\nChuck seem to be quite unhappy with your decision, and had a argument with Harris who tried to defend " +
                    "your course of action. Chuck began throwing punches at Harris but Luckily the confrontation was broken up by Danton before it could escalate further.";
            }
        }
    }

    void Event02()
	{

	}

	void Event03()
	{

	}
}

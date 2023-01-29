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

	public Text Header;
	public Text Body;

	public int EventSequence = 0;

	void Start()
	{
		TrainCanvas.SetActive(true);
		MainScreenCanvas.SetActive(false);
		CombatCanvas.SetActive(false);
		//gameObj1 = Instantiate(gameObject);
	}
	void Update()
	{
		CheckCurrentEvent();
/*		if (Input.GetKeyDown(KeyCode.E))
        {
			Destroy(gameObj1);
        }*/
    }

	void CheckCurrentEvent()
	{
		// Update eventsequence after every button click
	}

	// 
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

	void Event02()
	{

	}

	void Event03()
	{

	}
}

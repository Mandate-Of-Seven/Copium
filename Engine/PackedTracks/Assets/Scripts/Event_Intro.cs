using CopiumEngine;
using System;
using System.Runtime.Remoting.Messaging;

public class Event_Intro: CopiumScript
{
	public EventManager EventManager;
	void Start()
	{

	}
	void Update()
	{

	}

	public void Event()
	{
        if (EventManager.Option_01.activeSelf && EventManager.Option_02.activeSelf)
        {
            EventManager.Option_01.SetActive(false);
            EventManager.Option_02.SetActive(false);
        }

        EventManager.Header.text = "AI assistance Intro";

        EventManager.Body.text = "Welcome conductor, I am AI your digital assistance for this mission. " +
            "I have a message \nfrom Captain Bob.\n\n\"Good day volunteer, \n\nYou are likely " +
            "here due to the large reward offered by the Council should you complete\n this mission. " +
            "Although tempting, I can't say I'd like to be in your position. The \nrisk is just not " +
            "worth it in my opinion.\n\nYour task is to reach the settlement at the end of this railway. " +
            "Recently we have lost \ncontact with them, and with no way to reestablish communications remotely, " +
            "we are \ngoing to send a team there to investigate. As defending the city is a priority, " +
            "we are \nunable to send the military. \n\nYou might have heard about the 'Crops Curse' that has been happening. " +
            "Thus, we are \nonly able to provide 2 days worth of rations. I'm sure you will figure something out \nwhilst traveling there. " +
            "\n\nI wish you all the best and hope to receive good news.\n\nBest regards, \nCaptain Bob Jones\"\n\nEnd of transmission. ";

    }
}

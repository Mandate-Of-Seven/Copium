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
        EventManager.Option_01.Disable();
        EventManager.Option_02.Disable();
        EventManager.Option_03.Disable();

        EventManager.Body.text = "Welcome conductor, I am AI your digital assistance for this mission. " +
            "I have a message from Captain Bob.\n\n\"Good day volunteer, \n\nYou are likely here due " +
            "to the large reward offered by the Council should you complete this mission. Although tempting, " +
            "I can't say I'd like to be in your position. The risk is just not worth it in my opinion.\n\n" +
            "Your task is to reach the settlement at the end of this railway. Recently we have lost contact " +
            "with them, and with no way to reestablish communications remotely, we are going to send a team " +
            "there to investigate. As defending the city is a priority, we are unable to send the military. " +
            "\n\nYou might have heard about the 'Crops Curse' that has been happening. Thus, we are only " +
            "able to provide a small amount of rations. I'm sure you will figure something out whilst " +
            "traveling there. \n\nI wish you all the best and hope to receive good news.\n\nBest " +
            "regards, \nCaptain Bob Jones\"\n\nEnd of transmission.";
    }
}

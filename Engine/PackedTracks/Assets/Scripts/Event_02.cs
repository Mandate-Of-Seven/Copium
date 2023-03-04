using CopiumEngine;
using System;

public class Event_02: CopiumScript
{
    public EventManager EventManager;

    int resolutionTextNum = 0;

    void Start()
	{

	}
	void Update()
	{

	}

	public void Event(bool chuckAndBronsonAlive)
	{
        if (!chuckAndBronsonAlive)
		{
            if (EventManager.Option_03.activeSelf)
            {
                EventManager.Option_03.SetActive(false);
            }

            if (!EventManager.Option_01.activeSelf || !EventManager.Option_02.activeSelf)
            {
                EventManager.Option_01.SetActive(true);
                EventManager.Option_02.SetActive(true);
            }

            EventManager.Body.text = "From out of nowhere a group of 3 mysterious individuals boarded " +
                "and the crew cabin. They began shooting up the place demanding fuel and supplies. But " +
                "with out limited supplies, giving more of what we have can endanger everyone on board.";

			EventManager.Option_01_Text.text = "Attempt to fend off the raiders";
			EventManager.Option_02_Text.text = "Give all your supplies";

            resolutionTextNum = 1;

        }
		else
		{
            if (!EventManager.Option_01.activeSelf || !EventManager.Option_02.activeSelf || !EventManager.Option_03.activeSelf)
            {
                EventManager.Option_01.SetActive(true);
                EventManager.Option_02.SetActive(true);
                EventManager.Option_03.SetActive(true);
            }

            EventManager.Body.text = "A mysterious vehicles was seen appoarching the train, 3 mysterious " +
                "individuals proceeded to board through the crew cabin. Chuck taken by surprise and is " +
                "subsequently taken hostage by the raiders. The raiders demanded to be given either food " +
                "or fuel else Chuck will be killed.";

            EventManager.Option_01_Text.text = "Save Chuck";
            EventManager.Option_02_Text.text = "Let Chuck die and take out raiders";
            EventManager.Option_03_Text.text = "Give up food and fuel";

            resolutionTextNum = 2;

        }
    }

    public void Result(int choice)
    {
        if (resolutionTextNum == 1)
        {
            if (choice == 1)
            {
                EventManager.Body.text = "Player Narration (Record Voice, radio like sound): \n\n" +
                    "This mission was a set up! [Gun shot sounds] We where sent out to die! [More gun " +
                    "shot sounds], [Metal door open sound], No... NO, Please! [single gun shot sound] ";

                EventManager.EventSequence = -1;
            }
            else if (choice == 2)
            {
                EventManager.Body.text = "After taking the supplies, the crew were humuliated and tortured, " +
                    "they took our guns and forced the crew members to entertain them before they proceed to " +
                    "get off the train and ride off. ";

                EventManager.crewMenu.supplies = 0;
                //EventManager.crewMenu.mental1 = EventManager.crewMenu.mental2 = EventManager.crewMenu.mental3 = EventManager.crewMenu.mental4 = 5;

            }
        }
        else if (resolutionTextNum == 2)
        {
            if (choice == 1)
            {
                EventManager.Body.text = "Bronson fearing for Chuck's life, Jumped at the raiders and manage " +
                    "to successfully fend them off, but without any consequence, Bronson while protecting Chuck " +
                    "took a bullet to the stomach, rendering him critically injured";

                //EventManager.crewMenu.health2 = EventManager.crewMenu.health3 = 1;
            }
            else if (choice == 2)
            {
                EventManager.Body.text = "First they shot Chuck's knees and elbows crippling his ability to" +
                    " struggle violently, after taking down one of their guys, they retaliated and put a bullet" +
                    " right into Chuck's skull, after it was all over, the crew can only see a wrecked cabin " +
                    "with Chuck and the raiders bodies lying there lifelessly.";

                //EventManager.crewMenu.health3 = 0;
            }
            else if (choice == 3)
            {
                EventManager.Body.text = "After taking the supplies, the crew were humuliated and tortured, " +
                    "they took our guns and forced the crew members to entertain them before they proceed to " +
                    "get off the train and ride off.";

                EventManager.crewMenu.supplies -= 10;
                //EventManager.crewMenu.mental1 = EventManager.crewMenu.mental2 = EventManager.crewMenu.mental3 = EventManager.crewMenu.mental4 = 5;

            }
        }

    }
}

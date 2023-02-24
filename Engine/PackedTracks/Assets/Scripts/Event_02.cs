using CopiumEngine;
using System;

public class Event_02: CopiumScript
{
    public EventManager eventManager;

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
            if (eventManager.Option_03.activeSelf)
            {
                eventManager.Option_03.SetActive(false);
            }

            if (!eventManager.Option_01.activeSelf && !eventManager.Option_02.activeSelf)
            {
                eventManager.Option_01.SetActive(true);
                eventManager.Option_02.SetActive(true);
            }

            eventManager.Body.text = "From out of nowhere a group of 3 mysterious individuals boarded " +
                "and the crew cabin. They began shooting up the place demanding fuel and supplies. But " +
                "with out limited supplies, giving more of what we have can endanger everyone on board.";

			eventManager.Option_01_Text.text = "Attempt to fend off the raiders";
			eventManager.Option_02_Text.text = "Give all your supplies";

            resolutionTextNum = 1;

        }
		else
		{
            if (!eventManager.Option_01.activeSelf && !eventManager.Option_02.activeSelf && !eventManager.Option_03.activeSelf)
            {
                eventManager.Option_01.SetActive(true);
                eventManager.Option_02.SetActive(true);
                eventManager.Option_03.SetActive(true);
            }

            eventManager.Body.text = "A mysterious vehicles was seen appoarching the train, 3 mysterious " +
                "individuals proceeded to board through the crew cabin. Chuck taken by surprise and is " +
                "subsequently taken hostage by the raiders. The raiders demanded to be given either food " +
                "or fuel else Chuck will be killed.";

            eventManager.Option_01_Text.text = "Save Chuck";
            eventManager.Option_02_Text.text = "Let Chuck die and take out raiders";
            eventManager.Option_03_Text.text = "Give up food and fuel";

            resolutionTextNum = 2;

        }
    }

    public void Result(int choice)
    {
        if (resolutionTextNum == 1)
        {
            if (choice == 1)
            {
                eventManager.Body.text = "Player Narration (Record Voice, radio like sound): \n\n" +
                    "This mission was a set up! [Gun shot sounds] We where sent out to die! [More gun " +
                    "shot sounds], [Metal door open sound], No... NO, Please! [single gun shot sound] ";

                eventManager.EventSequence = -1;
            }
            else if (choice == 2)
            {
                eventManager.Body.text = "After taking the supplies, the crew were humuliated and tortured, " +
                    "they took our guns and forced the crew members to entertain them before they proceed to " +
                    "get off the train and ride off. ";

                eventManager.crewMenu.fuel = eventManager.crewMenu.food = 0;
                eventManager.crewMenu.mental1 = eventManager.crewMenu.mental2 = eventManager.crewMenu.mental3 = eventManager.crewMenu.mental4 = 5;

            }
        }
        else if (resolutionTextNum == 2)
        {
            if (choice == 1)
            {
                eventManager.Body.text = "Bronson fearing for Chuck's life, Jumped at the raiders and manage " +
                    "to successfully fend them off, but without any consequence, Bronson while protecting Chuck " +
                    "took a bullet to the stomach, rendering him critically injured";

                eventManager.crewMenu.health2 = eventManager.crewMenu.health3 = 1;
            }
            else if (choice == 2)
            {
                eventManager.Body.text = "First they shot Chuck's knees and elbows crippling his ability to" +
                    " struggle violently, after taking down one of their guys, they retaliated and put a bullet" +
                    " right into Chuck's skull, after it was all over, the crew can only see a wrecked cabin " +
                    "with Chuck and the raiders bodies lying there lifelessly.";

                eventManager.crewMenu.health3 = 0;
            }
            else if (choice == 3)
            {
                eventManager.Body.text = "After taking the supplies, the crew were humuliated and tortured, " +
                    "they took our guns and forced the crew members to entertain them before they proceed to " +
                    "get off the train and ride off.";

                eventManager.crewMenu.fuel -= 10;
                eventManager.crewMenu.food -= 10;
                eventManager.crewMenu.mental1 = eventManager.crewMenu.mental2 = eventManager.crewMenu.mental3 = eventManager.crewMenu.mental4 = 5;

            }
        }

    }
}

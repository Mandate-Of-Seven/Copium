using CopiumEngine;
using System;

public class Event_02: CopiumScript
{
    public EventManager EventManager;
    CrewMenu cm;

    int resolutionTextNum = 0;

    void Start()
	{
        cm = EventManager.crewMenu;
    }
	void Update()
	{

	}

	public void Event(bool requirement)
	{
        if (!requirement)
		{
            EventManager.Option_01.Enable();
            EventManager.Option_02.Enable();
            EventManager.Option_03.Disable();

            EventManager.Body.text = "From out of nowhere a group of 3 mysterious individuals boarded " +
                "and the crew cabin. They began shooting up the place demanding fuel and supplies. But " +
                "with out limited supplies, giving more of what we have can endanger everyone on board.";

            // Indicate GG GAME OVER
			EventManager.Option_01.txt.text = "Attempt to fend off the raiders";
            EventManager.Option_01.txt.color = Color.red;
            EventManager.Option_01.ShowAllIcons();

            // Indicate lost all supplies all crew mentally shaken
            EventManager.Option_02.txt.text = "Give all your supplies";
            EventManager.Option_02.ShowIcons(false, true, false, true);

            resolutionTextNum = 1;

        }
        else // Chuck and bronson alive
        {
            EventManager.Option_01.Enable();
            EventManager.Option_02.Enable();
            EventManager.Option_03.Enable();

            EventManager.Body.text = "A mysterious vehicles was seen appoarching the train, 3 mysterious " +
                "individuals proceeded to board through the crew cabin. Chuck taken by surprise and is " +
                "subsequently taken hostage by the raiders. The raiders demands you to give your supplies else " +
                "Chuck will be killed.";

            // Indicate Bronson and Chuck critically injured 
            EventManager.Option_01.txt.text = "Save Chuck";
            EventManager.Option_01.ShowIcons(true);

            // Indicate Chuck dies
            EventManager.Option_02.txt.text = "Let Chuck die and take out raiders";
            EventManager.Option_02.ShowIcons(true);


            // Indicate - 10 supplies Chuck survives(All crew members mental state shaken)
            EventManager.Option_03.txt.text = "Give up supplies";
            EventManager.Option_03.ShowIcons(false, true, false, true);

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
                    "This mission was a set up!\n\nWe where sent out to die!\n\nNo... NO, Please!";

                EventManager.EventSequence = -3;
                EventManager.OverideEvent();
            }
            else if (choice == 2)
            {
                EventManager.Body.text = "The crew was humiliated and tormented after the supplies were stolen. " +
                    "Our firearms were taken; and the crew were made to perform for the bandits before they were " +
                    "allowed to disembark and ride away.";

                cm.SetSupplies(0);
                cm.SetAllCrew(CrewMenu.STAT_TYPES.MENTAL, 9);
            }
        }
        else if (resolutionTextNum == 2)
        {
            if (choice == 1)
            {
                EventManager.Body.text = "As Chuck's safety was threatened, Bronson rushed to his side. When Bronson " +
                    "fought against the attackers, he took a gunshot to the stomach, critically injuring him. Despite " +
                    "that, Bronson effectively saved Chuck and disarm the attackers. Bronson collapsed lying in critical " +
                    "condition.";

                cm.SetCrew(CrewMenu.STAT_TYPES.HEALTH, 1, 4);
                cm.SetCrew(CrewMenu.STAT_TYPES.HEALTH, 2, 4);
            }
            else if (choice == 2)
            {
                EventManager.Body.text = "First they shot Chuck's knees and elbows crippling his ability to" +
                    " struggle violently, after taking down one of their guys, they retaliated and put a bullet" +
                    " right into Chuck's skull, after it was all over, the crew can only see a wrecked cabin " +
                    "with Chuck and the raiders bodies lying there lifelessly.";

                cm.SetCrew(CrewMenu.STAT_TYPES.ALIVE, 2, 0);
            }
            else if (choice == 3)
            {
                EventManager.Body.text = "After taking the supplies, the crew were humuliated and tortured, " +
                    "they took our guns and forced the crew members to entertain them before they proceed to " +
                    "get off the train and ride off.";


                cm.ChangeSupplies(-10);
                cm.SetAllCrew(CrewMenu.STAT_TYPES.MENTAL, 9);
            }
        }

    }
}

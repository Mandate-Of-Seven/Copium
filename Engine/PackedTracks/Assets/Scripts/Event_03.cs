using CopiumEngine;
using System;

public class Event_03: CopiumScript
{
    public EventManager EventManager;

    int resolutionTextNum = 0;

    void Start()
	{

	}
	void Update()
	{

	}

	public void Event(bool chuckAndHarrisAlive)
	{
        if (chuckAndHarrisAlive)
		{
            EventManager.Option_01.SetActive(true);
            EventManager.Option_02.SetActive(true);
            EventManager.Option_03.SetActive(true);

            EventManager.Body.text = "On a daily patrol, Chuck noticed a faint consistant bleeping sound coming from " +
                "engine room 2. After investigating, Chuck found a bomb hiding underneath the reactor for " +
                "the backup engine. From the looks of it removing it will prove quite " +
                "difficult as it might set off the explosion. Chuck gathered the "+
                "remaining crew members to decide how to proceed with the current situation.";

            EventManager.Option_01_Text.text = "Cover the bomb up to reduce damage";
			EventManager.Option_02_Text.text = "Attempt to diffuse the bomb ";
            EventManager.Option_03_Text.text = "Salvage parts from the back up engine to build a makeshift reactor and cut off engine room 2 from the train [requires Harris to be alive]";

            resolutionTextNum = 1;
        }
		else
		{
            EventManager.Option_01.SetActive(true);
            EventManager.Option_02.SetActive(true);

            EventManager.Body.text = "While attempting to keep Bronson, Chuck and Harris health stable, another explosion " +
                                     "went off in the depths of the train. Danton could not tell what was going on but the " +
                                     "train without any power came to a stop.";

            EventManager.Option_01_Text.text = "Send Danton out to find supplies";
            EventManager.Option_02_Text.text = "Save Harris";

            resolutionTextNum = 2;
        }
    }

    public void Result(int choice)
    {
        if (resolutionTextNum == 1)
        {
            if (choice == 1)
            {
                EventManager.Body.text = "The find what ever they can in attempt to try and reduce explosion and the damage caused, ... ";

                EventManager.EventSequence = -1;
            }
            else if (choice == 2)
            {
                EventManager.Body.text = "... ";

                

            }
            else if (choice == 3)
            {
                EventManager.Body.text = "... ";

                
            }
        }
        else if (resolutionTextNum == 2)
        {
            if (choice == 1)
            {
                EventManager.Body.text = "There were many days, if not weeks, with no word about Danton's whereabouts. Bronson, " +
                                         "Chuck, and Harris have all died from their wounds and the cold; I have run out of everything " +
                                         "I need to survive, including food, fuel, and water; and I'm afraid I won't be able to return " +
                                         "home alive this time. I wish you were still around, mom";

                EventManager.EventSequence = -1;
            }

        }

    }
}

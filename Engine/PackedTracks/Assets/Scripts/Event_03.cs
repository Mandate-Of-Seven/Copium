using CopiumEngine;
using System;

public class Event_03: CopiumScript
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
        if (requirement) // Chuck alive & not critically injured
        {
            EventManager.Option_01.SetActive(true);
            EventManager.Option_02.SetActive(true);
            EventManager.Option_03.SetActive(true);

            EventManager.Body.text = "While on routine patrol, Chuck heard a faint, steady bleeping emanating from engine room 2. " +
                "Chuck discovered a bomb concealed under the backup engine's reactor after looking for it there. It seems that " +
                "removing it will be challenging, since doing so might trigger the explosion. Chuck called a meeting of the " +
                "remaining crew members to discuss next steps.";

            // Indicate Chuck, Danton critically injured
            EventManager.Option_01_Text.text = "Cover the bomb up to reduce damage";

            // Indicate Chuck dies
            EventManager.Option_02_Text.text = "Attempt to diffuse the bomb, Let Chuck do it";

            // Indicate Food will be 0 for rest of the game, will take 2x as long to reach the end
            if (!cm.crew[0].alive)
                EventManager.Option_03.GetComponent<Button>().enabled = false;
            EventManager.Option_03_Text.text = "Salvage parts from the back up engine to build a makeshift reactor and cut off engine room 2 from the train [requires Harris to be alive]";

            resolutionTextNum = 1;
        }
        else // Bronson, Chuck and Harris Critially injured + Danton alive
        {
            EventManager.Option_01.SetActive(true);
            EventManager.Option_02.SetActive(false);
            EventManager.Option_03.SetActive(false);

            EventManager.Body.text = "While attempting to keep Bronson, Chuck and Harris health stable, another explosion " +
                                     "went off in the depths of the train. Danton could not tell what was going on but the " +
                                     "train without any power came to a stop.";

            // Indicate GG Game Over
            EventManager.Option_01_Text.text = "Send Danton out to find supplies";

            resolutionTextNum = 2;
        }
    }

    public void Result(int choice)
    {
        if (resolutionTextNum == 1)
        {
            if (choice == 1)
            {
                EventManager.Body.text = "As a precaution against the bomb's detonation, they use anything they " +
                    "can find to cover it. Fortunately, the engine kept running, but Chuck and Danton are in " +
                    "severe condition after being hit by bomb shrapnel.";

                cm.SetCrew(CrewMenu.STAT_TYPES.HEALTH, 2, 4);
                cm.SetCrew(CrewMenu.STAT_TYPES.HEALTH, 3, 4);
            }
            else if (choice == 2)
            {
                EventManager.Body.text = "Chuck attempted to diffuse the bomb, but only made the timer tick down faster. " +
                    "Seeing as there is no hope in diffusing the bomb, Chuck ripped the bomb out and quickly toss it out " +
                    "of the train cart. Although Chuck saved the engine, pieces of shrapnel flew right between Chuck's " +
                    "forehead killing him in that instant.";


                cm.SetCrew(CrewMenu.STAT_TYPES.ALIVE, 2, 0);
            }
            else if (choice == 3)
            {
                EventManager.Body.text = "With enough parts Harris managed to keep the train running with his makeshift reactor." +
                    " The mission is able to continue but there is no longer any supplies left on the train.";

                cm.SetSupplies(0);
                cm.storageComparment = false;
                EventManager.GameManager.distanceInterval *= 2;
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
                EventManager.OverideEvent();
            }

        }

    }
}

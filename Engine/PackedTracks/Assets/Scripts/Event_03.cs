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

	public void Event(int requirement)
	{
        if (requirement == 1) // Chuck alive & not critically injured
        {
            EventManager.Option_01.Enable();
            EventManager.Option_02.Enable();
            EventManager.Option_03.Enable();

            EventManager.Body.text = "While on routine patrol, Chuck heard a faint beeping coming from engine room 2. " +
                "He discovered a bomb concealed under the backup engine's reactor. It seems that " +
                "removing it will be challenging, as doing so may set it off. Chuck called the" +
                "remaining crew members to discuss next steps.";

            // Indicate Chuck, Danton critically injured
            EventManager.Option_01.txt.text = "Cover the bomb with sandbags to reduce damage";
            EventManager.Option_01.ShowIcons(true);

            // Indicate Chuck dies
            EventManager.Option_02.txt.text = "Attempt to defuse the bomb, let Chuck do it";
            EventManager.Option_01.ShowAllIcons();

            // Indicate supplies will be 0 for rest of the game, will take 2x as long to reach the end
            if (!cm.crew[0].alive)
                EventManager.Option_03.btn.enabled = false;
            EventManager.Option_03.txt.text = "Salvage parts from the back up engine to build a makeshift reactor and cut off engine room 2 from the train [requires Harris to be alive]";
            EventManager.Option_03.ShowIcons(false, false, false, true);

            resolutionTextNum = 1;
        }
        else if(requirement == 2) // Bronson, Chuck and Harris Critially injured + Danton alive
        {
            EventManager.Option_01.Enable();
            EventManager.Option_02.Disable();
            EventManager.Option_03.Disable();

            EventManager.Body.text = "While attempting to keep Bronson, Chuck and Harris alive, another explosion " +
                                     "went off in the depths of the train. Danton could not tell what was going on but the " +
                                     "train came grinding to a halt.";

            // Indicate GG Game Over
            EventManager.Option_01.txt.text = "Send Danton out to find supplies";
            EventManager.Option_01.txt.color = Color.red;
            EventManager.Option_01.ShowAllIcons();

            resolutionTextNum = 2;
        }
        else
        {
            EventManager.Option_01.Enable();
            EventManager.Option_02.Enable();
            EventManager.Option_03.Enable();

            EventManager.Body.text = "Out of nowhere an explosion ripped through the back up engine room, quickly engulfing " +
                "the back of the train in flames";

            // Indicate GG Game Over
            EventManager.Option_01.txt.text = "Watch the flames destroy the engine";
            EventManager.Option_01.txt.color = Color.red;
            EventManager.Option_01.ShowIcons(false, true, false, true);

            // Indicate Crew lose health
            if (cm.supplies <= 0)
                EventManager.Option_02.btn.enabled = false;
            EventManager.Option_02.txt.text = "Put out the fire and attempt to save the engine";
            EventManager.Option_02.ShowIcons(true);

            // Indicate supplies lost
            if (cm.supplies <= 0)
                EventManager.Option_03.btn.enabled = false;
            EventManager.Option_03.txt.text = "Build new simple engine to power the train";
            EventManager.Option_03.ShowIcons(false, false, false, true);

            resolutionTextNum = 3;
        }
    }

    public void Result(int choice)
    {
        if (resolutionTextNum == 1)
        {
            if (choice == 1)
            {
                EventManager.Body.text = "To minimise the damage the bomb will cause, the crew use anything they " +
                    "can find to cover it. As they worked, the beeping on the bomb stopped and it exploded. " + 
                    "As the dust settled, Chuck and Danton can be seen on the ground, it is evident that both are in critical condition." +
                    "Fortunately, the engine only suffered minor damage.";

                cm.SetCrew(CrewMenu.STAT_TYPES.HEALTH, 2, 4);
                cm.SetCrew(CrewMenu.STAT_TYPES.HEALTH, 3, 4);
            }
            else if (choice == 2)
            {
                EventManager.Body.text = "Chuck attempted to defuse the bomb, but upon cutting a red wire, the timer started ticking down faster. " +
                    "Seeing no other alternative, Chuck ripped the bomb out and quickly tossed it out " +
                    "of the train cart. Although Chuck saved the engine, Chuck was struck in the head by sharpnel " +
                    "killing him in an instant.";


                cm.SetCrew(CrewMenu.STAT_TYPES.ALIVE, 2, 0);
            }
            else if (choice == 3)
            {
                EventManager.Body.text = "Using whatever spare parts were available, Harris managed to keep the train running with his makeshift reactor." +
                    " The crew continue on their mission but there is no longer any supplies left on the train.";

                cm.SetSupplies(0);
                cm.storageComparment = false;
                EventManager.GameManager.distanceInterval *= 2;
            }
        }
        else if (resolutionTextNum == 2)
        {
            if (choice == 1)
            {
                cm.audioManager.ending3bVO.Play();
                EventManager.Body.text = "There were many days, if not weeks, with no word about Danton's whereabouts. Bronson, " +
                                         "Chuck, and Harris have all died from their wounds and the cold; I have run out of everything " +
                                         "I need to survive, including food, fuel, and water; and I'm afraid I won't be able to return " +
                                         "home alive this time. I wish you were still around, mom";

                EventManager.EventSequence = -3;
                EventManager.OverideEvent();
            }

        }
        else if(resolutionTextNum == 3)
        {
            if(choice == 1)
            {
                EventManager.Body.text = "As you and your crew watch the engine burst into flames, the train starts slowing down. " +
                    "Lights in the train start dimming out... \n\nThe train becomes silent...";

                EventManager.EventSequence = -3;
                EventManager.OverideEvent();
            }
            else if (choice == 2)
            {
                EventManager.Body.text = "The crew scramble to put out the fire before it could spread any further. It seems " +
                    "that the engine can continue running, but the crew suffered minor burns.";

                cm.ChangeAllCrew(CrewMenu.STAT_TYPES.HEALTH, -1);
            }
            else if (choice == 3)
            {
                EventManager.Body.text = "To assemble the makeshift reactor, most of the crew's remaining supplies have been exhausted." +
                    " It serves its purpose, but you notice that the train has started moving much slower.";

                cm.SetSupplies(0);
                EventManager.GameManager.distanceInterval *= 2;
            }
        }
    }
}

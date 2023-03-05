using CopiumEngine;
using System;
using System.Reflection;

public class Event_01: CopiumScript
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
        if (EventManager.Option_03.activeSelf)
        {
            EventManager.Option_03.SetActive(false);
        }

        if (!EventManager.Option_01.activeSelf || !EventManager.Option_02.activeSelf)
        {
            EventManager.Option_01.SetActive(true);
            EventManager.Option_02.SetActive(true);
        }

        // Harris is Dead
        if (requirement)
		{
            EventManager.Body.text = "In the dead of night, an explosion rocked the Main engine room, causing massive fires " +
                "to spread to the supply storage area. The raging fire that quickly engulfed the train roused the crew.";

            // Indicate crew injured and losing of supplies
			EventManager.Option_01_Text.text = "Attempt to put out the fire";

            // Indicate lost of compartment
			EventManager.Option_02_Text.text = "Detach main engine room and supply storage room from the train";


            resolutionTextNum = 1;
        }
		else
		{
            EventManager.Body.text = "Spontaneously, a huge bang could be heard coming from the direction of the engine room. " +
                "Massive fire enveloped the number one engine room. The speed with which the fire spread meant that it may " +
                "already have reached the supply storage area. Harris's cries for assistance can be heard throughout the train " +
                "as he sprints out of the engine room. The team watched in shock as Harris and their supplies caught fire.";

            // Indicate harris dies injured and save remaining supplies
            EventManager.Option_01_Text.text = "Salvage and save remaining supplies";

            // Indicate harris injure and lose all supplies
            EventManager.Option_02_Text.text = "Save Harris";

            resolutionTextNum = 2;
        }
    }

    public void Result(int choice)
    {
        if(resolutionTextNum == 1)
        {
            if (choice == 1)
            {
                EventManager.Body.text = "The undermanned crew did their best to battle the blaze, but ultimately, " +
                    "their efforts were futile. Fires were extinguished, but only at great personal risk. The whole " +
                    "crew looks like they've been burned to the point of third or fourth degree.";

                cm.SetAllCrew(CrewMenu.STAT_TYPES.HEALTH, 4);
                cm.SetSupplies(5);
            }
            else if (choice == 2)
            {
                EventManager.Body.text = "In an effort to save themselves, the crew slashed away sections of the flaming " +
                    "train as the fire raged rapidly and inexplicably. Thanks to a backup power source, the train was able " +
                    "to keep going.";

                cm.SetSupplies(0);
                cm.storageComparment = false;
            }
        }
        else if(resolutionTextNum == 2)
        {
            if (choice == 1)
            {
                EventManager.Body.text = "The crew ignored the cry of help from Harris as they put out the fire on " +
                    "the supplies. By the time the crew put out and salvage the remaining supplies all was remaining " +
                    "of Harris was his charred corpse frozen in a state where he tried to reach for help. ";

                int amount = cm.supplies - (int)RNG.Range(0, (cm.supplies / 2.0f));
                cm.SetSupplies(amount);
                cm.SetCrew(CrewMenu.STAT_TYPES.ALIVE, 0, 0);
            }
            else if (choice == 2)
            {
                EventManager.Body.text = "The crew scrambled to put out the fire on Harris, when Harris life was out of " +
                    "danger, the supplies had all burnt up.";

                cm.SetSupplies(0);
                cm.SetCrew(CrewMenu.STAT_TYPES.HEALTH, 0, 4);
            }
        }

    }
}

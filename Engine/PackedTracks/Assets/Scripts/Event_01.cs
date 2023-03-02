using CopiumEngine;
using System;
using System.Reflection;

public class Event_01: CopiumScript
{
    public EventManager eventManager;

    int resolutionTextNum = 0;

	void Start()
	{

	}
	void Update()
	{

	}

	public void Event(bool harrisDead)
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

        if (harrisDead)
		{
            eventManager.Body.text = "Deep in the middle of the night an explosion went off in Main " +
                "engine room and huge flames burst out into the food storage room. The flames engulfing " +
                "the train woke the crew up.";

			eventManager.Option_01_Text.text = "Attempt to put out the fire";
			eventManager.Option_02_Text.text = "Detach main engine room and food storage room from the train";

            resolutionTextNum = 1;
        }
		else
		{
            eventManager.Body.text = "Out of nowhere a loud explosion was heard from the engine room. " +
                "Huge flames engulfed engine room 1. The flame spread so quickly that it have reached " +
                "the food supply room. Cries of Harris can be heard throughout the train as he runs out the " +
                "engine room begging for help. The crew stood in horror as both Harris and the food supplies" +
                " was burning right infront of them.";

            eventManager.Option_01_Text.text = "Salvage and save remaining food";
            eventManager.Option_02_Text.text = "Save Harris";

            resolutionTextNum = 2;

        }
    }

    public void Result(int choice)
    {
        if(resolutionTextNum == 1)
        {
            if (choice == 1)
            {
                eventManager.Body.text = "The crew short handed fought the fire as much as they could but" +
                    " there is only so much they can do. Althought successful in putting out the fires, but" +
                    " at a cost. Seem like all the crew has either suffered third or fourth degree burn. ";

                //eventManager.crewMenu.health1 = eventManager.crewMenu.health2 = eventManager.crewMenu.health3 = eventManager.crewMenu.health4 = 1;
                eventManager.crewMenu.supplies = 5;
            }
            else if (choice == 2)
            {
                eventManager.Body.text = "The fire spread wildly and indescrimiately, fearing for their lives " +
                    "the crew cut away the burning train segments. Luckily the train is still able to continue " +
                    "due to a backup generator.";

                eventManager.crewMenu.supplies = 0;
            }
        }
        else if(resolutionTextNum == 2)
        {
            if (choice == 1)
            {
                eventManager.Body.text = "The crew ignored the cry of help from Harris as they put out the fire on " +
                    "the food supply. By the time the crew put out and salvage the remaining supplies all was remaining " +
                    "of Harris was his charred corpse frozen in a state where he tried to reach for help. ";

                //eventManager.crewMenu.health1 = 0;
            }
            else if (choice == 2)
            {
                eventManager.Body.text = "The crew scrambled to put out the fire on Harris, when Harris life was out of " +
                    "danger, the food supplies had all burnt up.";

                //eventManager.crewMenu.health1 = 1;
                eventManager.crewMenu.supplies = 0;
            }
        }

    }
}

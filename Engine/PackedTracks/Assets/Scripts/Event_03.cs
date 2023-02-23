using CopiumEngine;
using System;

public class Event_03: CopiumScript
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

        }
    }
}

/*!***************************************************************************************
\file			Event_01.cs
\project
\author			Sean Ngo (90%)
                Zacharie Hong (10%)

\par			Course: GAM200
\par			Section:
\date			10/03/2023

\brief
	This script has event 1 of the game.

All content � 2023 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/
using CopiumEngine;
using System;
using System.Reflection;

public class Event_01: CopiumScript
{
    public EventManager EventManager;
    CrewMenu cm;
    public EyesClosingEffect eyesClosingEffect;
    public CameraShakeEffect cameraShakeEffect;
    public ExplosionEffect explosionEffect;
    public TrainManager trainManager;
    public StringTypeWriterEffect bodyTypeWriter;
    bool effectTriggered = false;
    float timerElasped = 0f;
    public float shakeTime = 5f;
    public float eyesTime = 2f;

    //state 1 -> play sound, flash bang
    //state 2 -> shake
    //state 3 -> close eyes
    int state = 1;

    int resolutionTextNum = 0;

	void Start()
	{
        cm = EventManager.crewMenu;
	}
	void Update()
	{

	}

    // Event to display onto the game
    public void Event(bool requirement)
	{
        if (!effectTriggered)
        {
            bodyTypeWriter = new StringTypeWriterEffect("",Messages.Instance.PreEvent01,0.01f);
            if (trainManager.IsAccelerating())
                trainManager.FlickLever();
            effectTriggered = true;
        }
        if (state == 1)
        {
            EventManager.Body.text = bodyTypeWriter.Write();
            if (bodyTypeWriter.Done())
            {
                cameraShakeEffect.Trigger();
                explosionEffect.Trigger();
                ++state;
            }
            return;
        }
        else if (state == 2)
        {
            if (timerElasped < shakeTime)
            {
                timerElasped+=Time.deltaTime;
            }
            else
            {
                eyesClosingEffect.Trigger();
                timerElasped = 0;
                ++state;
            }
            return;
        }
        else if (state == 3)
        {
            if (timerElasped < eyesTime)
            {
                timerElasped+=Time.deltaTime;
            }
            else
            {
                cm.prepareButton.gameObject.SetActive(true);
                timerElasped = 0;
                ++state;
            }
            return;
        }

        //After effect then display

        EventManager.Option_01.Enable();
        EventManager.Option_02.Enable();
        EventManager.Option_03.Disable();

        // Harris is Dead
        if (requirement)
		{
            EventManager.Body.text = "In the dead of night, an explosion rocked the main engine room, causing massive flames " +
                "to spread to the adjacent supply storage area. The raging fire that quickly engulfed the train roused the crew.";

            // Indicate crew injured and losing of supplies
			EventManager.Option_01.txt.text = "Attempt to put out the fire";
            EventManager.Option_01.ShowIcons(true, false, false, true);

            // Indicate lost of compartment
            EventManager.Option_02.txt.text = "Detach the main engine room and supply storage room from the train";
            EventManager.Option_02.ShowIcons(false, false, false, true);

            resolutionTextNum = 1;
        }
		else
		{
            EventManager.Body.text = "Suddenly, a huge bang could be heard coming from the direction of the engine room. " +
                "Massive flames enveloped the number one engine room. The speed with which the fire spread meant that it may " +
                "already have reached the supply storage area. Harris' cries for assistance can be heard throughout the train " +
                "as he sprints out of the engine room. The crew watched in horror as both Harris and their supplies caught fire.";

            // Indicate harris dies injured and save remaining supplies
            EventManager.Option_01.txt.text = "Salvage and save remaining supplies";
            EventManager.Option_01.ShowIcons(true, false, false, true);


            // Indicate harris injure and lose all supplies
            EventManager.Option_02.txt.text = "Save Harris";
            EventManager.Option_02.ShowIcons(true, false, false, true);


            resolutionTextNum = 2;
        }
    }

    // The result after choosing a choice to display on to the game
    public void Result(int choice)
    {
        if(resolutionTextNum == 1)
        {
            if (choice == 1)
            {
                EventManager.Body.text = "The undermanned crew did their best to battle the blaze, but ultimately, " +
                    "their efforts were futile. The flames were extinguished, but only at great personal risk. It seems " +
                    "that most of the crew have suffered third to fourth degree burns.";

                cm.SetAllCrew(CrewMenu.STAT_TYPES.HEALTH, (int)Crew.HEALTH_STATE.CRITICAL);
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
                EventManager.Body.text = "The crew ignored Harris' cries for help as they focused on putting out the fire on " +
                    "the supplies. By the time the crew put out and salvaged the remaining supplies, all that remained " +
                    "of Harris was his charred corpse, still reaching out for help. ";

                int amount = cm.supplies - (int)RNG.Range(0, (cm.supplies / 2.0f));
                cm.SetSupplies(amount);
                cm.SetCrew(CrewMenu.STAT_TYPES.ALIVE, 0, 0);
            }
            else if (choice == 2)
            {
                EventManager.Body.text = "The crew scrambled to put out the fire on Harris. Though they succeeded in saving Harris, " +
                                            "most of the supplies had been reduced to ash.";

                cm.SetSupplies(0);
                cm.SetCrew(CrewMenu.STAT_TYPES.HEALTH, 0, (int)Crew.HEALTH_STATE.CRITICAL);
            }
        }

    }
}

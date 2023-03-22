/*!***************************************************************************************
\file			Event_02.cs
\project
\author			Sean Ngo (95%)
                Zacharie Hong (5%)

\par			Course: GAM200
\par			Section:
\date			10/03/2023

\brief
	This script has event 2 of the game.

All content � 2023 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/
using CopiumEngine;
using System;

public class Event_02: CopiumScript
{
    public TrainManager trainManager;
    bool effectTriggered = false;
    public CameraShakeEffect cameraShakeEffect;
    CrewMenu cm;
    StatusUpdate su;
    float timerElasped = 0f;

    int state = 0;
    int resolutionTextNum = 0;
    public float waitTime = 3f;

    void Start()
	{
    }
	void Update()
	{

	}

    // Event to display onto the game
    public void Event(bool requirement)
	{
        
        if (!effectTriggered)
        {
            EventManager.Instance.WriteToBody(Messages.Event02.preempt);
            AudioManager.Instance.gunfireSFX.Play();
            effectTriggered = true;
        }
        if (state == 0 && !EventManager.Instance.IsFinishedWriting())
        {
            ++state;
        }

        if (state == 1)
        {
            if (timerElasped < waitTime)
            {
                if (trainManager.IsAccelerating())
                    trainManager.FlickLever();
                timerElasped+=Time.deltaTime;
            }
            else
            {
                timerElasped = 0;
                cameraShakeEffect.intensity = 0.2f;
                cameraShakeEffect.totalDuration = 2f;
                cameraShakeEffect.Trigger();
                ++state;
                if (!requirement)
                {

                    EventManager.Instance.WriteToBody(Messages.Event02.ChuckDead.body);
                    // Indicate GG GAME OVER
                    EventManager.Instance.Option_01.txt.text = Messages.Event02.ChuckDead.choice01;
                    EventManager.Instance.Option_01.txt.color = Color.red;

                    // Indicate lost all supplies all crew mentally shaken
                    EventManager.Instance.Option_02.txt.text = Messages.Event02.ChuckDead.choice02;

                    resolutionTextNum = 1;

                }
                else // Chuck and bronson alive
                {
                    EventManager.Instance.WriteToBody(Messages.Event02.ChuckAlive.body);

                    // Indicate Bronson and Chuck critically injured 
                    EventManager.Instance.Option_01.txt.text = Messages.Event02.ChuckAlive.choice01;

                    // Indicate Chuck dies
                    EventManager.Instance.Option_02.txt.text = Messages.Event02.ChuckAlive.choice02;


                    // Indicate - 10 supplies Chuck survives(All crew members mental state shaken)
                    EventManager.Instance.Option_03.txt.text = Messages.Event02.ChuckAlive.choice03;

                    resolutionTextNum = 2;

                }

            }
            return;
        }

        if (EventManager.Instance.IsFinishedWriting())
        {
            if (!requirement)
            {
                EventManager.Instance.Option_01.Enable();
                EventManager.Instance.Option_02.Enable();
                EventManager.Instance.Option_03.Disable();
            }
            else
            {
                EventManager.Instance.Option_01.Enable();
                EventManager.Instance.Option_02.Enable();
                EventManager.Instance.Option_03.Enable();

            }
        }
    }

    // The result after choosing a choice to display on to the game
    public void Result(int choice)
    {
        if (resolutionTextNum == 1)
        {
            if (choice == 1)
            {
                AudioManager.Instance.ending2aVO.Play();
                EventManager.Instance.WriteToBody(Messages.Event02.ChuckDead.result01);

                EventManager.Instance.EventSequence = -3;
                EventManager.Instance.OverrideEvent();
            }
            else if (choice == 2)
            {
                EventManager.Instance.WriteToBody(Messages.Event02.ChuckDead.result02);

                cm.SetSupplies(0);
                cm.SetAllCrew(CrewMenu.STAT_TYPES.MENTAL, (int)Crew.MENTAL_STATE.IRRATIONAL);
            }
        }
        else if (resolutionTextNum == 2)
        {
            if (choice == 1)
            {
                EventManager.Instance.WriteToBody(Messages.Event02.ChuckAlive.result01);

                cm.SetCrew(CrewMenu.STAT_TYPES.HEALTH, 1, (int)Crew.HEALTH_STATE.CRITICAL);
                cm.SetCrew(CrewMenu.STAT_TYPES.HEALTH, 2, (int)Crew.HEALTH_STATE.CRITICAL);
            }
            else if (choice == 2)
            {
                EventManager.Instance.WriteToBody(Messages.Event02.ChuckAlive.result02);

                cm.SetCrew(CrewMenu.STAT_TYPES.ALIVE, 2, 0);
            }
            else if (choice == 3)
            {
                EventManager.Instance.WriteToBody(Messages.Event02.ChuckAlive.result03);

                cm.ChangeSupplies(-10);
                cm.SetAllCrew(CrewMenu.STAT_TYPES.MENTAL, (int)Crew.MENTAL_STATE.IRRATIONAL);
            }
        }

    }
}

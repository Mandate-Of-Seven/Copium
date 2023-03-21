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

public class Event01: Event
{
    public EyesClosingEffect eyesClosingEffect;
    public CameraShakeEffect cameraShakeEffect;
    public ExplosionEffect explosionEffect;
    bool effectTriggered = false;
    float timerElasped = 0f;
    public float shakeTime = 5f;
    public float eyesTime = 2f;

    //state 1 -> play sound, flash bang
    //state 2 -> shake
    //state 3 -> close eyes
    int state = 1;

    int resolutionTextNum = 0;

    // Event to display onto the game
    public void Event(bool requirement)
	{
        if (!effectTriggered)
        {
            EventManager.Instance.WriteToBody(Messages.Event01.preempt);
            effectTriggered = true;
        }
        if (state == 1)
        {
            if (EventManager.Instance.IsFinishedWriting())
            {
                explosionEffect.Trigger();
                ++state;
            }
            return;
        }
        else if (state == 2)
        {
            if (timerElasped < 1f)
            {
                timerElasped+=Time.deltaTime;
            }
            else
            {
                timerElasped = 0;
                cameraShakeEffect.Trigger();
                ++state;
            }
            return;
        }
        else if (state == 3)
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
        else if (state == 4)
        {
            if (timerElasped < eyesTime)
            {
                timerElasped+=Time.deltaTime;
            }
            else
            {
                CrewMenu.Instance.prepareButton.gameObject.SetActive(true);
                timerElasped = 0;
                ++state;

                if (requirement)
                {
                    // Indicate crew injured and losing of supplies
                    EventManager.Instance.WriteToBody(Messages.Event01.HarrisDead.body);
                    EventManager.Instance.Option_01.txt.text = Messages.Event01.HarrisDead.choice01;
                    EventManager.Instance.Option_01.ShowIcons(true, false, false, true);

                    // Indicate lost of compartment
                    EventManager.Instance.Option_02.txt.text = Messages.Event01.HarrisDead.choice02;
                    EventManager.Instance.Option_02.ShowIcons(false, false, false, true);

                    resolutionTextNum = 1;
                }
                else
                {
                    // Indicate harris dies injured and save remaining supplies
                    EventManager.Instance.WriteToBody(Messages.Event01.HarrisAlive.body);
                    EventManager.Instance.Option_01.txt.text = Messages.Event01.HarrisAlive.choice01;
                    EventManager.Instance.Option_01.ShowIcons(true, false, false, true);


                    // Indicate harris injure and lose all supplies
                    EventManager.Instance.Option_02.txt.text = Messages.Event01.HarrisAlive.choice02;
                    EventManager.Instance.Option_02.ShowIcons(true, false, false, true);

                    resolutionTextNum = 2;
                }
            }
            return;
        }

        //After effect then display
        if (EventManager.Instance.IsFinishedWriting())
        {
            EventManager.Instance.Option_01.Enable();
            EventManager.Instance.Option_02.Enable();
            EventManager.Instance.Option_03.Disable();
        }
    }

    // The result after choosing a choice to display on to the game
    public void Result(int choice)
    {
        if(resolutionTextNum == 1)
        {
            if (choice == 1)
            {
                EventManager.Instance.WriteToBody(Messages.Event01.HarrisDead.result01);

                CrewMenu.Instance.SetAllCrew(CrewMenu.STAT_TYPES.HEALTH, (int)Crew.HEALTH_STATE.CRITICAL);
                CrewMenu.Instance.SetSupplies(5);
            }
            else if (choice == 2)
            {
                EventManager.Instance.WriteToBody(Messages.Event01.HarrisDead.result02);

                CrewMenu.Instance.SetSupplies(0);
                CrewMenu.Instance.storageComparment = false;
            }
        }
        else if(resolutionTextNum == 2)
        {
            if (choice == 1)
            {
                EventManager.Instance.WriteToBody(Messages.Event01.HarrisAlive.result01);

                int amount = CrewMenu.Instance.supplies - (int)RNG.Range(0, (CrewMenu.Instance.supplies / 2.0f));
                CrewMenu.Instance.SetSupplies(amount);
                CrewMenu.Instance.SetCrew(CrewMenu.STAT_TYPES.ALIVE, 0, 0);
            }
            else if (choice == 2)
            {
                EventManager.Instance.WriteToBody(Messages.Event01.HarrisAlive.result02);
                CrewMenu.Instance.SetSupplies(0);
                CrewMenu.Instance.SetCrew(CrewMenu.STAT_TYPES.HEALTH, 0, (int)Crew.HEALTH_STATE.CRITICAL);
            }
        }

    }
}

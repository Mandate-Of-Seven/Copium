using CopiumEngine;
using System;

public class Event_Explosion : Event
{
    float timerElasped = 0f;
    float shakeTime = 5f;
    float eyesTime = 2f;
    int state = 0;

    // Event to display onto the game


    public override string preempt
    {
        get
        {
            return Messages.Event_Explosion.preempt;
        }
    }

    public override bool isRemovable()
    {
        return GameManager.Instance.eventSequence >= 1;
    }

    public override bool ForeShadow()
    {
        switch (state)
        {
            case 0:
            {
                ExplosionEffect.Instance.Trigger();
                ++state;
                return false;
            }
            case 1:
            {
                if (timerElasped < 1f)
                {
                    timerElasped += Time.deltaTime;
                }
                else
                {
                    timerElasped = 0;
                    CameraShakeEffect.Instance.Trigger();
                    ++state;
                }
                return false;
            } 
            case 2:
            {
                if (timerElasped < shakeTime)
                {
                    timerElasped += Time.deltaTime;
                }
                else
                {
                    EyesClosingEffect.Instance.Trigger();
                    timerElasped = 0;
                    ++state;
                }
                return false;
            }
            case 3:
            {
                if (timerElasped < eyesTime)
                {
                    timerElasped += Time.deltaTime;
                }
                else
                {
                    timerElasped = 0;
                    ++state;
                }
                return false;
            }
        }
        return true;
    }

    // The result after choosing a choice to display on to the game
    public void Result(int choice)
    {
        if (resolutionTextNum == 1)
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
        else if (resolutionTextNum == 2)
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

public class Event_Explosion_HarrisDead : Event_Explosion
{
    public override string body
    {
        get { return Messages.Event_Explosion.HarrisDead.body; }
    }

    public override bool isTriggered()
    {
        return !CrewMenu.Instance.crewMembers["Harris"].alive && GameManager.Instance.eventSequence == 1;
    }
}

public class Event_Explosion_HarrisAlive : Event_Explosion
{
    public override string body
    {
        get { return Messages.Event_Explosion.HarrisAlive.body; }
    }

    public override bool isTriggered()
    {
        return CrewMenu.Instance.crewMembers["Harris"].alive && GameManager.Instance.eventSequence == 1;
    }
}


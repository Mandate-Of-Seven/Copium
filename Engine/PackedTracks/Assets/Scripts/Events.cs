/*!***************************************************************************************
\file			Events.cs
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			03/04/2023

\brief
	All events inherited off Event
All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/


using CopiumEngine;
using System;

public class Event_Intro : Event
{

    public override string body { get { return Messages.Event_Intro.body; } }

    public override bool isRemovable(){return true; }
    public override bool isTriggered() { return true; }

}

#region EVENT01
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
}

public class Event_Explosion_HarrisDead : Event_Explosion
{
    public Event_Explosion_HarrisDead()
    {
        #region Choice1
        choices[0].choiceText = Messages.Event_Explosion.HarrisDead.choice01;
        choices[0].resultText = Messages.Event_Explosion.HarrisDead.result01;
        choices[0].SetAllStats(HEALTH_STATE.CRITICAL);
        choices[0].SetSupply(5);
        #endregion


        #region Choice2
        choices[1].choiceText = Messages.Event_Explosion.HarrisDead.choice02;
        choices[1].resultText = Messages.Event_Explosion.HarrisDead.result02;
        choices[1].SetSupply(0);
        choices[1].AddOtherEffects(CrewMenu.Instance.DetachStorageCompartment);
        #endregion
    }
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
    public Event_Explosion_HarrisAlive()
    {
        #region Choice1
        choices[0].choiceText = Messages.Event_Explosion.HarrisAlive.choice01;
        choices[0].resultText = Messages.Event_Explosion.HarrisAlive.result01;
        int amount = CrewMenu.Instance.supplies/2;
        choices[0].SetSupply(amount);
        choices[0].SetMemberStats("Harris",HEALTH_STATE.DEAD);
        #endregion

        #region Choice2
        choices[1].choiceText = Messages.Event_Explosion.HarrisAlive.choice02;
        choices[1].resultText = Messages.Event_Explosion.HarrisAlive.result02;
        choices[1].SetAllStats(HEALTH_STATE.CRITICAL);
        choices[1].SetSupply(0);
        #endregion
    }
    public override string body
    {
        get { return Messages.Event_Explosion.HarrisAlive.body; }
    }

    public override bool isTriggered()
    {
        return CrewMenu.Instance.crewMembers["Harris"].alive && GameManager.Instance.eventSequence == 1;
    }
}

#endregion

#region EVENT02
public class Event_Intruders : Event
{
    float timerElasped = 0f;
    int state = 0;
    public float waitTime = 9f;

    // Event to display onto the game

    public override string preempt
    {
        get
        {
            return Messages.Event_Intruders.preempt;
        }
    }

    public override bool isRemovable()
    {
        return GameManager.Instance.eventSequence >= 2;
    }

    public override bool ForeShadow()
    {
        switch (state)
        {
            case 0:
            {
                AudioManager.Instance.gunfireSFX.Play();
                ++state;
                return false;
            }
            case 1:
            {
                if (timerElasped < waitTime)
                {
                    timerElasped += Time.deltaTime;
                }
                else
                {
                    timerElasped = 0;
                    CameraShakeEffect.Instance.intensity = 0.2f;
                    CameraShakeEffect.Instance.totalDuration = 2f;
                    CameraShakeEffect.Instance.Trigger();
                    ++state;
                }
                return false;
            }
        }
        return true;
    }
}

public class Event_Intruders_ChuckDead : Event_Intruders
{
    public override string body
    {
        get { return Messages.Event_Intruders.ChuckDead.body; }
    }

    public override bool isTriggered()
    {
        return !CrewMenu.Instance.crewMembers["Chuck"].alive && GameManager.Instance.eventSequence == 2;
    }

    public Event_Intruders_ChuckDead()
    {
        #region Choice1
        choices[0].choiceText = Messages.Event_Intruders.ChuckDead.choice01;
        //choices[0].resultText = Messages.Event_Intruders.ChuckDead.result01;
        choices[0].AddOtherEffects(delegate()
        {
            GameManager.Instance.eventSequence = -2;
        });
        
        #endregion

        #region Choice2
        choices[1].choiceText = Messages.Event_Intruders.ChuckDead.choice02;
        choices[1].resultText = Messages.Event_Intruders.ChuckDead.result02;
        choices[1].SetAllStats(MENTAL_STATE.IRRATIONAL);
        choices[1].SetSupply(0);
        #endregion
    }
}

public class Event_Intruders_ChuckAlive : Event_Intruders
{
    public Event_Intruders_ChuckAlive()
    {
        #region Choice1
        choices[0].choiceText = Messages.Event_Intruders.ChuckAlive.choice01;
        choices[0].resultText = Messages.Event_Intruders.ChuckAlive.result01;
        choices[0].SetMemberStats("Chuck", HEALTH_STATE.CRITICAL);
        choices[0].SetMemberStats("Bronson", HEALTH_STATE.CRITICAL);
        #endregion

        #region Choice2
        choices[1].choiceText = Messages.Event_Intruders.ChuckAlive.choice02;
        choices[1].resultText = Messages.Event_Intruders.ChuckAlive.result02;
        choices[1].SetMemberStats("Chuck", HEALTH_STATE.DEAD);
        #endregion

        #region Choice3
        choices[2].choiceText = Messages.Event_Intruders.ChuckAlive.choice03;
        choices[2].resultText = Messages.Event_Intruders.ChuckAlive.result03;
        choices[2].SetAllStats(MENTAL_STATE.IRRATIONAL);
        choices[2].SetSupply(0);
        #endregion
    }
    public override string body
    {
        get { return Messages.Event_Intruders.ChuckAlive.body; }
    }

    public override bool isTriggered()
    {
        return CrewMenu.Instance.crewMembers["Chuck"].alive && GameManager.Instance.eventSequence == 2;
    }
}

#endregion

#region EVENT03
public class Event_Bomb: Event
{
    float timerElasped = 0f;
    int state = 0;

    public override string preempt
    {
        get { return Messages.Event_Bomb.preempt; }
    }

    public override bool isRemovable()
    {
        return GameManager.Instance.eventSequence >= 3;
    }

    public override bool ForeShadow()
    {
        switch (state)
        {
            case 0:
                {
                    AudioManager.Instance.explosionForEvent3SFX.Play();
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
                        CameraShakeEffect.Instance.intensity = 0.5f;
                        CameraShakeEffect.Instance.totalDuration = 4f;
                        CameraShakeEffect.Instance.Trigger();
                        ++state;
                    }
                    return false;
                }
        }
        return true;
    }
}

public class Event_Bomb_ChuckHealthy : Event_Bomb
{
    public override bool ForeShadow()
    {
        if (!CrewMenu.Instance.harris.person.alive)
            EventsManager.Instance.Option_03.btnWrapper.SetInteractable(false);
        return base.ForeShadow();
    }
    public Event_Bomb_ChuckHealthy()
    {
        #region Choice1
        choices[0].choiceText = Messages.Event_Bomb.ChuckHealthy_3A.choice01;
        choices[0].resultText = Messages.Event_Bomb.ChuckHealthy_3A.result01;
        choices[0].SetMemberStats("Harris", HEALTH_STATE.CRITICAL);
        choices[0].SetMemberStats("Danton", HEALTH_STATE.CRITICAL);
        #endregion

        #region Choice2
        choices[1].choiceText = Messages.Event_Bomb.ChuckHealthy_3A.choice02;
        choices[1].resultText = Messages.Event_Bomb.ChuckHealthy_3A.result02;
        choices[1].SetMemberStats("Chuck", HEALTH_STATE.DEAD);
        #endregion

        #region Choice3
        choices[2].choiceText = Messages.Event_Bomb.ChuckHealthy_3A.choice03;
        choices[2].resultText = Messages.Event_Bomb.ChuckHealthy_3A.result03;
        choices[2].SetSupply(0);
        choices[2].AddOtherEffects(CrewMenu.Instance.DetachStorageCompartment);
        choices[2].AddOtherEffects(delegate(){ GameManager.Instance.distanceInterval *= 2;});
        #endregion

    }
    public override string body
    {
        get { return Messages.Event_Bomb.ChuckHealthy_3A.body; }
    }

    public override bool isTriggered()
    {
        return 
            CrewMenu.Instance.crewMembers["Chuck"].health >= HEALTH_STATE.INJURED && 
            GameManager.Instance.eventSequence == 3;
    }
}
public class Event_Bomb_CrewInjured : Event_Bomb
{
    public Event_Bomb_CrewInjured()
    {
        #region Choice1
        choices[0].choiceText = Messages.Event_Bomb.CrewInjured_3B.choice01;
        //choices[0].resultText = Messages.Event_Bomb.CrewInjured_3B.result01;
        choices[0].AddOtherEffects(delegate () 
        { 
            GameManager.Instance.eventSequence = -1;
        });
        #endregion

    }
    public override string body
    {
        get { return Messages.Event_Bomb.CrewInjured_3B.body; }
    }

    public override bool isTriggered()
    {
        return 
            CrewMenu.Instance.crewMembers["Chuck"].health == HEALTH_STATE.CRITICAL &&
            CrewMenu.Instance.crewMembers["Harris"].health == HEALTH_STATE.CRITICAL &&
            CrewMenu.Instance.crewMembers["Bronson"].health == HEALTH_STATE.CRITICAL &&
            CrewMenu.Instance.crewMembers["Danton"].alive &&
            GameManager.Instance.eventSequence == 3;
    }
}
public class Event_Bomb_Default : Event_Bomb
{
    public override bool ForeShadow()
    {
        if (CrewMenu.Instance.supplies == 0)
            EventsManager.Instance.Option_03.btnWrapper.SetInteractable(false);
        return base.ForeShadow();
    }
    public override string body
    {
        get { return Messages.Event_Bomb.Default_3C.body; }
    }

    public override bool isTriggered()
    {
        return GameManager.Instance.eventSequence == 3;
    }

    public Event_Bomb_Default()
    {
        #region Choice1
        choices[0].choiceText = Messages.Event_Bomb.Default_3C.choice01;
        //choices[0].resultText = Messages.Event_Bomb.Default_3C.result01;
        choices[0].AddOtherEffects(delegate ()
        {
            GameManager.Instance.eventSequence = -5;
        });
        #endregion
        #region Choice2
        choices[1].choiceText = Messages.Event_Bomb.Default_3C.choice02;
        choices[1].resultText = Messages.Event_Bomb.Default_3C.result02;
        choices[1].ChangeAllStat(-1);
        #endregion
        #region Choice3
        choices[2].choiceText = Messages.Event_Bomb.Default_3C.choice03;
        choices[2].resultText = Messages.Event_Bomb.Default_3C.result03;
        choices[2].SetSupply(0);
        choices[2].AddOtherEffects(delegate ()
        {
            GameManager.Instance.distanceInterval *= 2;
        });
        #endregion
    }
}

#endregion

#region EVENT_ENDING

public class Event_Endings : Event
{
    protected float duration = 4.0f;
    public override bool isRemovable()
    {
        return false;
    }
}

public class Ending_3B : Event_Endings
{
    public override string preempt
    {
        get
        {
            return Messages.Event_Bomb.CrewInjured_3B.result01;
        }
    }
    public override bool ForeShadow()
    {
        Fade.Instance.duration = duration;
        if (!Fade.Instance.shouldFade)
            Fade.Instance.Start();

        if (!Fade.Instance.FadeEnded())
            return false;

        SceneManager.LoadScene("Ending");
        return true;
    }
    public override bool isTriggered()
    {
        return GameManager.Instance.eventSequence == -1;
    }
}

public class Ending_3C : Event_Endings
{
    public override string preempt
    {
        get
        {
            return Messages.Event_Bomb.Default_3C.result01;
        }
    }
    public override bool ForeShadow()
    {
        Fade.Instance.duration = duration;
        if (!Fade.Instance.shouldFade)
            Fade.Instance.Start();

        if (!Fade.Instance.FadeEnded())
            return false;

        SceneManager.LoadScene("Ending");
        return true;
    }

    public override bool isTriggered()
    {
        return GameManager.Instance.eventSequence == -5;
    }
}

public class Ending_2A : Event_Endings
{
    public override string preempt
    {
        get
        {
            return Messages.Event_Intruders.ChuckDead.result01;
        }
    }
    
    public override bool ForeShadow()
    {
        Fade.Instance.duration = duration;
        if (!Fade.Instance.shouldFade)
            Fade.Instance.Start();

        if (!Fade.Instance.FadeEnded())
            return false;

        SceneManager.LoadScene("Ending");
        return true;
    }
    public override bool isTriggered()
    {
        return GameManager.Instance.eventSequence == -2;
    }
}

public class Ending_Generic : Event_Endings
{
    public override bool ForeShadow()
    {
        AudioManager.Instance.endingGenericVO.Play();
        return true;
    }
    public Ending_Generic()
    {
        EndingChoices();
    }

    public override string body
    {
        get { return Messages.Event_Ending.Generic.body; }
    }


    public override bool isTriggered()
    {
        return GameManager.Instance.eventSequence == -6;
    }
}

public class Ending_Harris : Event_Endings
{
    public override bool ForeShadow()
    {
        AudioManager.Instance.endingHarrisVO.Play();
        return true;
    }
    public Ending_Harris()
    {
        EndingChoices();
    }
    public override string body
    {
        get { return Messages.Event_Ending.Harris.body; }
    }

    public override bool isTriggered()
    {
        return CrewMenu.Instance.crewMembers["Harris"].alive && !CrewMenu.Instance.crewMembers["Bronson"].alive
            && !CrewMenu.Instance.crewMembers["Danton"].alive && !CrewMenu.Instance.crewMembers["Chuck"].alive
            && GameManager.Instance.eventSequence == -4;
    }
}

public class Ending_Bronson : Event_Endings
{
    public override bool ForeShadow()
    {
        AudioManager.Instance.endingBronsonVO.Play();
        return true;
    }
    public Ending_Bronson()
    {
        EndingChoices();
    }
    public override string body
    {
        get { return Messages.Event_Ending.Bronson.body; }
    }

    public override bool isTriggered()
    {
        return !CrewMenu.Instance.crewMembers["Harris"].alive && CrewMenu.Instance.crewMembers["Bronson"].alive
            && !CrewMenu.Instance.crewMembers["Danton"].alive && !CrewMenu.Instance.crewMembers["Chuck"].alive
            && GameManager.Instance.eventSequence == -4;
    }
}

public class Ending_Chuck : Event_Endings
{
    public override bool ForeShadow()
    {
        AudioManager.Instance.endingChuckVO.Play();
        return true;
    }
    public Ending_Chuck()
    {
        EndingChoices();
    }
    public override string body
    {
        get { return Messages.Event_Ending.Chuck.body; }
    }

    public override bool isTriggered()
    {
        return !CrewMenu.Instance.crewMembers["Harris"].alive && !CrewMenu.Instance.crewMembers["Bronson"].alive 
            && !CrewMenu.Instance.crewMembers["Danton"].alive && CrewMenu.Instance.crewMembers["Chuck"].alive 
            && GameManager.Instance.eventSequence == -4;
    }
}

public class Ending_Danton : Event_Endings
{
    public override bool ForeShadow()
    {
        AudioManager.Instance.endingDantonVO.Play();
        return true;
    }
    public Ending_Danton()
    {
        EndingChoices();
    }
    public override string body
    {
        get { return Messages.Event_Ending.Danton.body; }
    }

    public override bool isTriggered()
    {
        return !CrewMenu.Instance.crewMembers["Harris"].alive && !CrewMenu.Instance.crewMembers["Bronson"].alive
            && CrewMenu.Instance.crewMembers["Danton"].alive && !CrewMenu.Instance.crewMembers["Chuck"].alive
            && GameManager.Instance.eventSequence == -4;
    }
}

public class Ending_AllAlive : Event_Endings
{
    public override bool ForeShadow()
    {
        AudioManager.Instance.endingAllAliveVO.Play();
        return true;
    }
    public Ending_AllAlive()
    {
        EndingChoices();
    }
    public override string body
    {
        get { return Messages.Event_Ending.AllAlive.body; }
    }

    public override bool isTriggered()
    {
        return CrewMenu.Instance.CheckAllCrewAlive() && GameManager.Instance.eventSequence == -4;
    }
}

public class Ending_AllDead : Event_Endings
{
    public override string preempt
    {
        get
        {
            return "All crew members have died...";
        }
    }
    public override bool ForeShadow()
    {
        Fade.Instance.duration = duration;
        if (!Fade.Instance.shouldFade)
            Fade.Instance.Start();

        if (!Fade.Instance.FadeEnded())
            return false;

        SceneManager.LoadScene("Ending");
        return true;
    }
    public override bool isTriggered()
    {
        return CrewMenu.Instance.CheckAllCrewDead();
    }
}

#endregion
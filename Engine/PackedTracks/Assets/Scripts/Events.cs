using CopiumEngine;
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
    public float waitTime = 3f;

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
        choices[0].resultText = Messages.Event_Intruders.ChuckDead.result01;
        choices[0].AddOtherEffects(delegate()
        {
            AudioManager.Instance.ending2aVO.Play();
            GameManager.Instance.eventSequence = -3;
            EventsManager.Instance.UpdateCurrentEvent();
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
        choices[2].ChangeSupply(-10);
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

    public override string preempt
    {
        get { return Messages.Event_Bomb.preempt; }
    }
    public override bool isRemovable()
    {
        return GameManager.Instance.eventSequence >= 3;
    }
}

public class Event_Bomb_ChuckHealthy : Event_Bomb
{
    public Event_Bomb_ChuckHealthy()
    {
        #region Choice1
        choices[0].choiceText = Messages.Event_Bomb.ChuckHealthy.choice01;
        choices[0].resultText = Messages.Event_Bomb.ChuckHealthy.result01;
        choices[0].SetMemberStats("Harris", HEALTH_STATE.CRITICAL);
        choices[0].SetMemberStats("Danton", HEALTH_STATE.CRITICAL);
        #endregion

        #region Choice2
        choices[1].choiceText = Messages.Event_Bomb.ChuckHealthy.choice02;
        choices[1].resultText = Messages.Event_Bomb.ChuckHealthy.result02;
        choices[1].SetMemberStats("Chuck", HEALTH_STATE.DEAD);
        #endregion

        #region Choice3
        choices[2].choiceText = Messages.Event_Bomb.ChuckHealthy.choice03;
        choices[2].resultText = Messages.Event_Bomb.ChuckHealthy.result03;
        choices[2].SetSupply(0);
        choices[2].AddOtherEffects(CrewMenu.Instance.DetachStorageCompartment);
        choices[2].AddOtherEffects(delegate(){ GameManager.Instance.distanceInterval *= 2;});
        #endregion

    }
    public override string body
    {
        get { return Messages.Event_Bomb.ChuckHealthy.body; }
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
        choices[0].choiceText = Messages.Event_Bomb.ChuckHealthy.choice01;
        choices[0].resultText = Messages.Event_Bomb.CrewInjured.result01;
        choices[0].AddOtherEffects(delegate () 
        { 
            AudioManager.Instance.ending3bVO.Play();
            GameManager.Instance.eventSequence = -3;
            EventsManager.Instance.UpdateCurrentEvent();
        });
        #endregion

    }
    public override string body
    {
        get { return Messages.Event_Bomb.CrewInjured.body; }
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
    public override string body
    {
        get { return Messages.Event_Bomb.Default.body; }
    }

    public override bool isTriggered()
    {
        return GameManager.Instance.eventSequence == 3;
    }

    public Event_Bomb_Default()
    {
        #region Choice1
        choices[0].choiceText = Messages.Event_Bomb.Default.choice01;
        choices[0].resultText = Messages.Event_Bomb.Default.result01;
        choices[0].AddOtherEffects(delegate ()
        {
            GameManager.Instance.eventSequence = -3;
        });
        #endregion
        #region Choice2
        choices[1].choiceText = Messages.Event_Bomb.Default.choice02;
        choices[1].resultText = Messages.Event_Bomb.Default.result02;
        choices[1].ChangeAllStat(-1);
        #endregion
        #region Choice3
        choices[2].choiceText = Messages.Event_Bomb.Default.choice03;
        choices[2].resultText = Messages.Event_Bomb.Default.result03;
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
    float duration = 4.0f;

    public override bool ForeShadow()
    {
        Fade.Instance.duration = duration;
        Fade.Instance.Start();

        if (!Fade.Instance.FadeEnded())
            return false;

        return true;
    }
}

#endregion
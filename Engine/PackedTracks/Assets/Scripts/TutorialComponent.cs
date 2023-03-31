using CopiumEngine;
using System;

public class TutorialComponent
{
    public string text;
    public Vector3 scaleTrans;
    public Vector3 posTrans;

    public delegate bool TriggerCondition();

    public TriggerCondition triggerCondition;

    public TutorialComponent(
        string name,
        Vector3 _scaleTrans,
        Vector3 _posTrans,
        TriggerCondition _triggerCondition )
    {
        TutorialManager.Instance.tutorials.Add(name, this);
        scaleTrans = _scaleTrans; posTrans = _posTrans;
        triggerCondition = _triggerCondition;
    }

    public bool isFinished()
    {
        return triggerCondition();
    }
}

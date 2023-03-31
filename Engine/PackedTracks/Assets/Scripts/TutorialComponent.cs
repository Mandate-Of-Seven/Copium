using CopiumEngine;
using System;

public class TutorialComponent
{
    public string text;
    public Vector3 scale;
    public Vector3 textPos;
    public Transform posTrans;

    public delegate bool TriggerCondition();

    [NonSerialized]
    public TriggerCondition triggerCondition;

    public TutorialComponent(
        string name,
        Vector3 _scale,
        Transform _posTrans,
        string _text,
        Vector3 _textPos,
        TriggerCondition _triggerCondition
        )
    {
        text = _text;
        Console.WriteLine("RAN");
        TutorialManager.Instance.tutorials.Add(name, this);
        scale = _scale; posTrans = _posTrans;
        textPos = _textPos;
        triggerCondition = _triggerCondition;
    }

    public bool isFinished()
    {
       
        return triggerCondition();
    }
}

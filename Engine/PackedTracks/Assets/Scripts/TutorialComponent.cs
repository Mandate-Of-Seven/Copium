/*!***************************************************************************************
\file			TutorialComponent.cs
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			03/04/2023

\brief
	Tutorial object that is used by tutorial manager for the interactive tutorial

All content ? 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

using CopiumEngine;
using System;

public class TutorialComponent
{
    public string text;
    public Vector3 scale;
    public Vector3 textPos;
    public Transform posTrans;
    public bool transparentBlock;
    public delegate bool TriggerCondition();

    [NonSerialized]
    public TriggerCondition triggerCondition;

    //Constructor to initialize where and how big the interaction box should be
    //Also assigns a callback and text to run for the tutorial
    public TutorialComponent(
        string name,
        Vector3 _scale,
        Transform _posTrans,
        string _text,
        Vector3 _textPos,
        TriggerCondition _triggerCondition,
        bool _transparentBlock = false
        )
    {
        text = _text;
        TutorialManager.Instance.tutorials.Add(name, this);
        scale = _scale; posTrans = _posTrans;
        textPos = _textPos;
        triggerCondition = _triggerCondition;
        transparentBlock = _transparentBlock;
    }

    //Checks whether the tutorial is finished, determined by the assigned callback
    public bool isFinished()
    {
       
        return triggerCondition();
    }
}

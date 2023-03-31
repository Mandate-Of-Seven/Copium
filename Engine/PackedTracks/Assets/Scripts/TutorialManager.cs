using CopiumEngine;
using System;
using System.Collections.Generic;

public class TutorialManager: CopiumScript
{
	public static TutorialManager Instance;

    [NonSerialized]
	public Dictionary<string,TutorialComponent> tutorials = new Dictionary<string,TutorialComponent>();

	public InteractionMask mask;

	public float transitionTime = 1f;
	Vector3 originalPosition;
	Vector3 originalScale;
	float transitionTimer = 0f;

	public string[] sequence =
	new string[]
	{
		"ReportButton",
		"ReportDisplay",
		"ReportClose",
        /*"CrewStatusButton",
        "CrewStatusDisplay",
        "CrewStatusClose",*/
    };

	int sequenceIndex = 0;

	void Awake()
    {
		Instance = this;
    }

	void Start()
	{
	}

	void Update()
	{
		Debug.Log(sequence[sequenceIndex]);
		TutorialComponent tutorial = tutorials[sequence[sequenceIndex]];
		if (tutorial.isFinished())
        {
			originalPosition = tutorial.posTrans;
			originalScale = tutorial.scaleTrans;
			transitionTimer = 0f;
			++sequenceIndex;
        }
		else if (transitionTimer < transitionTime)
        {
			transitionTimer += Time.deltaTime;
			mask.scale = Vector3.Lerp(originalScale,tutorial.scaleTrans,transitionTimer/transitionTime);
			mask.transform.position = Vector3.Lerp(originalPosition, tutorial.posTrans, transitionTimer / transitionTime);
		}
	}
}

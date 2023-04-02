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
	Vector3 originalPos;
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
		TutorialText.Instance.SetContent(Messages.Tutorial.reportStart);
	}

	void Update()
	{
        if (PauseMenu.Instance.isPaused)
            return;

        TutorialComponent tutorial = tutorials[sequence[sequenceIndex]];
		TutorialText.Instance.transform.position = tutorial.textPos;
		if (tutorial.isFinished())
        {
			originalPos = tutorial.posTrans.worldPosition;
			originalScale = tutorial.scale;
			transitionTimer = 0f;
			++sequenceIndex;
			TutorialText.Instance.SetContent(tutorials[sequence[sequenceIndex]].text);
		}
		else if (transitionTimer < transitionTime)
        {
			transitionTimer += Time.deltaTime;
			mask.scale = Vector3.Lerp(originalScale,tutorial.scale,transitionTimer/transitionTime);
			mask.transform.position = Vector3.Lerp(originalPos, tutorial.posTrans.worldPosition, transitionTimer / transitionTime);
		}
		else
		{
			mask.scale = tutorial.scale;
			mask.transform.position = tutorial.posTrans.worldPosition;
		}
	}
}

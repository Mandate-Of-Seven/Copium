/*!***************************************************************************************
\file			TutorialText.cs
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			03/04/2023

\brief
	Text for tutorial

All content ? 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/


using CopiumEngine;
using System;

public class TutorialText: CopiumScript
{
	public static TutorialText Instance;
	StringTypeWriterEffect writer;
	Text text;
	public float interval = 0.1f;

	void Awake()
    {
		Instance = this;
    }

	void Start()
	{
		text = GetComponent<Text>();

	}

	//Starts a typewriter effect for new content
	public void SetContent(string content)
    {
		writer = new StringTypeWriterEffect(content, interval);
    }

	//Checks if the typewriter is done writing
	public bool Done()
    {
		return writer == null || writer.Done();
	}

	void Update()
	{
		if (PauseMenu.Instance.isPaused)
			return;

		if (!Done())
		{
			if (Input.GetKeyDown(KeyCode.Space))
            {
				writer.Skip();
			}
			text.text = writer.Write();
        }
	}
}

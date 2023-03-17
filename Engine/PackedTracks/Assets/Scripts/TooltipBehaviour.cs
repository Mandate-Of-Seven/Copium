/*!***************************************************************************************
\file			TrainManager.cs
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			10/03/2023

\brief
	Manages a text component and can print a set amount of messages to it through 
	appending a new line

All content � 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

using CopiumEngine;
using System;

public class TooltipBehaviour: CopiumScript
{
	public static TooltipBehaviour Instance;

	public Text content;
	public int maxLines = 5;
	public int lines = 0;

	void Awake()
	{
		Instance = this;
	}

	void Start()
	{
	}

	public void AddText(string newContent)
	{
		if (newContent.Length == 0)
			return;
		if (content.text.Length != 0)
			content.text += "\n";
		if (lines == maxLines)
		{
			string buffer = content.text;
			buffer = content.text.Substring(buffer.IndexOf("\n")+1);
			content.text = buffer;
			--lines;
		}
		++lines;
		Color color = content.color;
		color.a = 1;
		content.color = color;
		content.text += newContent;
	}

	void Update()
	{
		Color color = content.color;
		color.a = Mathf.Lerp(color.a, 0, Time.deltaTime);
		if (color.a <= 0.2f)
		{
			lines = 0;
			content.text = "";
		}
		content.color = color;
	}
}

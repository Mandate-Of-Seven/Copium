/*!***************************************************************************************
\file			StringTypeWriterEffect.cs
\project
\author			Zacharie Hong

\par			Course: GAM250
\par			Section:
\date			03/04/2023

\brief
	Typewriter effect for strings to display strings one character at a time interval

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

using CopiumEngine;
using System;
using System.Reflection;
using System.Text;

public class StringTypeWriterEffect
{
	float intervalTimeElasped = 0f;
	float interval = 0.01f;
	int charIndex = 0;
	StringBuilder initial = new StringBuilder();
	StringBuilder target = new StringBuilder();
	bool wait = false;
	bool done = false;
	float waitTime = 20f;
	float doneTime = 0f;
	float doneDuration = 0f;

	public StringTypeWriterEffect(string _target, float _interval)
	{
		target.Append(_target);
		interval = _interval;
		charIndex = 0;
		doneDuration = 50f * interval;
	}

	//Speeds up the speed of the text
	public void Skip()
	{
		interval /= 5f;
	}

	//Checks if the text is done displaying
	public bool Done()
	{
		return done;
	}

	//Adds a character to the string
	public string Write()
	{
		if (Done())
			return target.ToString();

		if (charIndex == target.Length)
		{
			if (doneTime < doneDuration)
            {
				doneTime += Time.deltaTime;
			}
			else
			{
				done = true;
			}
			return target.ToString();
		}
		float dt = Time.deltaTime;
		char ch = target[charIndex];
		if (charIndex > 0 && target[charIndex-1] == '.' && !wait)
		{
			wait = true;
		}
		
		if (wait)
		{
			if (intervalTimeElasped <= interval * waitTime)
			{
				intervalTimeElasped += dt;
				return initial.ToString();
			}
			else
			{
				intervalTimeElasped = interval;
				wait = false;
			}
		}
		while (intervalTimeElasped >= interval && charIndex < target.Length)
		{
			Update();
			intervalTimeElasped -= interval;
		}
		intervalTimeElasped += dt;
		return initial.ToString();
	}

	
	void Update()
	{
		initial.Append(target[charIndex]);
		++charIndex;
	}
}

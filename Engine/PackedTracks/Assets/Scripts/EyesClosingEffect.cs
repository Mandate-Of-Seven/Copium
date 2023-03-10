/*!***************************************************************************************
\file			EyeClosingEffect.cs
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			10/3/2023

\brief
	This plays an eye closing effect with two black images converge to simulate two
	eyelids

All content ? 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

using CopiumEngine;
using System;

public class EyesClosingEffect: CopiumScript
{

	public float originalPosY = 8.0f;
	public float targetPosY = 4.5f;
	public GameObject upperLid;
	public GameObject lowerLid;
	float timeAccumulated = 0f;
	public float timeTaken = 1f;
	bool playing = false;
	bool open = false;

	public void Trigger()
	{
		playing = true;
	}

	public void Reset()
	{
		playing = false;
		open = false;
		timeAccumulated = 0;
	}

	void Start()
	{

	}
	void Update()
	{
		if (!playing)
			return;
		Vector3 upperPos = upperLid.transform.position;
		Vector3 lowerPos = lowerLid.transform.position;
		float dt = Time.deltaTime;
		if (timeAccumulated > timeTaken)
		{
			if (open)
			{
				Reset();
				return;
			}
			open = !open;
			timeAccumulated = 0;
		}
		float newY = 0f;
		if (!open)
		{
			newY = Mathf.Lerp(upperPos.y,targetPosY,timeAccumulated/timeTaken);
		}
		else
		{
			Console.WriteLine("ORIGINAL POSITION BRO");
			newY = Mathf.Lerp(upperPos.y,originalPosY,timeAccumulated/timeTaken);
		}
		upperPos.y = newY;
		lowerPos.y = -newY;
		upperLid.transform.position = upperPos;
		lowerLid.transform.position = lowerPos;
		timeAccumulated+=dt;
	}
}

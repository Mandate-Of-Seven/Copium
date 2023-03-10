/*!***************************************************************************************
\file			BlinkingEffect.cs
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			10/3/2023

\brief
	This file contains the code for a flashing effect on any image

All content ? 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/


using CopiumEngine;
using System;

public class BlinkingEffect: CopiumScript
{
	public Image image;
	public float interval = 1.0f;
	private float timer = 0.0f;
	bool fadeIn = false;
	void Start()
	{

	}
	void Update()
	{
		float dt = Time.deltaTime;
		if (timer >= interval)
		{
			fadeIn = !fadeIn;
			timer = 0;
		}
		Color color = image.color;

		if (fadeIn)
		{
			color.a = Mathf.Lerp(0,1,timer);
		}
		else
		{
			color.a = Mathf.Lerp(1,0,timer);
		}
		image.color = color;
		timer += dt;
	}
}

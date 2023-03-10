/*!***************************************************************************************
\file			ExplosionEffect.cs
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			10/3/2023

\brief
	This plays an explosion sound effect and plays a flashbang screen fade

All content ? 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

using CopiumEngine;
using System;

public class ExplosionEffect: CopiumScript
{

	public float duration = 10f;
	public float waitpoint = 8f;
	public float breakpoint = 7f;
	public AudioManager audioManager;
	public Image flashImage;
	float timeElasped = 0f;
	bool playing = false;

	public void Trigger()
	{
		playing = true;
		audioManager.explosionSFX.Play();
		flashImage.enabled = true;
	}

	void Reset()
	{
		playing = false;
		timeElasped = 0;
		flashImage.color = new Color(1,1,1,0);
		flashImage.enabled = false;
	}
	
	void Start()
	{

	}

	void Update()
	{
		if (!playing)
			return;
		float alpha = 1f; 

		if (timeElasped < breakpoint)
		{
			alpha = Mathf.Lerp(0,1,timeElasped/breakpoint);
		}
		else if (timeElasped < waitpoint)
		{

		}
		else if (timeElasped < duration)
		{
			alpha = Mathf.Lerp(1,0,(timeElasped-waitpoint)/(duration - waitpoint));
		}
		else
		{
			Reset();
			return;
		}
		flashImage.color = new Color(1,1,1,alpha);
		timeElasped+= Time.deltaTime;
	}
}

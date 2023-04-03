/*!***************************************************************************************
\file			SplashScreen.cs
\project
\author			Sean Ngo

\par			Course: GAM250
\par			Section:
\date			27/04/2023

\brief
	Manages the splash screen in the game

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
using CopiumEngine;
using System;
using System.Runtime.CompilerServices;

public class SplashScreen: CopiumScript
{
	public float wait = 1.0f;
	private float timer = 0.0f;

	public AudioSource snow;
	public Fade digipenLogo;
	public Fade fmodLogo;
	public Fade engineLogo;
	public Fade skipper;

	private bool startWait = false;
	private bool startLogo = false;
	private bool skip = false;
	private bool endLogo = false;

	void Start()
	{
		digipenLogo.fadeInAndOut = true;
		snow.Play();
	}
	void Update()
	{
		if(skipper.hasFaded && !startWait)
		{
            skipper.duration = 0.25f;
            skipper.hasFaded = false;
			startWait = true;
		}

		if(startWait && !startLogo)
		{
			startLogo = true;
            digipenLogo.Start(true);
		}

        if (Input.GetMouseDown(0))
		{
            skipper.Start();
			skip = true;
		}

        if (engineLogo.hasFaded && !endLogo)
		{
			skipper.Start();
			endLogo = true;
		}

		if(skipper.shouldFade && endLogo)
			snow.volume = 1 - skipper.fadeVal;

		if ((skipper.FadeEnded() && endLogo) || (skipper.FadeEnded() && skip))
			SceneManager.LoadScene("MainMenu");

		if (digipenLogo.hasFaded)
		{
			digipenLogo.hasFaded = false;
            fmodLogo.fadeInAndOut = true;
            fmodLogo.Start(true);
        }

		if(fmodLogo.hasFaded)
		{
            fmodLogo.hasFaded = false;
            engineLogo.fadeInAndOut = true;
            engineLogo.Start(true);
        }
	}
}

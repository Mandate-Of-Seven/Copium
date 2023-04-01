using CopiumEngine;
using System;

public class SplashScreen: CopiumScript
{
	public float wait = 1.0f;
	private float timer = 0.0f;

	public Fade digipenLogo;
	public Fade engineLogo;
	public Fade skipper;

	private bool startWait = false;
	private bool skip = false;

	void Start()
	{
		digipenLogo.fadeInAndOut = true;
		digipenLogo.Start(true);
	}
	void Update()
	{
		if(Input.GetMouseDown(0))
		{
            skipper.Start();
			skip = true;
		}

        if (engineLogo.hasFaded || (skipper.FadeEnded() && skip))
            SceneManager.LoadScene("MainMenu");

        if (digipenLogo.hasFaded)
		{
			digipenLogo.hasFaded = false;
            startWait = true;
		}

        if (startWait)
		{
			if (timer > wait)
			{
				timer = 0.0f;
				startWait = false;
                engineLogo.fadeInAndOut = true;
                engineLogo.Start(true);
			}

			timer += Time.deltaTime;
		}
	}
}

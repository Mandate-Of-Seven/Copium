using CopiumEngine;
using System;
using System.Globalization;

public class Subtitles: CopiumScript
{
	public Text subtitles;

	public AudioManager audioManager;
	public Fade fade;

    private float wait = 0.5f;
	private float fadeVal = 0.0f;
	private float timer = 0.0f;
	private float duration = 1.0f;

    public bool shouldFade = false;
    public bool fadeIn = false;
    private bool hasFaded = false;
    private bool fadeOutAndIn = false;
    private bool fadeInAndOut = false;
    private bool dontWait = false;

	void Start()
	{
		subtitles = GetComponent<Text>();
	}
	void Update()
	{
		if(Input.GetKeyDown(KeyCode.Space))
		{
			StartFade();
		}
		else if (Input.GetKeyDown(KeyCode.F))
        {
            StartFade(false);
        }
        else if (Input.GetKeyDown(KeyCode.D))
        {
            FadeInAndOut();
        }

        if (!shouldFade)
			return;

		Fading();
	}

	public void FadeOutAndIn(float _wait = 1.0f)
	{
        wait = _wait;
        fadeOutAndIn = true;
        StartFade(false);
	}

    public void FadeInAndOut(float _wait = 1.0f)
    {
        wait = _wait;
        fadeInAndOut = true;
        StartFade();
    }

    public bool HasFadeEnded()
    {
        return (!hasFaded && !shouldFade);
    }

    void Fading()
	{
		if(!hasFaded)
		{
            if (fadeIn)
                FadeIn();
            else
                FadeOut();
        }

        if(dontWait && hasFaded)
        {
            timer = 0.0f;
            hasFaded = false;
            shouldFade = false;
        }
        else if (hasFaded && timer > wait)
        {
            timer = 0.0f;
			hasFaded = false;
            shouldFade = false;

            if(fadeOutAndIn)
            {
                StartFade();
                fadeOutAndIn = false;
                dontWait = true;
            }
            else if (fadeInAndOut)
            {
                StartFade(false);
                fadeInAndOut = false;
                dontWait = true;
            }    
        }

        Color temp = subtitles.color;
        temp.a = fadeVal;
        subtitles.color = temp;

        timer += Time.deltaTime;
    }

	public void StartFade(bool _fadeIn = true)
	{
		if (shouldFade)
			return;

		fadeIn = _fadeIn;
		shouldFade = true;
        dontWait = false;

        if (fadeIn)
			fadeVal = 0.0f;
		else
			fadeVal = 1.0f;

        Color temp = subtitles.color;
        temp.a = fadeVal;
        subtitles.color = temp;
    }

    void FadeIn()
    {
        fadeVal = fade.Lerp(0.0f, 1.0f, timer / duration);

		if(fadeVal >= 1.0f)
		{
			timer = 0.0f;
            hasFaded = true;
		}

    }

    void FadeOut()
	{
        fadeVal = fade.Lerp(1.0f, 0.0f, timer / duration);

        if (fadeVal <= 0.0f)
        {
            timer = 0.0f;
            hasFaded = true;
        }
    }
}

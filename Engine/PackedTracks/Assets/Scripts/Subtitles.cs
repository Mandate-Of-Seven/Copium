using CopiumEngine;
using System;
using System.Diagnostics.Contracts;
using System.Globalization;

public class Subtitles: CopiumScript
{
	public Text subtitles;
	public Fade fade;
    public AudioSource audio;

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
    private bool messageEnded = false;

    public int ending = 0;  // Which ending to use
    private int maxChar = 100;

    private string content;
    private char[] punctuation = { '.', ';', '!'};

	void Start()
	{
		subtitles = GetComponent<Text>();
        content = "";
	}
	void Update()
	{
        if (Input.GetKeyDown(KeyCode.Space))
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

        if (fade.shouldFade || !fade.FadeEnded())
            return;

        if (GameManager.Instance != null && audio == null)
        {
            ending = GameManager.Instance.eventSequence;
            SetContent();

            audio.Play();
        }

        if (HasFadeEnded() && !messageEnded)
        {
            FadeInAndOut();
            ChangingSubtitles();
        }

        if (!shouldFade)
			return;

		Fading();
	}

    void SetContent()
    {
        switch (ending)
        {
            case 1:
                content = Messages.Ending_3B.body;
                audio = AudioManager.Instance.ending3bVO;
                break;
            case 0:
                content = Messages.Ending_2A.body;
                audio = AudioManager.Instance.ending2aVO;
                break;
        }
    }

    void ChangingSubtitles()
    {
        int index = content.IndexOfAny(punctuation);

        // If the punctuation is an ellipsis(...)
        if (content[index + 1] == punctuation[0])
            index = content.LastIndexOfAny(punctuation);

        if (index >= maxChar)
        {
            string temp = content.Substring(0, maxChar);
            int space = temp.LastIndexOf(' ');
            subtitles.text = temp.Substring(0, space + 1);
            content = content.Substring(space + 1);
        }
        else if (index > 0)
        {
            subtitles.text = content.Substring(0, index + 1);
            content = content.Substring(index + 1);
        }
        else // If index is 0 or lesser, means no punctuations
        {
            subtitles.text = content;
            content = "";
        }

        if (index == content.Length || index <= 0 || content.Length == 0)
            messageEnded = true;

        wait = subtitles.text.Length * 0.03f;
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

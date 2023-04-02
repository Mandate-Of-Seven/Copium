using CopiumEngine;
using System;
using System.Diagnostics.Contracts;
using System.Globalization;

public class Subtitles: CopiumScript
{
	public Text subtitles;
	public Text reason;
	public Fade fade;
    public AudioSource audio;
    public AudioSource blizzard;

    private float wait = 0.5f;
	private float fadeVal = 0.0f;
	private float timer = 0.0f;
	private float duration = 1.0f;
    private float waitModifier = 0.0f;

    public float changeBlizzard = 1.0f;
    public float blizzardTimer = 0.0f;

    public bool shouldFade = false;
    public bool fadeIn = false;
    private bool hasFaded = false;
    private bool fadeOutAndIn = false;
    private bool fadeInAndOut = false;
    private bool dontWait = false;
    private bool messageEnded = false;
    private bool changeScene = false;
    private bool hasAudioStarted = false;

    private bool blizzardVolume = false;
    private bool blizzardWait = false;
    private bool blizzardSwap = false;

    public int ending = 0;  // Which ending to use
    private int maxChar = 100;

    private string content;
    private char[] punctuation = { '.', ';', '!'};

	void Start()
	{
		subtitles = GetComponent<Text>();
        waitModifier = 0.03f;
        content = "";

        if (GameManager.Instance != null && audio == null)
        {
            ending = GameManager.Instance.eventSequence;
            SetContent();
        }
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

        BlizzardLogic();

        if (HasFadeEnded() && !changeScene && messageEnded && !fade.shouldFade) // Message ended
        {
            fade.preFade = 2.0f;
            fade.Start(true);
            changeScene = true;
        }
        else if (changeScene && fade.FadeEnded()) // Load next scene
        {
            SceneManager.LoadScene("HowToPlayNew");
        }

        if (fade.shouldFade || !fade.postFaded)
            return;

        if(!hasAudioStarted)
        {
            if(audio != null)
                audio.Play();
            hasAudioStarted = true;
        }

        if (HasFadeEnded() && !messageEnded) // Change subtitles
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
            case -1:
                reason.text = Messages.Event_Ending.Ending_3B.reason;
                content = Messages.Event_Ending.Ending_3B.body;
                waitModifier = 0.03f;
                audio = AudioManager.Instance.ending3bVO;
                break;
            case -2:
                reason.text = Messages.Event_Ending.Ending_2A.reason;
                content = Messages.Event_Ending.Ending_2A.body;
                waitModifier = 0.028f;
                audio = AudioManager.Instance.ending2aVO;
                break;
            case -3:
                reason.text = Messages.Event_Ending.AllDead.reason;
                content = Messages.Event_Ending.AllDead.body;
                waitModifier = 0.02f;
                audio = AudioManager.Instance.endingAllDiedVO;
                break;
            case -4:
                break;
            case -5:
                reason.text = Messages.Event_Ending.Ending_3C.reason;
                content = Messages.Event_Ending.Ending_3C.body;
                waitModifier = 0.02f;
                messageEnded = true;
                break;
        }
    }

    void ChangingSubtitles()
    {
        if (content.Length == 0)
            return;

        int index = content.IndexOfAny(punctuation);

        // If the punctuation is an ellipsis(...)
        if (index + 1 < content.Length && content[index + 1] == punctuation[0])
            index = content.LastIndexOfAny(punctuation);

        if (index >= maxChar)
        {
            string temp = content.Substring(0, maxChar);
            int space = temp.LastIndexOf(' ');
            subtitles.text = temp.Substring(0, space + 1);
            content = content.Substring(space + 1);
        }
        else if (index > 0 && index < content.Length)
        {
            subtitles.text = content.Substring(0, index + 1);
            content = content.Substring(index + 1);
        }
        else // If index is 0 or lesser, means no punctuations
        {
            subtitles.text = content;
            content = "";
        }

        if (index == content.Length || content.Length == 0)
            messageEnded = true;

        wait = subtitles.text.Length * waitModifier;
    }

    void BlizzardLogic()
    {
        if (blizzardTimer > changeBlizzard && !blizzardVolume && !blizzardWait)
        {
            changeBlizzard = RNG.Range(3.0f, 10.0f);
            blizzardVolume = true;
            blizzardSwap = !blizzardSwap;
            blizzardTimer = 0.0f;
        }

        if (blizzardVolume)
            blizzardVolume = UpdateBlizzard();

        if (blizzardWait && blizzardTimer > changeBlizzard / 8.0f)
        {
            blizzardTimer = 0.0f;
            blizzardWait = false;
        }

        blizzardTimer += Time.deltaTime;
    }

    bool UpdateBlizzard()
    {
        float time = blizzardTimer / changeBlizzard;
        if (blizzardSwap)
        {
            blizzard.volume = fade.Lerp(0.25f, 0.8f, time);
        }
        else
        {
            blizzard.volume = fade.Lerp(0.8f, 0.25f, time);
        }


        if (time >= 1.0f)
        {
            blizzardWait = true;
            return false;
        }
        else
            return true;
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

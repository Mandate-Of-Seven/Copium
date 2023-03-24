/*!***************************************************************************************
\file			Event_Ending.cs
\project
\author			Shawn Tanary

\par			Course: GAM250
\par			Section:
\date			10/03/2023

\brief
	Contains functions needed a fade in effect

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

using CopiumEngine;
using System;

public class Fade: CopiumScript
{
	public bool shouldFade;
	public bool fadeIn = true;
	public float end = 1.0f;
	public float start = 0.0f;
	public float fadeVal = 0.0f;

	public float duration = 1.0f;
	public float preFade = 0.5f;
	public float postFade = 0.5f;

	public SpriteRenderer sr;

	private bool preFaded = false;
	private bool postFaded = false;
	private float preposTimer = 0.0f;
	private float timer = 0.0f;
	
    void Start()
	{
        sr = GetComponent<SpriteRenderer>();
		sr.enabled = false;
    }
	void Update()
	{
        if(!shouldFade)
            return;

        if(!preFaded)
        {
            sr.enabled = true;
            if (fadeIn)
                fadeVal = start;
            else
                fadeVal = end;

            Color temp = sr.color;
            temp.a = fadeVal;
            sr.color = temp;
        }

        if (!preFaded && preposTimer > preFade)
        {
            preFaded = true;
            postFaded = true;
            preposTimer = 0.0f;
        }

        if (preFaded && postFaded)
        {
            if (fadeIn)
                FadeIn();
            else
                FadeOut();

            Color temp = sr.color;
            temp.a = fadeVal;
            sr.color = temp;

            timer += Time.deltaTime;
        }

        if (!postFaded && preFaded && preposTimer > postFade)
        {
            fadeVal = start;
            postFaded = true;
            preFaded = false;
            shouldFade = false;
            sr.enabled = false;
        }

        preposTimer += Time.deltaTime;
    }

    public void Start(bool _fadeIn = true)
    {
        fadeIn = _fadeIn;
        shouldFade = true;
        preFaded = postFaded = false;
        timer = preposTimer = 0.0f;
    }

    public bool FadeEnded()
    {
        return (!postFaded && preFaded);
    }

	void FadeIn()
	{
        fadeVal = Lerp(start, end, timer / duration);
        if (fadeVal >= end)
        {
            timer = 0.0f;
            preposTimer = 0.0f;
            postFaded = false;
        }
    }

	void FadeOut()
	{
        fadeVal = Lerp(end, start, timer / duration);
        if (fadeVal <= start)
        {
            timer = 0.0f;
            preposTimer = 0.0f;
            postFaded = false;
        }
    }

	public float Lerp(float a, float b, float t)
	{
		return a + (b - a) * t;
    }
}

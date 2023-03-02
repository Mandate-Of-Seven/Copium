using CopiumEngine;
using System;

public class Fade: CopiumScript
{
	public bool shouldFade;
	public bool fadeIn = true;
	public float amount= 1.0f;
	public float fadeVal = 0.0f;

    void Start()
	{
		shouldFade = true;
    }
	void Update()
	{
        if(shouldFade)
		{
			if(fadeVal>100)
			{
				fadeIn = false;
			}
			else if(fadeVal < 0)
			{
				fadeVal = 0;
                shouldFade = false;
				return;
            }
			
			if(fadeIn)
			{
				fadeVal+=amount;
			}
			else
			{
				fadeVal-=amount;
			}
			Color temp = gameObject.GetComponent<SpriteRenderer>().color;
			temp.a = fadeVal;
			gameObject.GetComponent<SpriteRenderer>().color = temp;

        }

            
    }
}

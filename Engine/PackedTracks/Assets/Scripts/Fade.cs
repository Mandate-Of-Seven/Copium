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
	public float amount= 1.0f;
	public int maxFade = 150;
	public float fadeVal = 0.0f;
	
    void Start()
	{
		
    }
	void Update()
	{
        if(shouldFade)
		{
			if(fadeVal>maxFade)
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
				fadeVal-= amount;
			}
			Color temp = gameObject.GetComponent<SpriteRenderer>().color;
			temp.a = fadeVal/100;
			gameObject.GetComponent<SpriteRenderer>().color = temp;

        }

            
    }
}

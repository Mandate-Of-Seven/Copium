/*!***************************************************************************************
\file			StringScramblerEffect.cs
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			10/03/2023

\brief
	Creates a encoded animated text effect by randomising each character, can be tweaked
	by its constructor parameters.

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

using CopiumEngine;
using System;
using System.Reflection;
using System.Text;

public class StringScramblerEffect
{
	float intervalTimeElasped = 0f;
	float interval = 0.01f;
	int iterations = 0; //If one it should switch to the character immediately
	int charIndex = 0;
	int iterIndex = 0;
	int concurrency = 0;
	StringBuilder initial = new StringBuilder();
	StringBuilder target = new StringBuilder();

	public StringScramblerEffect(string _initial, string _target, int _concurrency, int _iterations, float _interval)
	{
		initial.Append(_initial);
		target.Append(_target);
		concurrency = _concurrency;
		iterations = _iterations;
		interval = _interval;
		charIndex = 0;
		iterIndex = 0;
	}

	public void Skip()
    {
		initial = target;
    }

	public bool Done()
	{
		return target == null || (target.ToString() == initial.ToString());
	}

	public string Current()
    {
		return initial.ToString();
    }
	char GetRandomChar()
	{
		int charType = RNG.Range(0,2);
		char ch = '0';
		switch(charType)
		{
			case 0:
				ch =  (char)RNG.Range(48,57);
			break;
			case 1:
				ch =  (char)RNG.Range(65,90);
			break;
			case 2:
				ch =  (char)RNG.Range(97,122);
			break;
		}
		return ch;
	}

	public string Scramble()
	{
		if (Done())
			return initial.ToString();
		float dt = Time.deltaTime;
		if (intervalTimeElasped >= interval)
		{
			Update();
			intervalTimeElasped -= interval;
		}
		intervalTimeElasped += dt;
		return initial.ToString();
	}

	
	void Update()
	{
		if (iterIndex == iterations)
		{
			int index = charIndex - concurrency;
			iterIndex = 0;
			if (index >= 0)
			{
				if (index >= target.Length)
				{
					initial.Remove(index,1);
				}
				else
				{
					initial[index] = target[index];
					++charIndex;
				}
				return;
			}
			else
			{
				++charIndex;
			}
		}
		for (int i = -concurrency; i < 0; ++i)
		{
			if (charIndex + i < 0)
				continue;

			char ch = GetRandomChar();
			// If it hasnt reached target length add, if it exceeds its own length, add only if 
			if (initial.Length == charIndex+i && (charIndex+i < target.Length))
			{
				initial.Append(ch);
			}
			else
			{
				if (charIndex+i == initial.Length)
					break;
				initial[charIndex+i] = ch;
			}
		}
		++iterIndex;
	}
}

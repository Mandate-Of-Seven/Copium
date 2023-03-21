using CopiumEngine;
using System;
using System.Reflection;
using System.Text;

public class StringTypeWriterEffect
{
	float intervalTimeElasped = 0f;
	float interval = 0.01f;
	int charIndex = 0;
	StringBuilder initial = new StringBuilder();
	StringBuilder target = new StringBuilder();
	bool wait = false;
	bool done = false;
	float waitTime = 20f;

	public StringTypeWriterEffect(string _target, float _interval)
	{
		target.Append(_target);
		interval = _interval;
		charIndex = 0;
	}
	public void Skip()
	{
		interval /= 5f;
	}
	public bool Done()
	{
		return done || target == null || (target.ToString() == initial.ToString());
	}

	public string Write()
	{
		if (Done())
			return target.ToString();
		float dt = Time.deltaTime;
		char ch = target[charIndex];
		if (charIndex > 0 && target[charIndex-1] == '.' && !wait)
		{
			wait = true;
		}
		
		if (wait)
		{
			if (intervalTimeElasped <= interval * waitTime)
			{
				intervalTimeElasped += dt;
				return initial.ToString();
			}
			else
			{
				intervalTimeElasped = interval;
				wait = false;
			}
		}
		while (intervalTimeElasped >= interval && charIndex < target.Length)
		{
			Update();
			intervalTimeElasped -= interval;
		}
		intervalTimeElasped += dt;
		if (charIndex+1 == target.Length)
        {
			done = true;
        }
		return initial.ToString();
	}

	
	void Update()
	{
		initial.Append(target[charIndex]);
		++charIndex;
	}
}

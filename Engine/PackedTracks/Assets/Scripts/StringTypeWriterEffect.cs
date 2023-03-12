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

	public StringTypeWriterEffect(string _initial, string _target, float _interval)
	{
		initial.Append(_initial);
		target.Append(_target);
		interval = _interval;
		charIndex = 0;
	}

	public bool Done()
	{
		return target == null || (target.ToString() == initial.ToString());
	}

	public string Write()
	{
		if (Done())
			return target.ToString();
		float dt = Time.deltaTime;
		char ch = target[charIndex];
		if (ch == '\n' && !wait)
		{
			wait = true;
		}
		
		if (wait)
		{
			if (intervalTimeElasped <= interval)
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
		while (intervalTimeElasped >= interval)
		{
			Update();
			intervalTimeElasped -= interval;
		}
		intervalTimeElasped += dt;
		return initial.ToString();
	}

	
	void Update()
	{
		initial.Append(target[charIndex]);
		++charIndex;
	}
}

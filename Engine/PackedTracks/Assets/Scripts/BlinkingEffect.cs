using CopiumEngine;
using System;

public class BlinkingEffect: CopiumScript
{
	public Image image;
	public float interval = 1.0f;
	private float timer = 0.0f;
	bool fadeIn = false;
	void Start()
	{

	}
	void Update()
	{
		float dt = Time.deltaTime;
		if (timer >= interval)
		{
			fadeIn = !fadeIn;
			timer = 0;
		}
		Color color = image.color;

		if (fadeIn)
		{
			color.a = Mathf.Lerp(0,1,timer);
		}
		else
		{
			color.a = Mathf.Lerp(1,0,timer);
		}
		image.color = color;
		timer += dt;
	}
}

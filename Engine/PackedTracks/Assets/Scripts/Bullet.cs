using CopiumEngine;
using System;

public class Bullet: CopiumScript
{
	float speed = 0.5f;
	public bool enemy;

	void Update()
	{
		Vector3 pos = transform.position;
		if (enemy)
		{
			pos.x -= speed * Time.deltaTime;
		}
		else
		{
			pos.x += speed * Time.deltaTime;
		}
		transform.position = pos;
	}
}

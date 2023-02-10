using CopiumEngine;
using System;

public class Bullet: CopiumScript
{
	public float speed = 0.5f;

    float timer = 0;

	void Update()
	{
        Vector3 pos = transform.position;
        pos.x += speed;
        transform.position = pos;
    }
}

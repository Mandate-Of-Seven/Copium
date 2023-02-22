using CopiumEngine;
using System;

public class Bullet: CopiumScript
{
	public float speed = 0.5f;
    public string yes = "hello";
    float timer = 0;

	void Update()
	{
        Vector3 pos = transform.position;
        pos.x += speed;
        transform.position = pos;
    }
}

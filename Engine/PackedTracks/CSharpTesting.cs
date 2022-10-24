using CopiumEngine;
using System;
public class CSharpTesting : CopiumScript
{
	public double speed = 0.2;
    void Awake()
	{
		Vector3 position = transform.position;
		if (Input.GetKey(KeyCode.D))
		{
			position.x += speed;
		}
		else if (Input.GetKey(KeyCode.A))
		{
			position.x -= speed;
		}

		if (Input.GetKey(KeyCode.W))
		{
			position.y += speed;
		}
		else if (Input.GetKey(KeyCode.S))
		{
			position.y -= speed;
		}
		transform.position = position;
	}

	void Start()
	{

	}

	void Update()
	{

	}
}

using CopiumEngine;
using System;
public class CSharpTesting : CopiumScript
{
	public float speed = 0.2f;
	public Vector2 vec2;
	public Vector3 vec3;
    void Awake()
	{
	}

	void Start()
	{

	}

	void Update()
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
		vec2.x = position.x;
		vec2.y = position.y;
		Console.WriteLine("GameObject ID: " + gameObject.ID + " running!");
	}
}

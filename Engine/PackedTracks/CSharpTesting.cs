using CopiumEngine;
using System;
public class CSharpTesting : CopiumScript
{
    void Awake()
    {
		if (Input.GetKey(KeyCode.D))
		{
			Vector3 position = transform.position;
			transform.position = position * 0.5f;
		}
    }

	void Start()
	{

	}

	void Update()
	{

	}
}

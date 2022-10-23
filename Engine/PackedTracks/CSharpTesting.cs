using CopiumEngine;
using System;

public class CSharpTesting : CopiumScript
{
    void Awake()
    {
		if (Input.GetKey(65))
		{
			//Vector3 yessir = new Vector3(1,2,3);
			if (transform == null)
				Console.WriteLine("Tranform was null");
			if (gameObject == null)
				Console.WriteLine("GameObj was nusll");
			Console.WriteLine("Running");
			//Console.WriteLine(yessir.y); //+ " Y: " + transform.position.y + " Z: " + transform.position.z);
		}
    }

	void Start()
	{

	}

	void Update()
	{

	}
}

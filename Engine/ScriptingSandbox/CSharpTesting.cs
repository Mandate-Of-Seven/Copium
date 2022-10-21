using CopiumEngine;
using System;

public class CSharpTesting : CopiumScript
{
    void Awake()
    {
		if (Input.GetKey(65))
		{
			Vector3 yessir = new Vector3(1,2,3);
			Console.WriteLine("WOT");
			Console.WriteLine(yessir.y); //+ " Y: " + transform.position.y + " Z: " + transform.position.z);
		}
    }

	void Start()
	{

	}

	void Update()
	{

	}
}

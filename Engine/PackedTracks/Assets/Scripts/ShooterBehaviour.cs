using CopiumEngine;
using System;

public class ShooterBehaviour: CopiumScript
{
	public GameManager gameManager;
	public bool enemy = false;
	ShooterBehaviour target;
	float baseHealth = 100f;

	void Start()
	{
	}

	void Update()
	{
		//Look for enemies

		Vector3 pos = transform.position;
		if (enemy)
        {
			pos.x -= 0.025f;
        }
		else
		{
			pos.x += 0.025f;
		}
		transform.position = pos;
	}
}

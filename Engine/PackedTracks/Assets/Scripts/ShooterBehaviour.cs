using CopiumEngine;
using System;

public class ShooterBehaviour: CopiumScript
{
	public GameManager gameManager;
	public Bullet bullet;
	public bool enemy = false;
	ShooterBehaviour target;
	float baseHealth = 100f;


	float timer = 0f;

	float shootingInterval = 1f;
	void Start()
	{
	}

	void Update()
	{
		timer += Time.deltaTime;
		if (timer > shootingInterval)
        {
			GameObject newBullet = Instantiate(bullet.gameObject);
			newBullet.transform.position = transform.position;
			timer = 0f;
        }
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

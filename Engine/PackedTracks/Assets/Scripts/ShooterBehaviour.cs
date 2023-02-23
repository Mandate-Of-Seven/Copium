using CopiumEngine;
using System;

public class ShooterBehaviour: CopiumScript
{
	public GameManager gameManager;
	public GameObject bullet;
	public Bullet scripto;
	public bool enemy = false;
	ShooterBehaviour target;
	float baseHealth = 100f;
	float yess = 10.0f;

	float timer = 0f;

	float shootingInterval = 1f;
	GameObject newBullet;
	void Start()
	{
	}

	void Update()
	{
		SceneManager.LoadScene("Demo");
		if (scripto == null)
			Console.WriteLine("SCRIPTO IS NULL");
		else
		{
			Console.WriteLine("SCRIPTO EXISTS");
			Console.WriteLine(scripto.gameObject.transform.position.x);
		}
		timer += Time.deltaTime;
		if (timer > shootingInterval)
        {
			// if (newBullet == null)
			// 	newBullet = Instantiate(bullet.gameObject);
			//newBullet.transform.position = transform.position;
			Vector3 pos2 = bullet.transform.position;
			pos2.x += 10.0f;
			bullet.transform.position = pos2;
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

using CopiumEngine;
using System;

public class ShowcaseMovement: CopiumScript
{
	public GameObject partner;
	public float speed;

	void Start()
	{

	}
	void Update()
	{
		Vector2 direction = Vector2.Zero;
		if (Input.GetKeyDown(KeyCode.A))
			direction.x = -speed;
		if (Input.GetKeyDown(KeyCode.D))
			direction.x = speed;
		if (Input.GetKeyDown(KeyCode.W))
			direction.y = speed;
		if (Input.GetKeyDown(KeyCode.S))
			direction.y = -speed;

		if (partner != null)
        {
			Vector3 partnerPos = partner.transform.position;
			partnerPos.x += direction.x;
			partnerPos.y += direction.y;
			partner.transform.position = pos;
		}
		Vector3 pos = transform.position;
		pos.x += direction.x;
		pos.y += direction.y;
		transform.position = pos;
	}
}

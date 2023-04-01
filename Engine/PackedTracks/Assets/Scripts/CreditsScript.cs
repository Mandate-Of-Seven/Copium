using CopiumEngine;
using System;

public class CreditsScript: CopiumScript
{

	public float endPoint = 2.1f;

	public float speed = 2f;

	public float WaitTime = 3f;

	public GameObject mandateOfSeven;

	private bool playNextText = true;

	private bool mandateOfSevenReachSpot = false;



	void Start()
	{

	}
	void Update()
	{
		Vector3 posMOS = mandateOfSeven.transform.position;

		if (posMOS.y < endPoint)
		{
			posMOS.y += speed * Time.deltaTime;
			mandateOfSeven.transform.position = posMOS;

			playNextText = false;

		}
		else if (posMOS.y > endPoint)
        {
			mandateOfSevenReachSpot = true;
			if (mandateOfSevenReachSpot && !playNextText)
            {
				WaitTime -= Time.deltaTime;
				if (WaitTime <= 0 && playNextText == false)
				{
					mandateOfSeven.SetActive(false);
					mandateOfSevenReachSpot = false;
					playNextText = true;
					WaitTime = 3f;
				}
			}
			
        }
	}

}

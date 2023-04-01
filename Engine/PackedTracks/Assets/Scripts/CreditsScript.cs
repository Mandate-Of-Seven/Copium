using CopiumEngine;
using System;

public class CreditsScript: CopiumScript
{

	public float endPoint = 2.1f;

	public float speed = 2f;

	public float WaitTime = 3f;

	public GameObject mandateOfSeven;

	public GameObject digipenStaff;

	public GameObject specialThanks;

	public GameObject softwareAndTools;

	public GameObject digipenText;

	public Image logosSIT;

	public Image logosDigipen;

	public GameObject logos;

	public Fade fade;

	private bool playNextText = true;

	private bool mandateOfSevenReachSpot = false;

	private bool digipenStaffReachSpot = false;

	private bool specialThanksReachSpot = false;

	private bool softwareAndToolsReachSpot = false;

	private bool digipenTextReachSpot = false;

	private bool logosReachSpot = false;

	private bool sceneChange = false;

	private bool loadScene = false;

	private int playCount = 0;


	void Start()
	{

	}
	void Update()
	{
		Vector3 posMOS = mandateOfSeven.transform.position;

		Vector3 posDS = digipenStaff.transform.position;

		Vector3 posST = specialThanks.transform.position;

		Vector3 posSU = softwareAndTools.transform.position;

		Vector3 posDT = digipenText.transform.position;

		Color colSIT = logosSIT.color;

		Color colDigipen = logosDigipen.color;

		if (playCount == 0)
        {
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
					if (WaitTime <= 0 && mandateOfSevenReachSpot)
					{
						
						
						//mandateOfSevenReachSpot = false;
						playCount = 1;
						playNextText = true;
						WaitTime = 3f;
					}
					
				}
			}
		}
		

		if (playCount == 1)
        {
			posMOS.y += speed * Time.deltaTime;
			mandateOfSeven.transform.position = posMOS;

			if (posDS.y < endPoint)
            {
				if (playNextText = true && !digipenStaffReachSpot && mandateOfSevenReachSpot)
				{
					posDS.y += speed * Time.deltaTime;
					digipenStaff.transform.position = posDS;

					playNextText = false;
				}
			}
			else if (posDS.y > endPoint)
            {
				digipenStaffReachSpot = true;
				mandateOfSeven.SetActive(false);
				if (digipenStaffReachSpot && !playNextText && digipenStaffReachSpot)
                {
					WaitTime -= Time.deltaTime;
					if (WaitTime <= 0)
                    {
						
						
						playCount = 2;
						playNextText = true;
						
						WaitTime = 3f;
                    }
                }
            }
			

        }

		if (playCount == 2)
        {
			posDS.y += speed * Time.deltaTime;
			digipenStaff.transform.position = posDS;
			if (posST.y < 0.5f)
            {
				if (!specialThanksReachSpot && digipenStaffReachSpot)
                {
					posST.y += speed * Time.deltaTime;
					specialThanks.transform.position = posST;

					playNextText = false;
                }
            }
			else if (posST.y > 0.5f)
            {
				specialThanksReachSpot = true;
				digipenStaff.SetActive(false);
				if (specialThanksReachSpot && !playNextText)
                {
					WaitTime -= Time.deltaTime;
					if (WaitTime <= 0)
                    {
						
						
						playNextText = true;
						playCount = 3;
						WaitTime = 3f;
                    }
                }

            }
        }

		if (playCount == 3)
        {
			posST.y += speed * Time.deltaTime;
			specialThanks.transform.position = posST;
			if (posSU.y < 0.5f)
            {
				if (!softwareAndToolsReachSpot && specialThanksReachSpot)
                {
					posSU.y += speed * Time.deltaTime;
					softwareAndTools.transform.position = posSU;

					playNextText = false;
                }

            }
			else if (posSU.y > 0.5f)
            {
				softwareAndToolsReachSpot = true;
				specialThanks.SetActive(false);
				if (softwareAndToolsReachSpot && !playNextText)
                {
					WaitTime -= Time.deltaTime;
					if(WaitTime <= 0)
                    {
						
						
						playNextText = true;
						playCount = 4;
						WaitTime = 3f;
                    }
				}
				
            }
        }


		if (playCount == 4)
        {
			posSU.y += speed * Time.deltaTime;
			softwareAndTools.transform.position = posSU;
			if (posDT.y < endPoint)
            {
				if (!digipenTextReachSpot && softwareAndToolsReachSpot)
                {
					posDT.y += speed * Time.deltaTime;
					digipenText.transform.position = posDT;

					playNextText = false;
                }
            }
			else if (posDT.y > endPoint)
            {
				digipenTextReachSpot = true;
				softwareAndTools.SetActive(false);
				if (digipenTextReachSpot && !playNextText)
                {
					WaitTime -= Time.deltaTime;
					if (WaitTime <= 0)
                    {
						
						
						playNextText = true;
						playCount = 5;
						WaitTime = 10f;
                    }
				}
            }
        }

		if (playCount == 5)
        {
			posDT.y += speed * Time.deltaTime;
			digipenText.transform.position = posDT;
			if (digipenTextReachSpot)
            {
				
				WaitTime -= Time.deltaTime;

				if (WaitTime < 7f)
                {
					logos.SetActive(true);
					colDigipen.a += Time.deltaTime;
					colSIT.a += Time.deltaTime;

					logosDigipen.color = colDigipen;
					logosSIT.color = colSIT;

					playNextText = false;
					logosReachSpot = true;
				}
				else if (WaitTime < 0f && logosReachSpot)
                {
					digipenText.SetActive(false);
					sceneChange = true;
				}

			}
        }

		if (sceneChange)
        {
			fade.Start();
			loadScene = true;
		}

		if (loadScene && fade.FadeEnded())
        {
			loadScene = false;
			SceneManager.LoadScene("MainMenu");
		}

	}


}

/*!***************************************************************************************
\file			BackstoryMenu.cs
\project
\author			Wu Ze Zheng
\co-author		Abdul Hadi
                Matthew Lau

\par			Course: GAM200
\par			Section:
\date			09/03/2023

\brief
	Functionality for the HowtoPlay Menu

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
using CopiumEngine;
using System;
using System.Runtime.Remoting.Messaging;
public class BackstoryMenu: CopiumScript
{
    public AudioSource sfx_pageflip;
    public GameObject MainPage;
    public GameObject Page2;
    public GameObject Page3;

    public Button NextButton;
    public Button PrevButton;
    public Button ExitButton;
    public int page = 1;

    void Start()
	{
        
	}


    void Update()
    {
        if (PrevButton.state == ButtonState.OnClick)
        {
            sfx_pageflip.Play();
            page--;
            ShowPage(page);
        }
        if (NextButton.state == ButtonState.OnClick)
        {
            sfx_pageflip.Play();
            page++;
            ShowPage(page);
        }
        if (ExitButton.state == ButtonState.OnClick)
        {
            gameObject.SetActive(false);

        }
    }

    void ShowPage(int _page)
    {

        if (page == 1)
        {
            MainPage.SetActive(true);
            Page2.SetActive(false);
            PrevButton.gameObject.SetActive(false);
        }
        else if (page == 2)
        {
            Page2.SetActive(true);
            MainPage.SetActive(false);
            Page3.SetActive(false);
            PrevButton.gameObject.SetActive(true);
            NextButton.gameObject.SetActive(true);
        }

        else if (page == 3)
        {
            MainPage.SetActive(false);
            Page3.SetActive(true);
            Page2.SetActive(false);
            NextButton.gameObject.SetActive(false);

        }
        else
        {
            /*page = 0;
            if (sceneChange)
            {
                fade.Start();
                loadScene = true;
            }
            else
                gameManager.CloseManual();*/
            Console.WriteLine("No Page");
        }

    }
}
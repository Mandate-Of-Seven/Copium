/*!***************************************************************************************
\file			BackstoryMenu.cs
\project
\author			Wu Ze Zheng
\co-author		Shawn Tanary
                Matthew Lau

\par			Course: GAM250
\par			Section:
\date			03/04/2023

\brief
	Functionality for the HowtoPlay Menu

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
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

    public Animator bBSIcon;
    public Animator cBSIcon;
    public Animator dBSIcon;
    public Animator hBSIcon;

    public Button NextButton;
    public Button PrevButton;
    public Button ExitButton;
    private ButtonWrapper ExitBtnWrapper;

    public Animator bronson;
    public Animator chuck;
    public Animator danton;
    public Animator harris;

    public int page = 1;

    void Start()
	{
        // bronson.stop();
        // chuck.stop();
        // danton.stop();
        // harris.stop();

        ExitBtnWrapper = new ButtonWrapper(ExitButton);
        ExitBtnWrapper.SetImage(ExitButton.GetComponent<Image>());
        ExitBtnWrapper.clickedSFX = AudioManager.Instance.fileCloseSFX;

        // bBSIcon.stop();
        // cBSIcon.stop();
        // dBSIcon.stop();
        // hBSIcon.stop();
	}


    void Update()
    {
        if(!PauseMenu.Instance.isPaused)
        {
            // bronson.stop();
            // chuck.stop();
            // danton.stop();
            // harris.stop();
        }

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
        if (ExitBtnWrapper.GetState() == ButtonState.OnClick)
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
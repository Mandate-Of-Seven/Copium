/*!***************************************************************************************
\file			HowtoPlayMenu.cs
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
public class HowtoPlayMenu: CopiumScript
{
    public bool sceneChange = false;
    public Text text;
    public Button NextButton;
    public Button PrevButton;
    public AudioSource sfx_pageflip;
    public GameObject prevButtonObject;
    public GameManager gameManager;

    public GameObject MainPage;
    public GameObject Page2;
    public GameObject Page3;
    public GameObject Page4;
    public GameObject NextPage;
    public GameObject PrevPage;
    public GameObject NextPage2;
    public GameObject PrevPage3;
    public GameObject NextPage3;
    public GameObject PrevPage4;

    public Fade fade;

    public Button ExitButton;

    public int page = 1;

    private bool loadScene = false;

    void Start()
	{
		prevButtonObject.SetActive(false);
        text.text = "Use the mouse to interact with objects in the train cockpit\n " +
                     "An alert will be shown if a new event has occured.\n " +
                     "In the cockpit, there is a meter to show how far you are\n " +
                     "from the destination.";
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
            Console.WriteLine(page.ToString());

        }

        if (loadScene && fade.FadeEnded())
        {
            loadScene = false;
            SceneManager.LoadScene("MainMenu");
        }

        if (NextButton.state == ButtonState.OnHover && page == 1)
        {
            NextPage.SetActive(true);

        }
        else
        {
            NextPage.SetActive(false);
        }

        if (PrevButton.state == ButtonState.OnHover && page == 2)
        {
            PrevPage.SetActive(true);
        }
        else
        {
            PrevPage.SetActive(false);
        }

        if (NextButton.state == ButtonState.OnHover && page == 2)
        {
            NextPage2.SetActive(true);
        }
        else
        {
            NextPage2.SetActive(false);
        }

        if (PrevButton.state == ButtonState.OnHover && page == 3)
        {
            PrevPage3.SetActive(true);
        }
        else
        {
            PrevPage3.SetActive(false);
        }

        if (NextButton.state == ButtonState.OnHover && page == 3)
        {
            NextPage3.SetActive(true);
        }
        else
        {
            NextPage3.SetActive(false);
        }

        if (PrevButton.state == ButtonState.OnHover && page == 4)
        {
            PrevPage4.SetActive(true);
        }
        else
        {
            PrevPage4.SetActive(false);
        }

    }
    /***************************************************************************/
    /*!
    \brief       
        Selects which page to show
    \param _page
        The page to show
    */
    /**************************************************************************/
    void ShowPage(int _page)
    {

        Console.WriteLine("show page");

        if (page == 1)
        {
            MainPage.SetActive(true);
            Page2.SetActive(false);
            prevButtonObject.SetActive(false);
        }
        else if (page == 2)
        {
            Page2.SetActive(true);
            MainPage.SetActive(false);
            Page3.SetActive(false);
            prevButtonObject.SetActive(true);
        }

        else if (page == 3)
        {
            MainPage.SetActive(false);
            Page3.SetActive(true);
            Page2.SetActive(false);
            Page4.SetActive(false);

        }
        else if (page == 4)
        {
            Page4.SetActive(true);
            Page3.SetActive(false);
            Page2.SetActive(false);
            MainPage.SetActive(false);
        }
        else
        {
            page = 0;
            if (sceneChange)
            {
                fade.Start();
                loadScene = true;
            }
            else
                gameManager.CloseManual();
        }

    }

}
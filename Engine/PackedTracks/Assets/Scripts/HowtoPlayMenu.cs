/*!***************************************************************************************
\file			HowtoPlayMenu.cs
\project
\author			Wu Zezheng
\co-authors     Abdul Hadi

\par			Course: GAM200
\par			Section:
\date			10/03/2022

\brief
	This file contains the start and update of the how to play menu.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
using CopiumEngine;
using System;
using System.Runtime.Remoting.Messaging;
public class HowtoPlayMenu: CopiumScript
{
    public Text text;
    public Button NextButton;
    public Button PrevButton;
    public AudioSource sfx_pageflip;
    public GameObject prevButtonObject;

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

    public Button ExitButton;

    int page = 1;

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
        if (page == 1)
        {
            MainPage.SetActive(true);
            Page2.SetActive(false);
            prevButtonObject.SetActive(false);
        }

        if (page == 2)
        {
            Page2.SetActive(true);
            MainPage.SetActive(false);
            Page3.SetActive(false);
            prevButtonObject.SetActive(true);
        }

        if (page == 3)
        {

            Page3.SetActive(true);
            Page2.SetActive(false);
            Page4.SetActive(false);

            //SceneManager.LoadScene("MainMenu");
        }
        if (page == 4)
        {
            Page4.SetActive(true);
            Page3.SetActive(false);
        }

    }

}
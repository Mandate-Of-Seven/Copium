/*!***************************************************************************************
\file			MainMenu.cs
\project
\author			Abdul Hadi
\co-authors     Wu Zezheng

\par			Course: GAM200
\par			Section:
\date			10/03/2022

\brief
	This file holds the start and update of the Main Menu screen.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
using CopiumEngine;
using System;

public class MainMenu: CopiumScript
{
    public AudioSource bgm_MainMenu;
    public AudioSource sfx_ButtonHover;
    public AudioSource sfx_ButtonClick;
    public Button StartGameButton;
    public Button QuitGameButton;
    public Button HowtoPlayButton;
    public Button YesButton;
    public Button NoButton;

    public GameObject QuitMenu;
    public GameObject arrowLocStart;
    public GameObject arrowLocHTP;
    public GameObject arrowLocQuit;

    public GameObject arrow;

    private bool hovering = false;
    private bool canPlayHoverSound = true;

    void Start()
    {
        bgm_MainMenu.Play();
        arrow.SetActive(false);
    }

    void Update()
    {
        if(StartGameButton.state == ButtonState.OnClick)
        {
            SceneManager.LoadScene("Demo");
        }

        if(HowtoPlayButton.state == ButtonState.OnClick)
        {
            SceneManager.LoadScene("HowToPlayNew");
        }

        if (QuitGameButton.state == ButtonState.OnClick)
        {
            QuitMenu.SetActive(true);

           
        } 
        if (YesButton.state == ButtonState.OnClick)
        {
            Application.Quit();
        }
            
        if (NoButton.state == ButtonState.OnClick)
        {
            QuitMenu.SetActive(false);
        }


        if (StartGameButton.state == ButtonState.OnHover || HowtoPlayButton.state == ButtonState.OnHover || QuitGameButton.state == ButtonState.OnHover)
        {
            hovering = true;
        }
        else
        {
            arrow.SetActive(false);
            hovering = false;
            canPlayHoverSound = true;
        }

        if (hovering && canPlayHoverSound)
        {
            sfx_ButtonHover.Play();
            canPlayHoverSound = false;
        }

        if (StartGameButton.state == ButtonState.OnHover)
        {
            arrow.SetActive(true);
            arrow.transform.position = arrowLocStart.transform.position;
        }
        if(HowtoPlayButton.state == ButtonState.OnHover)
        {
            arrow.SetActive(true);
            arrow.transform.position = arrowLocHTP.transform.position;
        }
        if(QuitGameButton.state == ButtonState.OnHover)
        {
            arrow.SetActive(true);
            arrow.transform.position = arrowLocQuit.transform.position;
        }
    }
}



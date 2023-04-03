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
    public Button StartGameButton;
    private ButtonWrapper StartGameButtonWrapper;
    public Button QuitGameButton;
    private ButtonWrapper QuitGameButtonWrapper;
    public Button CreditsButton;
    private ButtonWrapper CreditsButtonWrapper;
    public Button YesButton;
    private ButtonWrapper YesButtonWrapper;
    public Button NoButton;
    private ButtonWrapper NoButtonWrapper;

    public GameObject QuitMenu;
    public GameObject arrowLocStart;
    public GameObject arrowLocHTP;
    public GameObject arrowLocQuit;

    public GameObject arrow;

    public Fade fade;

    private bool hovering = false;
    private bool canPlayHoverSound = true;

    private bool loadGame = false;
    private bool loadCredits = false;

    void Start()
    {
        bgm_MainMenu.Play();
        arrow.SetActive(false);

        StartGameButtonWrapper = new ButtonWrapper(StartGameButton);
        CreditsButtonWrapper = new ButtonWrapper(CreditsButton);
        QuitGameButtonWrapper = new ButtonWrapper(QuitGameButton);
        YesButtonWrapper = new ButtonWrapper(YesButton);
        NoButtonWrapper = new ButtonWrapper(NoButton);
    }

    void Update()
    {
        if(StartGameButtonWrapper.GetState() == ButtonState.OnClick)
        {
            fade.Start();
            loadGame = true;
        }

        if(CreditsButtonWrapper.GetState() == ButtonState.OnClick)
        {
            fade.Start();
            loadCredits = true;
        }

        if(loadGame && fade.FadeEnded())
        {
            loadGame = false;
            SceneManager.LoadScene("Demo");
        }

        if(loadCredits && fade.FadeEnded())
        {
            loadCredits = false;
            SceneManager.LoadScene("HowToPlayNew");
        }

        if (QuitGameButtonWrapper.GetState() == ButtonState.OnClick)
        {
            QuitMenu.SetActive(true);
        } 
        if (YesButtonWrapper.GetState() == ButtonState.OnClick)
        {
            Application.Quit();
        }  
        if (NoButtonWrapper.GetState() == ButtonState.OnClick)
        {
            QuitMenu.SetActive(false);
        }

        if (StartGameButtonWrapper.GetState() == ButtonState.OnHover)
        {
            arrow.SetActive(true);
            arrow.transform.position = arrowLocStart.transform.position;
        }
        if(CreditsButtonWrapper.GetState() == ButtonState.OnHover)
        {
            arrow.SetActive(true);
            arrow.transform.position = arrowLocHTP.transform.position;
        }
        if(QuitGameButtonWrapper.GetState() == ButtonState.OnHover)
        {
            arrow.SetActive(true);
            arrow.transform.position = arrowLocQuit.transform.position;
        }
    }
}



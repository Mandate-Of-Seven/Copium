/*!***************************************************************************************
\file			PauseMenu.cs
\project
\author			Abdul Hadi
\co-authors     Shawn Tanary

\par			Course: GAM200
\par			Section:
\date			10/03/2022

\brief
	This file contains the start and update of the pause menu

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
using CopiumEngine;
using System;

public class PauseMenu: CopiumScript
{
    //gameObjects
    public GameObject PauseCanvas;
    public GameObject PauseYesGameObj;
    public GameObject PauseNoGameObj;
    public GameObject PauseResumeGameObj;
    public GameObject PauseMainMenuGameObj;
    public GameObject PauseQuitGameObj;
    public GameObject PauseTextGameObj;
    public GameObject AudioTextGameObj;
    public GameObject PauseOptionsGameObj;
    public GameObject PauseAudioOnGameObj;
    public GameObject PauseAudioOffGameObj;
    public GameObject PauseReturnGameObj;

    //buttons
    public Button PauseMainMenuBtn;
    public Button PauseResumeBtn;
    public Button PauseQuitBtn;
    public Button PauseYesBtn;
    public Button PauseNoBtn;    
    public Button PauseOptionsBtn;
    public Button PauseAudioOnBtn;
    public Button PauseAudioOffBtn;
    public Button PauseReturnBtn;

    //text
    public Text AudioText;
    public Text PauseText;
    public bool isPaused = false;
    public bool returnToMenu = false;
    public bool quitGame = false;

    public Fade fade;

    private bool loadQuit = false;
    private bool loadScene = false;

    void Start()
	{

	}
	void Update()
	{
        if (Input.GetKeyDown(KeyCode.Escape))
        {
            isPaused = !isPaused;
            PauseCanvas.SetActive(isPaused);
            if (isPaused)
            {
                InternalCalls.PauseAllAnimation();
            }
            else
            {
                InternalCalls.PlayAllAnimation();
            }
        }

        if (fade.FadeEnded())
        {
            if(loadQuit)
            {
                loadQuit = false;
                Application.Quit();
            }
            else if(loadScene)
            {
                loadScene = false;
                SceneManager.LoadScene("MainMenu");
            }
        }

        if(isPaused)
        {
            if (PauseResumeBtn.state == ButtonState.OnClick)
            {
                isPaused = false;
                PauseCanvas.SetActive(false);
                InternalCalls.PlayAllAnimation();
            }

            if (PauseMainMenuBtn.state == ButtonState.OnClick)
            {
                PauseYesGameObj.SetActive(true);
                PauseNoGameObj.SetActive(true);
                PauseResumeGameObj.SetActive(false);
                PauseMainMenuGameObj.SetActive(false);
                PauseOptionsGameObj.SetActive(false);
                PauseQuitGameObj.SetActive(false);
                PauseTextGameObj.SetActive(true);
                PauseText.text = "Are you sure?";
                returnToMenu = true;
            }
             if (PauseOptionsBtn.state == ButtonState.OnClick)
            {
                AudioTextGameObj.SetActive(true);
                PauseAudioOffGameObj.SetActive(true);
                PauseAudioOnGameObj.SetActive(true);
                PauseReturnGameObj.SetActive(true);
                PauseResumeGameObj.SetActive(false);
                PauseMainMenuGameObj.SetActive(false);
                PauseQuitGameObj.SetActive(false);
                PauseOptionsGameObj.SetActive(false);
                

            }

            if (PauseAudioOffBtn.state == ButtonState.OnClick)
            {
                
            }
            if (PauseReturnBtn.state == ButtonState.OnClick)
                {
                AudioTextGameObj.SetActive(false);
                PauseAudioOffGameObj.SetActive(false);
                PauseAudioOnGameObj.SetActive(false);
                PauseReturnGameObj.SetActive(false);
                PauseResumeGameObj.SetActive(true);
                PauseMainMenuGameObj.SetActive(true);
                PauseQuitGameObj.SetActive(true);
                PauseOptionsGameObj.SetActive(true);
                }
            if (PauseQuitBtn.state == ButtonState.OnClick)
            {
                PauseYesGameObj.SetActive(true);
                PauseNoGameObj.SetActive(true);
                PauseResumeGameObj.SetActive(false);
                PauseMainMenuGameObj.SetActive(false);
                PauseQuitGameObj.SetActive(false);
                PauseOptionsGameObj.SetActive(false);
                PauseTextGameObj.SetActive(true);
                PauseText.text = "Are you sure?";
                quitGame = true;
            }

            if (PauseNoBtn.state == ButtonState.OnClick)
            {
                PauseYesGameObj.SetActive(false);
                PauseNoGameObj.SetActive(false);
                PauseResumeGameObj.SetActive(true);
                PauseMainMenuGameObj.SetActive(true);
                PauseQuitGameObj.SetActive(true);
                PauseOptionsGameObj.SetActive(true);
                PauseTextGameObj.SetActive(false);
                quitGame = false;
                returnToMenu = false;
            }
            if (PauseYesBtn.state == ButtonState.OnClick)
            {
                fade.Start();
                if (quitGame)
                    loadQuit = true;
                else if (returnToMenu)
                    loadScene = true;
            }
        }
    }
}

/*!***************************************************************************************
\file			PauseMenu.cs
\project
\author			Abdul Hadi
\co-authors     Shawn Tanary

\par			Course: GAM200
\par			Section:
\date			10/03/2022

\brief
	This file contains the start and update of the pause menu/

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
using CopiumEngine;
using CopiumEngine;
using System;

public class PauseMenu: CopiumScript
{
    public GameObject PauseCanvas;
    public GameObject PauseYesGameObj;
    public GameObject PauseNoGameObj;
    public GameObject PauseResumeGameObj;
    public GameObject PauseMainMenuGameObj;
    public GameObject PauseQuitGameObj;
    public GameObject PauseTextGameObj;
    public Button PauseMainMenuBtn;
    public Button PauseResumeBtn;
    public Button PauseQuitBtn;
    public Button PauseYesBtn;
    public Button PauseNoBtn;
    public Text PauseText;
    public bool isPaused = false;
    public bool returnToMenu = false;
    public bool quitGame = false;
    void Start()
	{

	}
	void Update()
	{
        if (!isPaused)
        {
            if (Input.GetKeyDown(KeyCode.Escape))
            {
                isPaused = true;
            }
        }
        else
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
                PauseQuitGameObj.SetActive(false);
                PauseTextGameObj.SetActive(true);
                PauseText.text = "Are you sure?";
                returnToMenu = true;
            }
            if (PauseQuitBtn.state == ButtonState.OnClick)
            {
                PauseYesGameObj.SetActive(true);
                PauseNoGameObj.SetActive(true);
                PauseResumeGameObj.SetActive(false);
                PauseMainMenuGameObj.SetActive(false);
                PauseQuitGameObj.SetActive(false);
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
                PauseTextGameObj.SetActive(false);
                quitGame = false;
                returnToMenu = false;
            }
             if (PauseYesBtn.state == ButtonState.OnClick)
            {
               if (quitGame)
               {
                Application.Quit();
               }
               else if(returnToMenu)
               {
                SceneManager.LoadScene("MainMenu");
               }
            }
        }

        if (Input.GetKeyDown(KeyCode.P))
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
    }
}

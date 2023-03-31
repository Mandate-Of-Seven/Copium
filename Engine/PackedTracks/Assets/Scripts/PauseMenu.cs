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
    public GameObject PauseOptionsGameObj;
    public GameObject OptionsGameObj;
    public GameObject VolumeSliderGameObj;

    //buttons
    public Button PauseMainMenuBtn;
    public Button PauseResumeBtn;
    public Button PauseQuitBtn;
    public Button PauseYesBtn;
    public Button PauseNoBtn;    
    public Button PauseOptionsBtn;
    public Button PauseAudioOnBtn;
    public Button PauseAudioOffBtn;
    public Button VolumeBtn;
    public Button PauseReturnBtn;

    //text

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
                OptionsGameObj.SetActive(true);
                PauseResumeGameObj.SetActive(false);
                PauseMainMenuGameObj.SetActive(false);
                PauseQuitGameObj.SetActive(false);
                PauseOptionsGameObj.SetActive(false);
                

            }

            if (PauseAudioOffBtn.state == ButtonState.OnClick)
            {
                
            }

            if (VolumeBtn.state ==ButtonState.OnClick)
            {
                moveSlider(Input.GetMousePosition().x);
                
            }

            if (PauseReturnBtn.state == ButtonState.OnClick)
            {
                OptionsGameObj.SetActive(false);
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


    public void moveSlider(float val)
    {
        float percentage = val / 100;
        Console.WriteLine(percentage);
        Vector3 scale = VolumeSliderGameObj.transform.localScale;
        scale.x = percentage;
        VolumeSliderGameObj.transform.localScale = scale;
    }
}

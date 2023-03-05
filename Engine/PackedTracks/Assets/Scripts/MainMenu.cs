using CopiumEngine;
using System;

public class MainMenu: CopiumScript
{
    public AudioSource audio;
    public Button StartGameButton;
    public Button QuitGameButton;

    void Start()
    {
        audio.Play();
    }

    void Update()
    {
        if(StartGameButton.state == ButtonState.OnClick)
        {
            SceneManager.LoadScene("Demo");
        }

        if (QuitGameButton.state == ButtonState.OnClick)
        {
            Application.Quit();
        }
    }
}



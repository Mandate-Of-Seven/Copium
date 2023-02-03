using CopiumEngine;
using System;

public class MainMenu: CopiumScript
{
    public AudioSource audio;
    public Button NewGameButton;
    public Button QuitGameButton;

    void Start()
    {
        audio.Play();
    }

    void Update()
    {
        if (QuitGameButton.state == ButtonState.OnClick)
        {
            Application.Quit();
        }

        if ()
    }
}



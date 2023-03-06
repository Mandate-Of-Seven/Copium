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
            Application.Quit();
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



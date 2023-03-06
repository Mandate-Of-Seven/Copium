using CopiumEngine;
using System;

public class HowtoPlayMenu: CopiumScript
{
    public Text text;
    public Button NextButton;
    public Button PrevButton;
    public GameObject prevButtonObject;

    int page = 1;

    void Start()
	{
		prevButtonObject.SetActive(false);
	}


    void Update()
    {
        if (NextButton.state == ButtonState.OnClick)
        {
            switch (page)
            {
                case 1:
                text.text = "page 2";
                prevButtonObject.SetActive(true);
                page = 2;
                break;

                case 2:
                text.text = "page 3";
                page = 3;
                break;

                case 3:
                SceneManager.LoadScene("MainMenu");
                break;

            }

        }
    }

}
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
        text.text = "train cockpit info goes here";
	}


    void Update()
    {
        if (NextButton.state == ButtonState.OnClick)
        {
            switch (page)
            {
                case 1:
                text.text = "Events go here";
                prevButtonObject.SetActive(true);
                page = 2;
                break;

                case 2:
                text.text = "Stuff about the crew panel goes here";
                page = 3;
                break;

                case 3:
                SceneManager.LoadScene("MainMenu");
                break;

            }

        }
    }

}
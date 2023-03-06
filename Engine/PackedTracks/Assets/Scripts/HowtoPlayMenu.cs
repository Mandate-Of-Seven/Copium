using CopiumEngine;
using System;
using System.Runtime.Remoting.Messaging;
public class HowtoPlayMenu: CopiumScript
{
    public Text text;
    public Button NextButton;
    public Button PrevButton;
    public AudioSource sfx_pageflip;
    public GameObject prevButtonObject;

    int page = 1;

    void Start()
	{
		prevButtonObject.SetActive(false);
        text.text = "Use the mouse to interact with objects in the train cockpit\n " +
                     "An alert will be shown if a new event has occured.\n " +
                     "In the cockpit, there is a meter to show how far you are\n " +
                     "from the destination.";
	}


    void Update()
    {
        if (PrevButton.state == ButtonState.OnClick)
        {   
            sfx_pageflip.Play();
            page--;
            ShowPage(page);
        }
        if (NextButton.state == ButtonState.OnClick)
        {
            sfx_pageflip.Play();
            page++;
            ShowPage(page);
        

        }
    }

    void ShowPage(int _page)
    {
        if (page == 1)
        {
             text.text = "Use the mouse to interact with objects in the train cockpit\n " +
                     "An alert will be shown if a new event has occured.\n " +
                     "In the cockpit, there is a meter to show how far you are\n " +
                     "from the destination.";
                     prevButtonObject.SetActive(false);
        }

        if (page == 2)
        {
            text.text = "Every few kilometers, there will be an event that happens.\n " +
                             "You can go to the event menu from the middle screen in the \n" +
                             "cockpit to make a decision for the event. Your choices can\n" +
                             "have an impact on your journey to your destination.\n";
                prevButtonObject.SetActive(true);
        }

        if (page == 3)
        {
             text.text = "The left panel will show the status of your crew. There are\n"+
                             "three attributes of your crew members to take note of, and\n"  +
                             "their condition can determine whether they live or die.\n\n" +
                             "In addition, you can send your crew out to explore for supplies.\n" +
                             "When exploring, a random event can occur to them, which will\n" +
                             "impact their status.";
        }
        if (page == 4)
        {
              SceneManager.LoadScene("MainMenu");
        }

    }

}
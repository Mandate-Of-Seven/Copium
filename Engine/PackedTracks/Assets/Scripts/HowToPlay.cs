using System.Collections;
using System.Collections.Generic;
using System;
using CopiumEngine;
using static GameData;

public class HowToPlay : CopiumScript
{
    public GameObject howToPlayPanel;
    void OpenHowToPlay()
    {
        howToPlayPanel.SetActive(true);
    }

    void CloseHowToPlay()
    {
        howToPlayPanel.SetActive(false);
    }
}


using System.Collections;
using System.Collections.Generic;
using CopiumEngine;
using CopiumEngine.UI;
using System;

public class GameManager : CopiumScript
{
    public string unitName;

    public int healthState = 2;
    // Health state ranges from 0 to 2
    // 2 - Healthy, able to take some hits
    // 1 - Injured, took some hits
    // 0 - Critical, can possibly die

    public int mentalState = 2;
    // Mental state ranges from 0 to 2
    // 2 - Sound, haven't encountered anything too bad
    // 1 - Shaken, might trigger some events, increase chance to miss shots
    // 0 - Broken, might trigger some events, might ignore commands totally

    public int hungerState = 1;
    // Hunger state ranges from 0 to 1
    // 1 - Fed, will increase health state next day, chance to increase mental state
    // 0 - Famished, will decrease health state next day, might trigger hunger events

    public void TestFunc()
    {

    }
}
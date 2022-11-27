using System.Collections;
using System.Collections.Generic;
using System;
using CopiumEngine;
using static GameData;

public class ResumeButton : CopiumScript
{
    void ResumeGame()
    {
        GameData.CurrentGameState = GameState.Combat;
    }
}

using System.Collections;
using System.Collections.Generic;
using System;
using CopiumEngine;

using static GameData;

public class Manager : CopiumScript
{

    public GameObject panel;
    public GameObject howToPlay;

    void Start()
    {
        CurrentGameState = GameState.Combat;
        Console.WriteLine("Manager Start");
        panel.SetActive(false);
        howToPlay.SetActive(false);
    }

    void Update()
    {

        if(Input.GetKeyDown(KeyCode.Escape))
        {
            if (CurrentGameState == GameState.Combat)
            {
                Console.WriteLine("Pause Game");
                CurrentGameState = GameState.Menu;
                if (panel != null)
                {
                    //panel.active = true;
                    panel.SetActive(true);
                }
            }
            else
            {
                Console.WriteLine("Unpause Game");
                CurrentGameState = GameState.Combat;
                if (panel != null)
                {
                    //panel.active = false;
                    panel.SetActive(false);
                }
                howToPlay.SetActive(false);
            }
        }

        if(CurrentGameState == GameState.Combat && panel.activeSelf == true)
        {
            panel.SetActive(false);
        }

        // if(panel != null && CurrentGameState == GameState.Menu)
        // {
        //     //Console.WriteLine("bleep");
        //     panel.transform.position += new Vector2(0.1f, 0f);
        // }else
        // {
        //     panel.transform.position += new Vector2(-0.1f, 0f);
        // }



    }



    void ResumeGame()
    {
        Console.WriteLine("clicking resume btn");
        CurrentGameState = GameState.Combat;
        panel.SetActive(false);
    }
}




using System.Collections;
using System.Collections.Generic;
using System;
using CopiumEngine;

using static GameData;

public class Manager : CopiumScript
{

    public GameObject panel;


    void Start()
    {
        Console.WriteLine("Manager Start");
        panel.SetActive(false);
    }

    void Update()
    {

        if(Input.GetKey(KeyCode.P))
        {
            Console.WriteLine("Pause Game");
            CurrentGameState = GameState.Menu;
            if(panel != null)
            {
                //panel.active = true;
                panel.SetActive(true);
            }
        }


        if(Input.GetKey(KeyCode.U))
        {
            Console.WriteLine("Unpause Game");
            CurrentGameState = GameState.Combat;
            if(panel != null)
            {
                //panel.active = false;
                panel.SetActive(false);
            }        
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
}




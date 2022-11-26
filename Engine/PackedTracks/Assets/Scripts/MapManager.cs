using System.Collections;
using System.Collections.Generic;
using CopiumEngine;
using static GameData;
using CopiumEngine.UI;
//using TMPro;

public class MapManager : CopiumScript
{
    Button T2MapTop;
    Button T2MapBot;
    Button T3MapTop;
    Button T3MapBot;

    Text T1;
    Text T2T;
    Text T2B;
    Text T3T;
    Text T3B;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public void SetUpMaps()
    {
        if (MapTier == 0)
        {
            T2MapTop.interactable = true;
            T2MapBot.interactable = true;

            T3MapTop.interactable = false;
            T3MapBot.interactable = false;
        }
        else if (MapTier == 1)
        {
            T2MapTop.interactable = false;
            T2MapBot.interactable = false;

            T3MapTop.interactable = true;
            T3MapBot.interactable = true;
        }
        else if (MapTier > 1)
        {
            T2MapTop.interactable = false;
            T2MapBot.interactable = false;

            T3MapTop.interactable = false;
            T3MapBot.interactable = false;
        }

        if (MapTier == 0)
        {
            T1.text = "You are here";
            T2T.text = "Move here";
            T2B.text = "Move here";
            T3T.text = "";
            T3B.text = "";
        }
        else if (MapTier == 1)
        {
            T1.text = "";

            if (MapChoice == 0)
            {
                T2T.text = "You are here";
                T2B.text = "";
            }
            if (MapChoice == 1)
            {
                T2T.text = "";
                T2B.text = "You are here";
            }
                       
            T3T.text = "Move here";
            T3B.text = "Move here";
        }
        else if (MapTier == 2)
        {
            T1.text = "";
            T2T.text = "";
            T2B.text = "";

            if (MapChoice == 0)
            {
                T3T.text = "You are here";
                T3B.text = "";
            }
            if (MapChoice == 1)
            {
                T3T.text = "";
                T3B.text = "You are here";
            }
        }
    }

    public void NextMap()
    {
        MapTier++;
    }
    public void TopMap()
    {
        MapChoice = 0;
    }
    public void BotMap()
    {
        MapChoice = 1;
    }
}

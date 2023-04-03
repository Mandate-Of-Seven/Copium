/*!***************************************************************************************
\file			mom.cs
\project
\author			Shawn Tanary

\par			Course: GAM250
\par			Section:
\date			03/04/2022

\brief
	This file implements the audio messages from mom

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
using CopiumEngine;
using System;
using System.Collections.Generic;

public class Mom : CopiumScript
{
    public static Mom Instance;

    public AudioSource MomMsg1;
    public AudioSource MomMsg2;
    public AudioSource MomMsg3;
    public AudioSource MomMsg4;

    public GameManager gameManager;

    public GameObject RadioGameObj;
    public GameObject MsgIconGameObj;

    public Button RadioBtn;

    public bool MsgAvail = true;
    public int MsgCounter = 1;

    void Awake()
    {
        Instance = this;
    }

    void Start()
    {

    }

    void Update()
    {
        if (gameManager.distanceLeft <300 && MsgCounter < 2)
        {
            MsgCounter = 2;
            MsgAvail = true;
            MsgIconGameObj.SetActive(true);
        }
        else if (gameManager.distanceLeft < 200 && MsgCounter <3)
        {
            MsgCounter = 3;
            MsgAvail = true;
            MsgIconGameObj.SetActive(true);
        }
        else if (gameManager.distanceLeft < 100 && MsgCounter < 4)
        {
            MsgCounter = 4;
            MsgAvail = true;
            MsgIconGameObj.SetActive(true);
        }

        if (RadioBtn.state == ButtonState.OnClick && MsgAvail)
        {
            PlayMsg();
        }
    }

    public void PlayMsg()
    {
        switch (MsgCounter)
        {
            case 1:
                MomMsg1.Play();
                break;
            case 2:
                MomMsg2.Play();
                break;
            case 3:
                MomMsg3.Play();
                break;
            case  4:
                MomMsg4.Play();
                break;
            default:
                break;
        }

        MsgAvail = false;
        MsgIconGameObj.SetActive(false);
    }

    public void StopMsg()
    {
        MomMsg1.Stop();
        MomMsg2.Stop();
        MomMsg3.Stop();
        MomMsg4.Stop();
    }
}

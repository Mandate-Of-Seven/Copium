/*!***************************************************************************************
\file			BackstoryMenu.cs
\project
\author			Wu Ze Zheng
\co-author		Abdul Hadi
                Matthew Lau

\par			Course: GAM200
\par			Section:
\date			09/03/2023

\brief
	Functionality for the HowtoPlay Menu

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
using CopiumEngine;
using System;
using System.Runtime.Remoting.Messaging;
public class BackstoryMenu: CopiumScript
{
    public AudioSource audio;

    void Start()
	{
        Console.WriteLine(InternalCalls.GetSoundLength(audio.ID));
	}


    void Update()
    {
      

    }


}
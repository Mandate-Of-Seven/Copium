/*!***************************************************************************************
\file			Event_Intro.cs
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			10/03/2023

\brief
	This script has the introduction of the events in the game.

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/
using CopiumEngine;
using System;
using System.Runtime.Remoting.Messaging;

public class Event_Intro: CopiumScript
{
	public void Event()
	{
		EventManager.Instance.WriteToBody(Messages.EventIntro.body);
		EventManager.Instance.Option_01.Disable();
		EventManager.Instance.Option_02.Disable();
		EventManager.Instance.Option_03.Disable();
	}
}

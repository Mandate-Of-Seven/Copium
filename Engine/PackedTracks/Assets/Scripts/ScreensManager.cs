/*!***************************************************************************************
\file			ScreensManager.cs
\project
\author			Shawn Tanary
\co-author		Wisely

\par			Course: GAM200
\par			Section:
\date			26/01/2023

\brief
	manages the screens

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
using System.Collections;
using System.Collections.Generic;
using CopiumEngine;
using System;

public class ScreensManager : CopiumScript
{
    public GameObject screenCanvas;
    public GameObject screenCanvasButton;

    public GameObject storyCanvas;
    public GameObject storyCanvasButton;

    public GameObject combatCanvas;
    public GameObject combatCanvasButton;

	/**************************************************************************/
	/*!
	    \brief
		    Toggle story (deprecated)
	*/
	/**************************************************************************/
    public void ToggleStory()
    {
        screenCanvas.SetActive(!screenCanvas.activeSelf);
        screenCanvasButton.SetActive(!screenCanvasButton.activeSelf);

        storyCanvas.SetActive(!storyCanvas.activeSelf);
        storyCanvasButton.SetActive(!storyCanvasButton.activeSelf);

        Debug.Log("Clicked");
    }

	/**************************************************************************/
	/*!
	    \brief
		    Toggle Combat (deprecated)
	*/
	/**************************************************************************/
    public void ToggleCombat()
    {
        screenCanvas.SetActive(!screenCanvas.activeSelf);
        screenCanvasButton.SetActive(!screenCanvasButton.activeSelf);

        combatCanvas.SetActive(!combatCanvas.activeSelf);
        combatCanvasButton.SetActive(!combatCanvasButton.activeSelf);

        Debug.Log("Clicked");
    }
}
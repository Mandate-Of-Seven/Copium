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

    public void ToggleStory()
    {
        screenCanvas.SetActive(!screenCanvas.activeSelf);
        screenCanvasButton.SetActive(!screenCanvasButton.activeSelf);

        storyCanvas.SetActive(!storyCanvas.activeSelf);
        storyCanvasButton.SetActive(!storyCanvasButton.activeSelf);

        Debug.Log("Clicked");
    }

    public void ToggleCombat()
    {
        screenCanvas.SetActive(!screenCanvas.activeSelf);
        screenCanvasButton.SetActive(!screenCanvasButton.activeSelf);

        combatCanvas.SetActive(!combatCanvas.activeSelf);
        combatCanvasButton.SetActive(!combatCanvasButton.activeSelf);

        Debug.Log("Clicked");
    }
}
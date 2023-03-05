using CopiumEngine;
using System;

public class Prepare : CopiumScript
{
    public Button returnButton;

    public GameObject crewCanvas;


    void Start()
    {

    }
    void Update()
    {
        if (returnButton.state == ButtonState.OnRelease)
        {
            crewCanvas.SetActive(true);
            gameObject.SetActive(false);
        }
    }
}
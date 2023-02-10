using CopiumEngine;
using System;

public class FPS: CopiumScript
{
    public Text textBox;
    void Start()
    {
    }

	void Update()
	{
        string fps = "FPS: " + Application.GetFPS().ToString();
        textBox.text = fps;
    }
}

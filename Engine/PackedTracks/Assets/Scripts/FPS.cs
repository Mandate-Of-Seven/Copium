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
        if (Input.GetKeyDown(KeyCode.GraveAccent))
        {
            textBox.enabled = !textBox.enabled;
        }
        textBox.text = fps;
    }
}

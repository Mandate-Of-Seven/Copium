/*!***************************************************************************************
\file			FPS.cs
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			10/3/2023

\brief
	Displays fps to a text component, toggle with grave key

All content ? 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

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

/*!***************************************************************************************
\file			HowtoPlayMenu.cs
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			10/03/2023

\brief
	Display options based on whether they are enabled or disabled along with 
    different colors on the report screen

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

using CopiumEngine;
using System;

public class Option: CopiumScript
{
    public Text txt;
    public Button btn;
    [NonSerialized]
    public Choice mappedChoice;

	public StatusUpdate statusUpdate;
    Image image;
    Color disabled = new Color(1.0f, 1.0f, 1.0f, 0.5f); 
    Color enabled = new Color(1.0f, 1.0f, 1.0f, 1.0f);

    public ButtonWrapper btnWrapper;

    void Start()
	{
        btnWrapper = new ButtonWrapper(btn);
        btnWrapper.SetImage(btn.GetComponent<Image>());

        image = gameObject.GetComponent<Image>();
        enabled = image.color;
	}
	void Update()
	{
        if (!btn.enabled)
            image.color = disabled;
    }

    public bool Hovered()
    {
        if (btn.state == ButtonState.OnHover)
            return true;
        return false;
    }

    public void Enable()
    {
        if(!gameObject.activeSelf)
            gameObject.SetActive(true);
    }

    public void Disable()
    {
        if(gameObject.activeSelf)
            gameObject.SetActive(false);
    }

    public void ResetOption()
    {
        Disable();
        btn.enabled = true;
        image.color = enabled;
        txt.color = Color.white;
    }
}

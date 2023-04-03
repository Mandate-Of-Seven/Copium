/*!***************************************************************************************
\file			ButtonWrapper.cs
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			10/3/2023

\brief
	This file contains a wrapper struct to help play default sound effects that can
	be overwritten. Also helps display to the tooltip and handles disabled effects and
	behaviour

All content ? 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

using CopiumEngine;
using System;
using System.Security.Cryptography;

public class ButtonWrapper
{
	public Color disabledImageColor = new Color(0.8f,0.0f,0.0f,1f);
	public Color disabledTextColor = new Color(0.5f,0.2f,0.2f,1f);

	public Color disabledHoverColor = new Color(0.5f,0.0f,0.0f,0.5f);
	public Color disabledClickedColor = new Color(0.2f,0.0f,0.0f,0.5f);

	public bool useDisabled = true;
	public bool useHoverDisabled = true;
	Color imageOriginalColor;
	Color textOriginalColor;
	Color buttonOriginalHoverColor;
	Color buttonOriginalClickedColor;

    

	public string failureText;

	public ButtonWrapper(Button _button)
	{
	 	button = _button;
		buttonOriginalHoverColor = button.hoverColor;
		buttonOriginalClickedColor = button.clickedColor;
		interactable = true;
	}

	Button button;
	Image image;
	Text text;

	public AudioSource hoverSFX;
	public AudioSource clickedSFX;
	public AudioSource releasedSFX;
	public AudioSource failedSFX;

	bool hoverable = false;

	bool interactable;

	public void SetText(Text _text)
	{
		text = _text;
		if (text != null)
		{
			textOriginalColor = text.color;
		}
	}

	public void SetImage(Image _image)
	{
		image = _image;
		if (image != null)
		{
			imageOriginalColor = image.color;
		}
	}

	public void SetInteractable(bool _interactable)
	{
		if (interactable == _interactable)
			return;
		interactable = _interactable;
		if (interactable)
		{
			if (useDisabled)
			{
				if (image != null)
				{
					image.color = imageOriginalColor;
				}
				if (text != null)
				{
					text.color = textOriginalColor;
				}
			}
			if (useHoverDisabled)
			{
				button.hoverColor = buttonOriginalHoverColor;
				button.clickedColor = buttonOriginalClickedColor;
			}
		}
		else
		{
			if (useDisabled)
			{
				if (image != null)
				{
					image.color = disabledImageColor;
				}
				if (text != null)
				{
					text.color = disabledTextColor;
				}
			}
			if (useHoverDisabled)
			{
				button.hoverColor = disabledHoverColor;
				button.clickedColor = disabledClickedColor;
			}
		}
	}

	public ButtonState GetState()
	{
        if (button.state == ButtonState.OnHover)
        {
			if (hoverable)
			{
				hoverable = false;
				if (hoverSFX  != null)
				{
					hoverSFX.Play();
				}
				else
				{
                    int random = RNG.Range(0, 1);
                    if (random == 0)
                        AudioManager.Instance.hoverSFX.Play();
                    else if (random == 1)
                        AudioManager.Instance.hoverSFX2.Play();
				}
			}
        }
        else if (button.state == ButtonState.OnClick)
        {
			if (!interactable)
			{
				if (failedSFX != null)
				{
					failedSFX.Play();
				}
				else
					AudioManager.Instance.failedSFX.Play();
				if (failureText != null && failureText.Length != 0)
					TooltipBehaviour.Instance.AddText(failureText);
				return ButtonState.None;
			}
			if (clickedSFX != null)
			{
				clickedSFX.Play();
			}
			else
			{
				AudioManager.Instance.clickSFX.Play();
            }
        }
		else if (button.state == ButtonState.OnRelease)
		{
			if (!interactable)
				return ButtonState.None;
			if (releasedSFX != null)
				releasedSFX.Play();
		}
        else if (button.state == ButtonState.None)
        {	
			hoverable = true;
        }
		return button.state;
	}
}

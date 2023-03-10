using CopiumEngine;
using System;

public class ButtonWrapper
{
	public Color disabledImageColor = new Color(1f,0.2f,0.2f,1f);
	public Color disabledTextColor = new Color(0.5f,0.2f,0.2f,1f);

	public bool useDisabled = true;
	Color imageOriginalColor;
	Color textOriginalColor;

	public ButtonWrapper(Button _button, AudioManager _audioManager)
	{
	 	button = _button;
		audioManager = _audioManager;
		interactable = true;
	}

	AudioManager audioManager;
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
		if (!useDisabled)
			return;
		if (interactable)
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
		else
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
					audioManager.hoverSFX.Play();
				}
			}
        }
        else if (button.state == ButtonState.OnClick)
        {
			if (!interactable)
			{
				if (failedSFX != null)
					failedSFX.Play();
				else
					audioManager.failedSFX.Play();
				return ButtonState.None;
			}
			if (clickedSFX != null)
			{
				clickedSFX.Play();
			}
			else
			{
				audioManager.clickSFX.Play();
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

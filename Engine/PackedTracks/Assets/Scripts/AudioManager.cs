using CopiumEngine;
using System;

public class AudioManager: CopiumScript
{
	public AudioSource ambTrain;
	public AudioSource bgmMainMenu;
	public AudioSource bgmTrain;
	public AudioSource bgmCombat;
	public AudioSource alarmGunshot1;
	public AudioSource sfxGunshot1;
	public AudioSource sfxGunshot2;
	public AudioSource sfxGunshot3;
	public AudioSource sfxGunshot4;
	public AudioSource sfxDeath;
	public AudioSource sfxButtonHover;
	public AudioSource sfxButtonClick;
	public int state = 0;

	void Start()
	{
		if (state == 0)
		{
			bgmMainMenu.Play();
		}
		else if (state == 1)
		{
			bgmTrain.Play();
		}
		else if (state == 2)
		{
			bgmCombat.Play();
		}
	}
	void Update()
	{

	}
}

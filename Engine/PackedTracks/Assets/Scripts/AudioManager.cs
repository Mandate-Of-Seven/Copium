using CopiumEngine;
using System;

public class AudioManager: CopiumScript
{
	public AudioSource ambTrain;
	public AudioSource bgmMainMenu;
	public AudioSource bgmTrain;
	public AudioSource clickSFX;
	public AudioSource paperSFX;
	public AudioSource leverSFX;
	public AudioSource accelerateSFX;
	public AudioSource deccelerateSFX;
	public AudioSource leverEngagedSFX;
	public AudioSource hoverSFX;
	public AudioSource explosionSFX;

	
	public AudioSource ending3bVO;
	public int state = 0;

    void Start()
	{
        state = 1;
        bgmTrain.Play();
	}
	void Update()
	{
        
            
    }
}

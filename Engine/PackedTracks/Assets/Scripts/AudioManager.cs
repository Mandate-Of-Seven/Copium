/*!***************************************************************************************
\file			AudioManager.cs
\project
\author			Zacharie Hong
\co-author		Sean Ngo
                Shawn Tanary
				Abdul Hadi
                Matthew Lau

\par			Course: GAM250
\par			Section:
\date			30/01/2023

\brief
	Managed all the audio sources in the scene

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
using CopiumEngine;
using System;

public class AudioManager: CopiumScript
{
	//Singleton
	public static AudioManager Instance;

	public AudioSource ambTrain;
	public AudioSource bgmTrain;
	public AudioSource clickSFX;
	public AudioSource paperSFX;
	public AudioSource fileOpenSFX;
	public AudioSource fileCloseSFX;
	public AudioSource leverSFX;
	public AudioSource accelerateSFX;
	public AudioSource deccelerateSFX;
	public AudioSource leverEngagedSFX;
	public AudioSource autoDoorSFX;
	public AudioSource hoverSFX;
	public AudioSource hoverSFX2;
	public AudioSource gunfireSFX;
	public AudioSource explosionSFX;
	public AudioSource explosionForEvent3SFX;
	public AudioSource failedSFX;
	public AudioSource ending3bVO;
    public AudioSource ending2aVO;
    public AudioSource endingHarrisVO;
    public AudioSource endingBronsonVO;
    public AudioSource endingChuckVO;
    public AudioSource endingDantonVO;
    public AudioSource endingGenericVO;
    public AudioSource endingAllDiedVO;
    public AudioSource endingAllAliveVO;

	void Awake()
	{
		Instance = this;
	}

    void Start()
	{
		if (bgmTrain != null)
			bgmTrain.Play();
	}

	void Update()
	{
        
            
    }
}

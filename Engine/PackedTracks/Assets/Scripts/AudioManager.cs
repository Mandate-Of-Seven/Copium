using CopiumEngine;
using System;

public class AudioManager: CopiumScript
{
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
	public AudioSource hoverSFX;
	public AudioSource explosionSFX;
	public AudioSource failedSFX;
	public AudioSource ending3bVO;
    public AudioSource ending2aVO;
    public AudioSource endingHarrisVO;
    public AudioSource endingBronsonVO;
    public AudioSource endingChuckVO;
    public AudioSource endingDantonVO;
    public AudioSource endingGenericVO;
    public AudioSource endingAllDiedVO;



    void Start()
	{
        bgmTrain.Play();
	}
	void Update()
	{
        
            
    }
}

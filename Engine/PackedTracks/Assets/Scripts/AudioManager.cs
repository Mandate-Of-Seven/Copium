using CopiumEngine;
using System;

public class AudioManager: CopiumScript
{
	public AudioSource ambTrain;
	public AudioSource bgmMainMenu;
	public AudioSource bgmTrain;
	//public AudioManager audioManager;
	public int state = 0;

    public GameObject MainMenuStartGameObject;
    Button MainMenuStartBtn;

    void Start()
	{
		Console.WriteLine("HELLO FROM C#");
		// audioManager = gameObject.GetComponent<AudioManager>();
		Console.WriteLine("HELLO FROM C#");
        //MainMenuStartBtn = MainMenuStartGameObject.GetComponent<Button>();
        /*        if (state == 0)
				{
					bgmMainMenu.Play();
				}
				else if (state == 1)
				{
					Console.WriteLine("PLAYING STATE 1 AUDIO MANAGER");
					bgmTrain.Play();
					ambTrain.Play();
				}
				else if (state == 2)
				{
					bgmCombat.Play();
				}*/
        state = 1;

        bgmTrain.Play();
        ambTrain.Play();
    }
	void Update()
	{
        
            
    }
}

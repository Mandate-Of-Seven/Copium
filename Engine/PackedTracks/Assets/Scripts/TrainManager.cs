/*!***************************************************************************************
\file			TrainManager.cs
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			04/03/2023

\brief
	Manages the train screen, which includes all canvases, and also plays sound effects, 
	camera effects, visual effects when the train is moving to show a slowing down and 
	speed up effect

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
using CopiumEngine;
using System;
using System.Security.AccessControl;

public class TrainManager: CopiumScript
{
	public GameManager GameManager;

	public float maxSpeed = 100.0f;
	public float currentSpeed = 0.0f;
	public float acceleration = 0.0f;
	public float deceleration = 0.0f;
	public Animator snowAnimator;
	public Animator tracksAnimator;
	public AudioManager audioManager;

	public Button trainLeverBtn;
	public Image trainLeverActivated;
	public Image trainLeverDeactivated;
    public float levelTransSpeed = 2.0f;
	Color leverHoverColor = new Color(0.6f,0.6f,0.6f,1f);

    public Text tracker;
    public Button ManualBtn;
    public bool accelerate = false;
	bool manualHover = false;
	bool onHover = false;
	
	public float snowMaxDelay = 0.1f;
	public float tracksMaxDelay = 0.1f;
	public float snowMinDelay = 0.01f;
	public float tracksMinDelay = 0.01f;

	public float shakeRotation;
	public float shakePosition;

	float initialSnowScale;
	public float targetSnowScale;

	private Vector3 targetRotation = new Vector3(0,0,0);
	private Vector3 targetPosition = new Vector3(0,0,0);
	private Vector3 targetScale = new Vector3(1,1,1);
	private float targetSnowDelay;
	private float targetTracksDelay;

	public float zoomOutScale;
	public float zoomInScale;

	float targetAmbienceVolume = 0.0f;

	public GameObject cabinBtn;


	void Start()
	{
		targetSnowDelay = snowMaxDelay;
		targetTracksDelay = tracksMaxDelay;
		initialSnowScale = snowAnimator.gameObject.transform.localScale.y;
	}

	void Update()
	{
        //ToggleManual();

		ToggleLever();
		
		// Speed of train
		if (accelerate && currentSpeed < maxSpeed)
		{
			currentSpeed += acceleration* Time.deltaTime;
		}
		else if (currentSpeed > 0.0f)
		{
			currentSpeed -= deceleration * Time.deltaTime;
		}

		float ratio = (currentSpeed/maxSpeed);

		

		if (!accelerate && ratio <= 0.2f)
		{
			audioManager.accelerateSFX.Stop();
			audioManager.deccelerateSFX.Stop();
			tracksAnimator.play = false;
			targetPosition = new Vector3(0,0,0);
			targetRotation = new Vector3(0,0,0);
			targetScale = new Vector3(1,1,1);
		}
		else if (!accelerate && ratio <= 0.4f)
		{
			audioManager.ambTrain.Stop();
			float shakePosX = RNG.Range(-shakePosition,shakePosition);
			float shakePosY = RNG.Range(0,shakePosition*2);
			targetPosition = new Vector3(shakePosX,shakePosY,0);
			float shakeRot = RNG.Range(-shakeRotation,shakeRotation);
			targetRotation = new Vector3(0,0,shakeRot);
			targetScale = new Vector3(1,1,1);
		}
		else
		{
			float shakeRot = RNG.Range(-shakeRotation,shakeRotation);
			float shakePosY = RNG.Range(-shakePosition,shakePosition);
			float shakePosX = RNG.Range(-shakePosition,shakePosition);
			tracksAnimator.play = true;
			targetPosition = new Vector3(shakePosX,shakePosY,0);
			targetRotation = new Vector3(0,0,shakeRot);
		}
		float timeStep = Time.deltaTime* ratio;
		tracksAnimator.delay = 
			Mathf.Lerp(tracksAnimator.delay,targetTracksDelay,timeStep);
		snowAnimator.delay = 
			Mathf.Lerp(snowAnimator.delay,targetSnowDelay,timeStep);
		if (!accelerate && ratio <= 0.1f)
		{
        	audioManager.leverEngagedSFX.Stop();
			transform.localScale = 
				Vector3.Lerp(transform.localScale,targetScale,Time.deltaTime * 2.0f);
			Vector3 snowScale = snowAnimator.gameObject.transform.localScale;
			Vector3 newSnowScale = snowScale;
			newSnowScale.y = initialSnowScale;
			snowAnimator.gameObject.transform.localScale = 
				Vector3.Lerp(snowScale,newSnowScale,Time.deltaTime * 2.0f);
		}
		else 
		{
			transform.localScale = 
				Vector3.Lerp(transform.localScale,targetScale,timeStep);
			Vector3 snowScale = snowAnimator.gameObject.transform.localScale;
			Vector3 newSnowScale = snowScale;
			newSnowScale.y = targetSnowScale;
			snowAnimator.gameObject.transform.localScale = 
				Vector3.Lerp(snowScale,newSnowScale,timeStep);
		}
		if (ratio >= 0.3f)
		{
			audioManager.ambTrain.volume = 
				Mathf.Lerp(audioManager.ambTrain.volume,targetAmbienceVolume,timeStep);
			transform.localRotation = 
				Vector3.Lerp(Vector3.zero,targetRotation,timeStep);
			transform.localPosition = 
				Vector3.Lerp(Vector3.zero,targetPosition,timeStep);
		}

		// Update the distance tracker
        tracker.text = ((int)GameManager.distanceLeft).ToString() + "KM";
    }

	/**************************************************************************/
	/*!
	    \brief
		    Toggle the lever. 
	*/
	/**************************************************************************/
	void ToggleLever()
	{
        if (trainLeverBtn.state == ButtonState.OnHover)
        {
            if (!onHover)
            {
                audioManager.hoverSFX.Play();
                onHover = true;
            }
            Color leverCurrentColor = Color.Lerp(trainLeverActivated.color, Color.white, Time.deltaTime * levelTransSpeed);
            trainLeverActivated.color = leverCurrentColor;
            trainLeverDeactivated.color = leverCurrentColor;
        }
        else if (trainLeverBtn.state == ButtonState.OnClick)
        {
            FlickLever();
        }
        else 
        {
            if (trainLeverBtn.state == ButtonState.None)
                onHover = false;
            Color leverCurrentColor = Color.Lerp(trainLeverActivated.color, leverHoverColor, Time.deltaTime * levelTransSpeed);
            trainLeverActivated.color = leverCurrentColor;
            trainLeverDeactivated.color = leverCurrentColor;
        }
    }

	/**************************************************************************/
	/*!
	    \brief
			Toggle the lever and depending on the state of it, train stops/starts
	*/
	/**************************************************************************/
	public void FlickLever()
	{
		if (GameManager.gameEnd)
			return;

        accelerate = !accelerate;
        trainLeverActivated.gameObject.SetActive(accelerate);
        trainLeverDeactivated.gameObject.SetActive(!accelerate);
        audioManager.accelerateSFX.Stop();
        audioManager.leverSFX.Stop();
        audioManager.leverSFX.Play();
        audioManager.leverEngagedSFX.Play();
        if (accelerate)
        {
			StartTrain();
			targetAmbienceVolume = 1.0f;
        }
        else
        {
			StopTrain();
			targetAmbienceVolume = 0.0f;
        }
    }

	/**************************************************************************/
	/*!
	    \brief
			Toggle the lever and depending on the state of it, train stops/starts

		\param _accelerate
			flag to determine if train should accelerate or not
	*/
	/**************************************************************************/
    public void FlickLever(bool _accelerate)
    {
		if (accelerate == _accelerate)
			return;

        accelerate = _accelerate;
        trainLeverActivated.gameObject.SetActive(accelerate);
        trainLeverDeactivated.gameObject.SetActive(!accelerate);
        audioManager.accelerateSFX.Stop();
        audioManager.leverSFX.Stop();
        audioManager.leverSFX.Play();
        audioManager.leverEngagedSFX.Play();
        if (accelerate)
        {
            StartTrain();
            targetAmbienceVolume = 1.0f;
        }
        else
        {
            StopTrain();
            targetAmbienceVolume = 0.0f;
        }
    }

	/**************************************************************************/
	/*!
	    \brief
			Starts the train
	*/
	/**************************************************************************/
    void StartTrain()
	{       
		audioManager.ambTrain.Stop();
        audioManager.ambTrain.Play();
		audioManager.deccelerateSFX.Stop();
		audioManager.accelerateSFX.Play();
		targetSnowDelay = snowMinDelay;
		targetTracksDelay = tracksMinDelay;
		targetScale = new Vector3(zoomOutScale,zoomOutScale,1);

		cabinBtn.SetActive(false);
	}

	/**************************************************************************/
	/*!
	    \brief
			Stop the train
	*/
	/**************************************************************************/
	void StopTrain()
	{
		audioManager.accelerateSFX.Stop();
		audioManager.deccelerateSFX.Play();
		targetSnowDelay = snowMaxDelay;
		targetTracksDelay = tracksMaxDelay;
		targetScale = new Vector3(zoomInScale,zoomInScale,1);

		cabinBtn.SetActive(true);
	}

	/**************************************************************************/
	/*!
	    \brief
			Check if train is accelerating or not
	*/
	/**************************************************************************/
	public bool IsAccelerating()
	{
		return accelerate;
	}
	// Toggle the manual to open or close
    // void ToggleManual()
    // {
    //     if (!ManualPopUpBtn.gameObject.activeSelf) // When manual is not open
    //     {
    //         if (ManualBtn.state == ButtonState.OnHover)
    //         {
    //             if (!manualHover)
    //             {
    //                 manualHover = true;
    //                 audioManager.hoverSFX.Play();
    //             }
    //         }
    //         else if (ManualBtn.state == ButtonState.OnRelease)
    //         {
    //             audioManager.paperSFX.Play();
    //             ManualPopUpBtn.gameObject.SetActive(true);
    //             manualHover = true;
    //         }
    //         else if (ManualBtn.state == ButtonState.None)
    //         {
    //             manualHover = false;
    //         }
    //     }
    //     else
    //     {
    //         if (ManualPopUpBtn.state == ButtonState.OnHover)
    //         {
    //             if (!manualHover)
    //             {
    //                 manualHover = true;
    //                 audioManager.hoverSFX.Play();
    //             }
    //         }
    //         else if (ManualPopUpBtn.state == ButtonState.OnRelease)
    //         {
    //             audioManager.paperSFX.Play();
    //             ManualPopUpBtn.gameObject.SetActive(false);
    //             manualHover = true;
    //         }
    //         else if (ManualPopUpBtn.state == ButtonState.None)
    //         {
    //             manualHover = false;
    //         }
    //     }
    // }
}

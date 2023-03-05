using CopiumEngine;
using System;

public class TrainManager: CopiumScript
{
	public float maxSpeed = 1.0f;
	public float currentSpeed = 0.0f;
	public float acceleration = 0.0f;
	public float deceleration = 0.0f;
	public Animator snowAnimator;
	public Animator tracksAnimator;
	public AudioManager audioManager;

	public Button trainLeverBtn;
	public GameObject trainLeverActivated;
	public GameObject trainLeverDeactivated;
	public GameObject trainCanvas;
	
	bool accelerate = false;
	
	public float snowMaxDelay = 0.1f;
	public float tracksMaxDelay = 0.1f;
	public float snowMinDelay = 0.01f;
	public float tracksMinDelay = 0.01f;

	public float shakeRotation;
	public float shakePosition;

	private Vector3 targetRotation = new Vector3(0,0,0);
	private Vector3 targetPosition = new Vector3(0,0,0);
	private Vector3 targetScale = new Vector3(1,1,1);
	private float targetSnowDelay;
	private float targetTracksDelay;

	public float zoomOutScale;
	public float zoomInScale;

	void Start()
	{
		targetSnowDelay = snowMaxDelay;
		targetTracksDelay = tracksMaxDelay;
	}

	void Update()
	{
		if (trainLeverBtn.state == ButtonState.OnClick)
		{
			accelerate = !accelerate;
			trainLeverActivated.SetActive(accelerate);
			trainLeverDeactivated.SetActive(!accelerate);
			audioManager.accelerateSFX.Stop();
			audioManager.leverSFX.Stop();
			audioManager.leverSFX.Play();
			if (accelerate)
			{
				StartTrain();
			}
			else
			{
				StopTrain();
			}
		}
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
		if (ratio <= 0.1f)
		{
			trainCanvas.transform.localScale = 
				Vector3.Lerp(trainCanvas.transform.localScale,targetScale,Time.deltaTime * 2.0f);
		}
		else
		{
			trainCanvas.transform.localScale = 
				Vector3.Lerp(trainCanvas.transform.localScale,targetScale,timeStep);
		}
		trainCanvas.transform.localRotation = 
			Vector3.Lerp(Vector3.zero,targetRotation,timeStep);
		trainCanvas.transform.localPosition = 
			Vector3.Lerp(Vector3.zero,targetPosition,timeStep);
	}

	void StartTrain()
	{
		audioManager.deccelerateSFX.Stop();
		audioManager.accelerateSFX.Play();
		targetSnowDelay = snowMinDelay;
		targetTracksDelay = tracksMinDelay;
		targetScale = new Vector3(zoomOutScale,zoomOutScale,1);
	}

	void StopTrain()
	{
		audioManager.accelerateSFX.Stop();
		audioManager.deccelerateSFX.Play();
		targetSnowDelay = snowMaxDelay;
		targetTracksDelay = tracksMaxDelay;
		targetScale = new Vector3(zoomInScale,zoomInScale,1);
	}
}

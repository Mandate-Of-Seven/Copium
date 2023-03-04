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
			if (accelerate)
			{
				targetSnowDelay = snowMinDelay;
				targetTracksDelay = tracksMinDelay;
			}
			else
			{
				targetSnowDelay = snowMaxDelay;
				targetTracksDelay = tracksMaxDelay;
			}
		}
		if (accelerate)
		{
			if (currentSpeed < maxSpeed)
			{
				currentSpeed += acceleration* Time.deltaTime;
			}
		}
		else
		{
			if (currentSpeed > 0.0f)
			{
				currentSpeed -= deceleration * Time.deltaTime;
			}
		}

		float ratio = (currentSpeed/maxSpeed);


		if (ratio <= 0.1f)
		{
			tracksAnimator.play = false;
			targetScale = new Vector3(1,1,1);
			targetPosition = new Vector3(0,0,0);
			targetRotation = new Vector3(0,0,0);
		}
		else if (!accelerate && ratio <= 0.4f)
		{
			float shakePosX = RNG.Range(-shakePosition,shakePosition);
			float shakePosY = RNG.Range(0,shakePosition*2);
			targetPosition = new Vector3(shakePosX,shakePosY,0);
			float shakeRot = RNG.Range(-shakeRotation,shakeRotation);
			targetRotation = new Vector3(0,0,shakeRot);
		}
		else
		{
			float shakeRot = RNG.Range(-shakeRotation,shakeRotation);
			float shakePosY = RNG.Range(-shakePosition,shakePosition);
			float shakePosX = RNG.Range(-shakePosition,shakePosition);
			tracksAnimator.play = true;
			if (accelerate)
			{
				targetScale = new Vector3(zoomOutScale,zoomOutScale,1);
			}
			else 
			{
				targetScale = new Vector3(1,1,1);
			}
			targetPosition = new Vector3(shakePosX,shakePosY,0);
			targetRotation = new Vector3(0,0,shakeRot);
		}
		float timeStep = Time.deltaTime* ratio;
		tracksAnimator.delay = 
			Mathf.Lerp(tracksAnimator.delay,targetTracksDelay,timeStep);
		snowAnimator.delay = 
			Mathf.Lerp(snowAnimator.delay,targetSnowDelay,timeStep);
		trainCanvas.transform.localScale = 
			Vector3.Lerp(trainCanvas.transform.localScale,targetScale,timeStep);
		trainCanvas.transform.localRotation = 
			Vector3.Lerp(Vector3.zero,targetRotation,timeStep);
		trainCanvas.transform.localPosition = 
			Vector3.Lerp(Vector3.zero,targetPosition,timeStep);
	}
}

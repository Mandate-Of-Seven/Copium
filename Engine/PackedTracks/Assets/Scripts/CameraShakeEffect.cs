using CopiumEngine;
using System;

public class CameraShakeEffect: CopiumScript
{
	public GameObject trainCanvas;
	public float intensity;
	public float timeElasped;
	public float interval = 0.1f;
	public float totalDuration;
	float intervalTimer = 0;
	bool playing = false;
	Vector3 targetPos;

	public void Trigger()
	{
		playing = true;
		Randomize();
	}

	void Reset()
	{
		trainCanvas.transform.position = Vector3.zero;
		playing = false;
		timeElasped = 0;
		intervalTimer = 0;
	}

	void Randomize()
	{
		targetPos = Vector3.zero;
		targetPos.x = RNG.Range(-intensity,intensity);
		targetPos.y = RNG.Range(-intensity,intensity);
	}

	void Start()
	{

	}
	void Update()
	{
		
		if (!playing)
			return;
		if (timeElasped > totalDuration) 
			Reset();
		
		if (intervalTimer > interval)
		{
			Randomize();
			intervalTimer = 0;
		}
		trainCanvas.transform.position = Vector3.Lerp(Vector3.zero,targetPos,intervalTimer/interval);
		intervalTimer += Time.deltaTime;
		timeElasped += Time.deltaTime;
	}
}

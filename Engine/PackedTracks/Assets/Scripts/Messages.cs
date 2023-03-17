using CopiumEngine;
using System;

public class Messages: CopiumScript
{
	public static Messages Instance;
	[System.NonSerialized]public string ErrorTrainMoving = "";
	[System.NonSerialized]public string ErrorMainEvent = "You need to handle the event first!";

	[System.NonSerialized]public string  PreEvent01 = "There is a faint smell of something burning in the air...";

	void Awake()
	{
		Instance = this;
	}

	void Start()
	{
	}
	void Update()
	{

	}
}

using CopiumEngine;
using System;

public class GameManager: CopiumScript
{
	public GameObject TrainCanvas;
	public GameObject MainScreenCanvas;
	public GameObject CombatCanvas;

	void Start()
	{
		TrainCanvas.SetActive(true);
		MainScreenCanvas.SetActive(false);
		CombatCanvas.SetActive(false);
	}
	void Update()
	{

	}
}

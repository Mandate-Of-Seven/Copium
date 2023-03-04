using CopiumEngine;
using System;

public class Test: CopiumScript
{
    double temp = 0.075;
    void Start()
	{
		
    }
	void Update()
	{
        if (Input.GetKeyDown(KeyCode.O))
        {
            Console.WriteLine("Play");
            InternalCalls.PlayAnimation(gameObject.ID);
        }

        if (Input.GetKeyDown(KeyCode.P))
        {
            Console.WriteLine("Pause");
            InternalCalls.PauseAnimation(gameObject.ID);
        }
        if (Input.GetKeyDown(KeyCode.L))
        {
            temp += 0.001;
            InternalCalls.SetAnimationSpeed(gameObject.ID, temp);
        }
    }
}

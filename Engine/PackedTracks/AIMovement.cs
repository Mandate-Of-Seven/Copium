using System.Collections;
using System.Collections.Generic;
using CopiumEngine;
using System;

public class AIMovement : CopiumScript
{
    enum AIState { Idle, Chase, Evade }
    AIState currentAIState = AIState.Idle;

    public float TrainSpeed = 0;
    public float MaxTrainSpeed = 1;
    public float TrainAcc = 0.01f;

    GameObject PlayerTrainGO;
    float AITimer = 0.5f;
    // Start is called before the first frame update
    void Start()
    {
        PlayerTrainGO = GameObject.FindByName("PlayerTrain");
    }

    // Update is called once per frame
    void Update()
    {
        Vector3 pos = transform.position;
        Vector3 waypoint = PlayerTrainGO.transform.position;
        if (Math.Abs(pos.x - waypoint.x) > 0.5f)
        {
            if (transform.position.x < waypoint.x)
            {
                if (TrainSpeed < MaxTrainSpeed)
                {
                    TrainSpeed += TrainAcc;
                }
                else
                {
                    TrainSpeed = MaxTrainSpeed;
                }
            }
            else if (transform.position.x > waypoint.x)
            {
                if (-TrainSpeed < MaxTrainSpeed)
                {
                    TrainSpeed -= TrainAcc;
                }
                else
                {
                    TrainSpeed = -MaxTrainSpeed;
                }
            }
        }
        else
        {
            TrainSpeed *= 0.97f;
        }
        if (pos.x < -4.5f)
        {
            TrainSpeed /= 2f;
            pos.x = -4.5f;
        }
        else if (pos.x > 7.0f)
        {
            TrainSpeed /= 2f;
            pos.x = 7.0f;
        }

        pos.x += TrainSpeed;
        transform.position = pos;
    }
}

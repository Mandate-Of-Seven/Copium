using System.Collections;
using System.Collections.Generic;
using CopiumEngine;

public class CombatMovement : CopiumScript
{

    public float TrainSpeed = 0;
    public float MaxTrainSpeed = 1;
    public float TrainAcc = 0.01f;

    bool canMoveLeft = true;
    bool canMoveRight = true;
    
    // Start is called before the first frame update
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {
        bool inputLeft = false;
        bool inputRight = false;

        Vector3 pos = transform.position;
        if (Input.GetKey(KeyCode.A))
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
        else if (Input.GetKey(KeyCode.D))
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
        else
        {
            TrainSpeed *= 0.97f;
        }
        pos.x += TrainSpeed;
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
        transform.position = pos;
    }
}

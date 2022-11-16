using System.Collections;
using System.Collections.Generic;
using System;
using CopiumEngine;
using static GameData;

public class CombatMovement : CopiumScript
{
    Rigidbody2D rb;

    public Vector3 hello;

    bool inputLeft = false;
    bool inputRight= false;

    bool movingLeft = false;
    bool movingRight = false;

    bool canMoveLeft = true;
    bool canMoveRight= true;

    // Start is called before the first frame update
    void Start()
    {
        rb = GetComponent<Rigidbody2D>();
        CurrentGameState = GameState.Combat;
    }

    // Update is called once per frame
    void Update()
    {
        Console.WriteLine("HELLOWRODL");
        if (Input.GetKey(KeyCode.Escape))
        {
            Application.Quit();
        }

        if (CurrentGameState == GameState.Combat)
        {
            if (Input.GetKey(KeyCode.A))
            {
                inputLeft = true;

                if (canMoveLeft)
                {
                    movingLeft = true;
                    canMoveRight = false;
                }

            }
            if (Input.GetKey(KeyCode.D))
            {
                inputRight = true;

                if (canMoveRight)
                {
                    movingRight = true;
                    canMoveLeft = false;
                }
            }

            if (Input.GetKeyUp(KeyCode.A))
            {
                inputLeft = false;
            }
            if (Input.GetKeyUp(KeyCode.D))
            {
                inputRight = false;
            }
        }
    }

    private void FixedUpdate()
    {
        if (CurrentGameState != GameState.Combat)
        {
            rb.velocity = rb.velocity * 0f;
            inputLeft = false;
            inputRight = false;

            movingLeft = false;
            movingRight = false;

            canMoveLeft = true;
            canMoveRight = true;
        }
        if (transform.position.x < -4.5)
        {
            rb.velocity = rb.velocity * 0f;
        }
        if (transform.position.x > 7.5)
        {
            rb.velocity = rb.velocity * 0f;
        }

        if (movingLeft && inputRight || movingRight && inputLeft || inputLeft && inputRight)
        {
            if (TrainSpeed > 1)
            {
                TrainSpeed = 1;
            }

            rb.velocity = rb.velocity * 0.9f;

            if (rb.velocity.x < 0.5f && rb.velocity.x > -0.5f)
            {
                TrainSpeed = 1;

                movingLeft = false;
                movingRight = false;

                canMoveLeft = true;
                canMoveRight = true;
            }
        }
        else if (movingLeft && inputLeft)
        {
            rb.velocity = new Vector2(-TrainSpeed, 0);

            if (TrainSpeed < MaxTrainSpeed)
            {
                TrainSpeed += TrainAcc;
            }
        }
        else if (movingRight && inputRight)
        {
            rb.velocity = new Vector2(TrainSpeed, 0);

            if (TrainSpeed < MaxTrainSpeed)
            {
                TrainSpeed += TrainAcc;
            }
        }
        else if (!inputLeft && !inputRight)
        {
            if (TrainSpeed > 1)
            {
                TrainSpeed = 1;
            }

            rb.velocity = rb.velocity * 0.97f;

            if (rb.velocity.x < 0.05f && rb.velocity.x > -0.05f)
            {
                TrainSpeed = 1;

                movingLeft = false;
                movingRight = false;

                canMoveLeft = true;
                canMoveRight = true;
            }
        }
    }
}

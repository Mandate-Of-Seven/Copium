using System.Collections;
using System.Collections.Generic;
using System;
using CopiumEngine;
using static GameData;

public class CombatMovement : CopiumScript
{
    public Rigidbody2D rb;
    public GameObject gameObj;
    public GameObject gameObj2;

    public Vector3 hello;
    float MaxTrainSpeed = 1.0f;
    Vector2 velocity = new Vector2(0f, 0f);

    bool inputLeft = false;
    bool inputRight= false;

    bool movingLeft = false;
    bool movingRight = false;

    bool canMoveLeft = true;
    bool canMoveRight= true;

    // Start is called before the first frame update
    void Start()
    {
        Console.WriteLine("COMBAT MOVEMENT STARTED");
        CurrentGameState = GameState.Combat;
    }

    // Update is called once per frame
    void Update()
    {
        if (gameObj == null)
        {
            return;
        }
        if (Input.GetKey(KeyCode.Escape))
        {
            Application.Quit();
        }


        if(CurrentGameState != GameState.Combat)
        {
            inputLeft = false;
            inputRight = false;

            movingLeft = false;
            movingRight = false;

            canMoveLeft = true;
            canMoveRight = true;
        }

        if (rb == null)
        {
            Console.WriteLine("RB WAS NULL");
        }else
        {
            if (CurrentGameState == GameState.Combat)
            {
                if (Input.GetKey(KeyCode.A))
                {
                    //Instantiate(gameObj);
                    velocity = new Vector2(-0.1f, 0f);
                    gameObj.transform.position += velocity;

                }
                if (Input.GetKey(KeyCode.D))
                {
                    velocity = new Vector2(0.1f, 0f);
                    gameObj.transform.position += velocity;
                    Console.WriteLine("d");
                }
    
            }
        }

 
    }

    private void FixedUpdate()
    {
        return;
        if (rb == null)
            return;
        rb.velocity = Vector3.zero;
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
            rb.velocity = new Vector2(-0.1f, 0);

            if (TrainSpeed < MaxTrainSpeed)
            {
                TrainSpeed += TrainAcc;
            }
        }
        else if (movingRight && inputRight)
        {
            rb.velocity = new Vector2(0.1f, 0);

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
    void OnCollisionEnter2D(Collision2D collision)
    {
        Console.WriteLine("COLLIDING FROM C#!");
    }

    void ButtonClickedEffect()
    {
        Console.WriteLine("I AM CLICKED!!");
        Application.Quit();
    }
}

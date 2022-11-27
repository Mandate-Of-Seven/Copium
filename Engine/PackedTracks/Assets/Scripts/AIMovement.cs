using System.Collections;
using System.Collections.Generic;
using System;
using CopiumEngine;
using static GameData;

public class AIMovement : CopiumScript
{
    public enum AIState { Idle, Chase, Evade }
    AIState currentAIState = AIState.Idle;

    public GameObject PlayerTrainGO;
    public GameObject self;


    Vector2 waypoint;
    public float AITimer = 0.75f;
    public float enemySpeed = 0f;
    float timer;

    public Rigidbody2D rb;

    bool inputLeft = false;
    bool inputRight = false;

    bool movingLeft = false;
    bool movingRight = false;

    bool canMoveLeft = true;
    bool canMoveRight = true;
    // Start is called before the first frame update
    void Start()
    {
        Console.WriteLine("Player ID:" + PlayerTrainGO.ID);
        Console.WriteLine("Enemy ID:" + self.ID);
        waypoint = PlayerTrainGO.transform.position;
        timer = AITimer;
    }

    // Update is called once per frame
    void Update()
    {
        if(timer <= 0f)
        {
            timer = AITimer;
            DoAIThings();
        }else{
            timer -= Time.deltaTime;

        }

        if (CurrentGameState == GameState.Combat && currentAIState == AIState.Chase)
        {


            if(self.transform.position.x < waypoint.x + 0.1f && self.transform.position.x > waypoint.x - 0.1f)
            {
                currentAIState = AIState.Idle;
            }else if (self.transform.position.x < waypoint.x)
            {
                self.transform.position += new Vector2(enemySpeed, 0f);
            }
            else if (self.transform.position.x > waypoint.x)
            {
                self.transform.position += new Vector2(-enemySpeed, 0f);
            }
        }
    }
    private void FixedUpdate()
    {
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
            if (EnemyTrainSpeed > 1)
            {
                EnemyTrainSpeed = 1;
            }

            rb.velocity = rb.velocity * 0.9f;

            if (rb.velocity.x < 0.5f && rb.velocity.x > -0.5f)
            {
                EnemyTrainSpeed = 1;

                movingLeft = false;
                movingRight = false;

                canMoveLeft = true;
                canMoveRight = true;
            }
        }
        else if (movingLeft && inputLeft && transform.position.x > -4)
        {
            rb.velocity = new Vector2(-EnemyTrainSpeed, 0);

            if (EnemyTrainSpeed < EnemyMaxTrainSpeed)
            {
                EnemyTrainSpeed += EnemyTrainAcc;
            }
        }
        else if (movingRight && inputRight && transform.position.x < 7)
        {
            rb.velocity = new Vector2(EnemyTrainSpeed, 0);

            if (EnemyTrainSpeed < EnemyMaxTrainSpeed)
            {
                EnemyTrainSpeed += EnemyTrainAcc;
            }
        }
        else if (!inputLeft && !inputRight)
        {
            if (EnemyTrainSpeed > 1)
            {
                EnemyTrainSpeed = 1;
            }

            rb.velocity = rb.velocity * 0.97f;

            if (rb.velocity.x < 0.05f && rb.velocity.x > -0.05f)
            {
                EnemyTrainSpeed = 1;

                movingLeft = false;
                movingRight = false;

                canMoveLeft = true;
                canMoveRight = true;
            }
        }
    }

    void GetPlayerPos()
    {
        waypoint = PlayerTrainGO.transform.position;
    }

    void DoAIThings()
    {
        if (currentAIState == AIState.Idle)
        {
            waypoint = PlayerTrainGO.transform.position;
            currentAIState = AIState.Chase;
        }else if(currentAIState == AIState.Chase)
        {
            waypoint =  PlayerTrainGO.transform.position;

        }
    }

    void AIMoveLeft()
    {
        inputLeft = true;

        if (canMoveLeft)
        {
            movingLeft = true;
            canMoveRight = false;
        }
    }

    void AIMoveRight()
    {
        inputRight = true;

        if (canMoveRight)
        {
            movingRight = true;
            canMoveLeft = false;
        }
    }

    void AIStopMoveLeft()
    {
        inputLeft = false;
    }

    void AIStopMoveRight()
    {
        inputRight = false;
    }
}

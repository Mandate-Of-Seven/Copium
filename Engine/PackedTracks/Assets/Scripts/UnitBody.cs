using System.Collections;
using System.Collections.Generic;
using CopiumEngine;
using System;
using static GameData;
using static Unitf;
using static Itemf;

public class UnitBody : CopiumScript
{

    //Rigidbody2D rb;


    // public enum UnitFaction { Player = 0, Enemy = 1}
    // public UnitFaction thisUnitFaction;
    // public enum UnitRef { Unit0 = 0, Unit1 = 1, Unit2 = 2 };
    // public UnitRef thisUnitRef;

    public GameObject cbdRef;
    public GameObject cbdcopy;
    public int thisUnitFaction;
    public int thisUnitRef;

    public Unit thisUnit;
    Bullet thisBullet;
    public Gun thisGun;

    CombatBulletBody cbd;

    public float AutoAttackTimer;

    public bool CanShoot { get; set; } = false;

    GameObject shieldGO;

    // Start is called before the first frame update
    void Start()
    {
        
    }

    // Update is called once per frame
    void Update()
    {
        /*if (Input.GetKeyDown(KeyCode.Q))
        {
            UpdateUnitBody();
            CurrentGameState = GameState.Combat;
        }

        if (Input.GetKeyDown(KeyCode.W))
        {
            ShootBullet();
        }

        if (Input.GetKeyDown(KeyCode.E))
        {
            Debug.Log(thisUnit.Hp);
        }*/
        if (thisUnitFaction == 1)
        {
            if (AutoAttackTimer > 0 && CurrentGameState == GameState.Combat && CanShoot == true && !thisUnit.IsDead)
            {
                AutoAttackTimer -= Time.deltaTime;
            }
            else if (AutoAttackTimer < 0 && CurrentGameState == GameState.Combat && CanShoot == true && !thisUnit.IsDead)
            {
                ShootBullet();
                AutoAttackTimer = thisGun.AutoAttTimer + 1.0f;// ; Random.Range(1f,2f);
            }
        }
    }

    public void UpdateUnitBody()
    {
        
        // if (thisUnitFaction == UnitFaction.Player)
        // {
        //     if (thisUnitRef == UnitRef.Unit0)
        //     {
        //         thisUnit = PlayerUnits[0];

        //     }
        //     else if (thisUnitRef == UnitRef.Unit1)
        //     {
        //         thisUnit = PlayerUnits[1];

        //     }
        //     else if (thisUnitRef == UnitRef.Unit2)
        //     {
        //         thisUnit = PlayerUnits[2];

        //     }
        // }
        // else if (thisUnitFaction == UnitFaction.Enemy)
        // {
        //     if (thisUnitRef == UnitRef.Unit0)
        //     {
        //         thisUnit = EnemyUnits[0];

        //     }
        //     else if (thisUnitRef == UnitRef.Unit1)
        //     {
        //         thisUnit = EnemyUnits[1];

        //     }
        //     else if (thisUnitRef == UnitRef.Unit2)
        //     {
        //         thisUnit = EnemyUnits[2];

        //     }
        // }

        thisBullet = thisUnit.Bullet;
        thisGun = thisUnit.Gun;
        AutoAttackTimer = thisGun.AutoAttTimer;
        
    }

    public void ShootBullet()
    {
        if (thisBullet.IsShield == false)
        {
            if (thisUnitFaction == 0) // Player
            {
                cbdcopy = Instantiate(cbdRef, new Vector3(transform.position.x, transform.position.y + 0.6f), transform.rotation);
                CombatBulletBody cbd = cbdcopy.GetComponent<CombatBulletBody>();
                cbd.SpawnBullet(thisBullet, true);
            }
            else // Enemy
            {
                cbdcopy = Instantiate(cbdRef, new Vector3(transform.position.x, transform.position.y - 0.5f), transform.rotation);
                CombatBulletBody cbd = cbdcopy.GetComponent<CombatBulletBody>();
                cbd.SpawnBullet(thisBullet, false);
            }
        }
        else
        {
            shieldGO.SetActive(true);
        }
    }

    private void OnCollisionEnter2D(Collision2D collision)
    {
        
    }

    private void OnTriggerEnter2D(Collider2D collision)
    {
        
    }
}

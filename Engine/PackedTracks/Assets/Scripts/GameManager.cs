using System.Collections;
using System.Collections.Generic;
using CopiumEngine;
using CopiumEngine.UI;
using System;
using static GameData;
using static Unitf;

public class GameManager : CopiumScript
{
    public List<UnitBody> playerTrainList = new List<UnitBody>();

    public List<UnitBody> enemyTrainList = new List<UnitBody>();

    public GameObject HeatBar;

    public List<ItemBody> playerInventory = new List<ItemBody>();

    public Camera MainCam;
    public delegate void SetUp();
    SetUp SetUpF;
    SetUp TutSetUpF;

    public GameObject MenuCanvas;
    Vector3 menuCamPos = new Vector3(0, 15, -100);

    public GameObject CombatCanvas;
    Vector3 combatCamPos = new Vector3(0, 0, -100);
    public GameObject playerTrainGO;
    Vector2 playerTrainStartingPos = new Vector2(0.5f, -2);
    public GameObject enemyTrainGO;
    Vector2 enemyTrainStartingPos = new Vector2(0.5f, 4.5f);
    bool combatEnd = false;
    public GameObject victoryText;
    float victoryTimer = 2f;

    public GameObject ResultCanvas;

    public GameObject PrepCanvas;
    public GameObject PrepCanvasBGImg;
    Vector3 prepCamPos = new Vector3(0, -15, -100);
    public GameObject PrepGOs;
    public GameObject PrepButtonGO;
    public Button PrepButtonComp;
    public Text PrepButtonTMP;

    public GameObject TutCanvas;
    public Button Tut0ButtonComp;

    public GameObject TopBlackScreen;
    public GameObject BotBlackScreen;
    float clapVelo = 0f;
    float clapTimer = 0.5f;
    bool clap = false;
    bool unclap = false;

    public MapManager mapManager;

    int lootCounter = 0;

    // Time taken for the transition.
    float thingyTimer = 0.5f;
    float thingyTimer2 = 0.5f;

    bool moveThingy = false;
    bool moveThingy2 = false;

    float thingyTimer3 = 0.5f;
    float thingyTimer4 = 0.5f;

    bool moveThingy3 = false;
    bool moveThingy4 = false;

    float velo = 0f;
    float velo2 = 0f;
    // Start is called before the first frame update
    void Start()
    {
        Console.WriteLine("STOPPED AT Turn off all canvas");
        TurnOffAllCanvas();

        Console.WriteLine("STOPPED AT Set Main cam");
        MainCam.transform.position = menuCamPos;
        Console.WriteLine("STOPPED AT SET ACTIVE");
        MenuCanvas.SetActive(true);
        Console.WriteLine("STOPPED AT ADDING PLAYER UNITS");
        PlayerUnits.Add(new Unit("Basic unit", 10, new Bullet("test bullet", 2, 10), new Gun("test gun", 0.5f), new Skill("Shoot", 0, 0.25f)));
        PlayerUnits.Add(new Unit("Empty unit", 10, new Bullet("test bullet", 2, 10), new Gun("test gun", 0.5f), null));
        PlayerUnits.Add(new Unit("Empty unit", 10, new Bullet("test bullet", 2, 10), new Gun("test gun", 0.5f), null));

        Console.WriteLine("STOPPED AT ADDING ENEMY UNITS");
        EnemyUnits.Add(new Unit("test unit", 6, new Bullet("test bullet", 1, 5), new Gun("test gun", 1), new Skill("Blank", 0, 10)));
        EnemyUnits.Add(new Unit("test unit", 6, new Bullet("test bullet", 1, 6), new Gun("test gun", 2), new Skill("Blank", 0, 10)));
        EnemyUnits.Add(new Unit("test unit", 6, new Bullet("test bullet", 1, 7), new Gun("test gun", 3), new Skill("Blank", 0, 10)));


        Console.WriteLine("STOPPED AT PREP ALL UNITS");
        PrepAllUnits();

        Console.WriteLine("STOPPED AT INTERACTABLE");
        Tut0ButtonComp.interactable = false;

        Console.WriteLine("STOPPED AT PREP ALL ITEMS");
        PrepAllItems();
    }

    // Update is called once per frame
    void Update()
    {
        if (moveThingy)
        {
            float newPosition = Mathf.SmoothDamp(PrepGOs.transform.localPosition.x, -1750, ref velo, thingyTimer);
            PrepGOs.transform.localPosition = new Vector2(newPosition, 0);
            thingyTimer -= Time.deltaTime;
            if (thingyTimer < 0)
            {
                moveThingy = false;
                thingyTimer = 0.5f;
            }
        }
        if (moveThingy2)
        {
            float newPosition = Mathf.SmoothDamp(PrepGOs.transform.localPosition.x, 0, ref velo, thingyTimer2);
            PrepGOs.transform.localPosition = new Vector2(newPosition, 0);
            thingyTimer2 -= Time.deltaTime;
            if (thingyTimer2 < 0)
            {
                moveThingy2 = false;
                thingyTimer2 = 0.5f;
            }
        }
        
        if (moveThingy3)
        {
            float newPosition = Mathf.SmoothDamp(PrepButtonGO.transform.localPosition.x, 1050, ref velo2, thingyTimer3);
            PrepButtonGO.transform.localPosition = new Vector2(newPosition, PrepButtonGO.transform.localPosition.y);
            thingyTimer3 -= Time.deltaTime;
            if (thingyTimer3 < 0)
            {
                moveThingy3 = false;
                thingyTimer3 = 0.5f;
                PrepButtonComp.interactable = true;
            }
        }

        if (moveThingy4)
        {
            float newPosition = Mathf.SmoothDamp(PrepButtonGO.transform.localPosition.x, 700, ref velo2, thingyTimer4);
            PrepButtonGO.transform.localPosition = new Vector2(newPosition, PrepButtonGO.transform.localPosition.y);
            thingyTimer4 -= Time.deltaTime;
            if (thingyTimer4 < 0)
            {
                moveThingy4 = false;
                thingyTimer4 = 0.5f;
                PrepButtonComp.interactable = true;
            }
        }

        /*if (Input.GetKeyUp(KeyCode.L))
        {
            ClapScreen();
        }*/
        if (clap)
        {
            float newPosition = Mathf.SmoothDamp(TopBlackScreen.transform.localPosition.y, 500, ref clapVelo, clapTimer);
            TopBlackScreen.transform.localPosition = new Vector2(0, newPosition);
            BotBlackScreen.transform.localPosition = new Vector2(0, -newPosition);
            clapTimer -= Time.deltaTime;
            if (clapTimer < 0)
            {
                clapVelo = 0f;
                clap = false;
                clapTimer = 0.5f;
                unclap = true;

                TurnOffAllCanvas();
                SetUpF?.Invoke();

                TutSetUpF?.Invoke();
                TutSetUpF = null;
            }
        }
        if (unclap)
        {
            float newPosition = Mathf.SmoothDamp(TopBlackScreen.transform.localPosition.y, 1050, ref clapVelo, clapTimer);
            TopBlackScreen.transform.localPosition = new Vector3(0, newPosition);
            BotBlackScreen.transform.localPosition = new Vector3(0, -newPosition);
            clapTimer -= Time.deltaTime;
            if (clapTimer < 0)
            {
                clapVelo = 0f;
                unclap = false;
                clapTimer = 0.5f;
                TopBlackScreen.SetActive(false);
                BotBlackScreen.SetActive(false);
                if (CurrentGameState == GameState.Combat)
                {
                    CanShootAllUnits(true);
                    if (TutCount == 0)
                    {
                        CanShootAllUnits(false);
                        Tut0ButtonComp.interactable = true;
                    }
                }
            }
        }

        if (CheckEnemyTeamDead() == true && combatEnd == false && CurrentGameState == GameState.Combat)
        {
            victoryText.SetActive(true);
            victoryTimer -= Time.deltaTime;
            if (victoryTimer < 0)
            {
                combatEnd = true;
                ChangeTo("Result");
                victoryText.SetActive(false);
                victoryTimer = 2f;
            }
        }
    }

    public bool CheckEnemyTeamDead()
    {
        for (int i = 0; i < enemyTrainList.Count; i++)
        {
            if (enemyTrainList[i].thisUnit != null && enemyTrainList[i].thisUnit.IsDead == false)
            {
                return false;
            }
        }

        return true;
    }

    public void ClapScreen()
    {
        if (!clap && !unclap)
        {
            clap = true;
            TopBlackScreen.SetActive(true);
            BotBlackScreen.SetActive(true);
        }
    }

    public void ChangeTo(string state)
    {
        //{ Menu, Combat, Result, Prep, Map }
        switch (state)
        {
            case "Menu":
                CurrentGameState = GameState.Menu;
                SetUpF = SetupMenuState;
                ClapScreen();
                break;

            case "Combat":
                CurrentGameState = GameState.Combat;
                PrepAllUnits();
                SetUpF = SetupCombatState;
                CanShootAllUnits(false);
                if (TutCount == 0)
                {
                    TutSetUpF = SetupTut;
                }
                else
                {
                    SpawnEnemyUnits();
                }
                ClapScreen();
                break;

            case "Result":
                /*CurrentGameState = GameState.Result;

                ResultCanvas.SetActive(true);
                break;*/

            case "Prep":
                if (CurrentGameState == GameState.Combat)
                {
                    GenerateLoot();
                    PrepAllItems();

                    CanShootAllUnits(false);
                }
                mapManager.SetUpMaps();

                foreach (Unit unit in PlayerUnits)
                {
                    EndCombatTempHpMod(unit);
                }
                if (CurrentGameState == GameState.Prep)
                {
                    CurrentGameState = GameState.Map;
                    PrepButtonTMP.text = "See Train";
                    moveThingy = true;
                    moveThingy3 = true;
                    PrepButtonComp.interactable = false;
                }
                else if (CurrentGameState == GameState.Map)
                {
                    CurrentGameState = GameState.Prep;
                    PrepButtonTMP.text = "See Map";
                    moveThingy2 = true;
                    moveThingy4 = true;
                    PrepButtonComp.interactable = false;
                }
                else
                {
                    CurrentGameState = GameState.Prep;
                    combatEnd = false;
                    SetUpF = SetupPrepState;
                    if (TutCount == 4)
                    {
                        TutSetUpF = SetupTut;
                    }
                    ClapScreen();
                }
                break;
        }

        
    }
    void SetupMenuState()
    {
        MenuCanvas.SetActive(true);
        MainCam.transform.position = menuCamPos;
    }

    void SetupCombatState()
    {
        PrepCanvasBGImg.SetActive(false);
        CombatCanvas.SetActive(true);
        if (HeatBar.activeSelf != true)
        {
            HeatBar.SetActive(true);
        }
        MainCam.transform.position = combatCamPos;
        playerTrainGO.transform.position = playerTrainStartingPos;
        enemyTrainGO.transform.position = enemyTrainStartingPos;
    }

    void SetupPrepState()
    {
        PrepCanvas.SetActive(true);
        PrepCanvasBGImg.SetActive(true);
        MainCam.transform.position = prepCamPos;
        PrepGOs.transform.localPosition = new Vector2(0, 0);
        PrepButtonGO.transform.localPosition = new Vector2(700, 215);
        PrepButtonTMP.text = "See Map";
    }

    void SetupTut()
    {
        TutCanvas.SetActive(true);
    }

    void TurnOffAllCanvas()
    {
        MenuCanvas.SetActive(false);
        CombatCanvas.SetActive(false);
        ResultCanvas.SetActive(false);
        PrepCanvas.SetActive(false);
        TutCanvas.SetActive(false);
    }

    void PrepAllUnits()
    {
        for (int i = 0; i < playerTrainList.Count; i++)
        {
            playerTrainList[i].UpdateUnitBody();
        }
        for (int i = 0; i < enemyTrainList.Count; i++)
        {
            enemyTrainList[i].UpdateUnitBody();
        }
    }

    void PrepAllItems()
    {
        for (int i = 0; i < playerInventory.Count; i++)
        {
            playerInventory[i].UpdateItemBody();
        }
    }

    void GenerateLoot()
    {
        int rng = 2;//Random.Range(1, 3);

        if (lootCounter == 0)
        {
            PlayerItems.Add(new Item("Single shot gun", "A gun that shoots a single projectile at a time", Item.ModType.Skill, new Skill("Shoot", 0, 1)));
            PlayerItems.Add(new Item("Shield generator", "Creates a shield that bounces projectiles back", new Skill("Shoot", 0, 3), new Bullet("Shield", 0)));
            PlayerItems.Add(new Item("Armor", "Restore health and increase max health to your unit.", Item.ModType.Hp, 1));
        }
        else
        {
            if (rng == 1)
            {
                PlayerItems.Add(new Item("Repair kit", "Restore some health to your unit.", Item.ModType.Heal, 2));
            }
            else if ( rng == 2)
            {
                PlayerItems.Add(new Item("Armor", "Restore health and increase max health to your unit.", Item.ModType.Hp, 1));
            }
            else
            {
                PlayerItems.Add(new Item("Single shot gun", "A gun that shoots a single projectile at a time", Item.ModType.Skill, new Skill("Shoot", 0, 1)));
            }
        }

        lootCounter++;
    }

    public void CanShootAllUnits(bool set)
    {
        for (int i = 0; i < playerTrainList.Count; i++)
        {
            playerTrainList[i].CanShoot = set;
            if (playerTrainList[i].thisGun != null)
            {
                playerTrainList[i].AutoAttackTimer = playerTrainList[i].thisGun.AutoAttTimer;
            }
        }
        for (int i = 0; i < enemyTrainList.Count; i++)
        {
            enemyTrainList[i].CanShoot = set;
            if (enemyTrainList[i].thisGun != null)
            {
                enemyTrainList[i].AutoAttackTimer = enemyTrainList[i].thisGun.AutoAttTimer;
            }
        }
    }
    void SpawnEnemyUnits()
    {
        EnemyUnits.Clear();

        EnemyUnits.Add(new Unit("test unit", 6, new Bullet("test bullet", 1, 5), new Gun("test gun", 1), new Skill("Blank", 0, 10)));
        EnemyUnits.Add(new Unit("test unit", 6, new Bullet("test bullet", 1, 6), new Gun("test gun", 2), new Skill("Blank", 0, 10)));
        EnemyUnits.Add(new Unit("test unit", 6, new Bullet("test bullet", 1, 7), new Gun("test gun", 3), new Skill("Blank", 0, 10)));

        PrepAllUnits();
    }
}

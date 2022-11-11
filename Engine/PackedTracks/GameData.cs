using System.Collections;
using System.Collections.Generic;
using CopiumEngine;

public static class GameData
{
    public enum GameState { Menu, Combat, Result, Prep, Map }
    public static GameState CurrentGameState { get; set; } = GameState.Menu;
    public static int TutCount { get; set; } = 0;
    public static bool UsingSkillState { get; set; } = false;


    public static bool SelectingItem { get; set; } = false;
    public static Item SelectedItem;
    public static ItemBody SelectedItemBody;

    public static int Heat { get; set; } = 10;
    public static int MaxHeat { get; set; } = 50;

    public static int MaxTrainSpeed { get; set; } = 10;
    public static float TrainSpeed { get; set; } = 1;
    public static float TrainAcc { get; set; } = 0.2f;
    
    public static int EnemyMaxTrainSpeed { get; set; } = 10;
    public static float EnemyTrainSpeed { get; set; } = 1;
    public static float EnemyTrainAcc { get; set; } = 0.2f;

    public static List<Item> allItemsRef = new List<Item>();

    //public static List<Skill> allSkillsRef = new List<Skill>();
    public static List<Bullet> allBulletsRef = new List<Bullet>();
    public static List<Gun> allGunsRef = new List<Gun>();

    public static List<Unit> PlayerUnits = new List<Unit>();
    public static List<Unit> EnemyUnits = new List<Unit>();

    
    public static List<Item> PlayerItems = new List<Item>();

    public static int MapTier { get; set; } = 0;
    public static int MapChoice { get; set; } = 0;

    //public static Sprite shieldSprite;
}

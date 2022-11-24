using System.Collections;
using System.Collections.Generic;
using CopiumEngine;

public class GameData : CopiumScript
{
    public enum GameState { Menu, Combat, Result, Prep, Map }
    public  GameState CurrentGameState { get; set; } = GameState.Menu;
    public  int TutCount { get; set; } = 0;
    public  bool UsingSkillState { get; set; } = false;


    public  bool SelectingItem { get; set; } = false;
    public  Item SelectedItem;
    public  ItemBody SelectedItemBody;

    public  int Heat { get; set; } = 10;
    public  int MaxHeat { get; set; } = 50;

    public  int MaxTrainSpeed { get; set; } = 10;
    public  float TrainSpeed { get; set; } = 1;
    public  float TrainAcc { get; set; } = 0.2f;
    
    public  int EnemyMaxTrainSpeed { get; set; } = 10;
    public  float EnemyTrainSpeed { get; set; } = 1;
    public  float EnemyTrainAcc { get; set; } = 0.2f;

    public  List<Item> allItemsRef = new List<Item>();

    //public static List<Skill> allSkillsRef = new List<Skill>();
    public  List<Bullet> allBulletsRef = new List<Bullet>();
    public  List<Gun> allGunsRef = new List<Gun>();

    public  List<Unit> PlayerUnits = new List<Unit>();
    public  List<Unit> EnemyUnits = new List<Unit>();

    
    public  List<Item> PlayerItems = new List<Item>();

    public  int MapTier { get; set; } = 0;
    public  int MapChoice { get; set; } = 0;

    //public static Sprite shieldSprite;
}

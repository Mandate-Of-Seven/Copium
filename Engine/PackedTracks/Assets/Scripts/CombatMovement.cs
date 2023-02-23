using CopiumEngine;
using System;

public class CombatMovement : CopiumScript
{
    public int hello;
    public StoryObject story;
    public Test script;
    public CombatMovement combatMovement;
    public Text text;
    public string textContent = "";
    void Start()
    {
        Console.WriteLine("HELLO FROM COMBAT MOVEMENT");
        text.text = "BUTTON";
        Console.WriteLine(text.text);
    }

    void Update()
    {
        text.text = "GEE";
    }
}
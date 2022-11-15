using System.Collections;
using System.Collections.Generic;
using CopiumEngine;

public class Bullet
{
    public string Name { get; set; }

    public int Dmg { get; set; } = 1;
    public int Speed { get; set; } = 5;
    public bool IsShield { get; set; } = false;

    public Bullet(string name, int dmg, int speed)
    {
        Name = name;
        Dmg = dmg;
        Speed = speed;
    }

    public Bullet(string name, int speed)
    {
        Name = name;
        Speed = speed;

        IsShield = true;
    }
}

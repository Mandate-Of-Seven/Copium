using System.Collections;
using System.Collections.Generic;
using CopiumEngine;

public class Gun
{
    public string Name { get; set; }

    public float AutoAttTimer { get; set; } = 2;
    public int BulletCount { get; set; } = 1;

    public Gun(string name, float autoAttTimer)
    {
        Name = name;
        AutoAttTimer = autoAttTimer;
    }

    public Gun(string name, float autoAttTimer, int bulletCount)
    {
        Name = name;
        AutoAttTimer = autoAttTimer;
        BulletCount = bulletCount;
    }
}

using System.Collections;
using System.Collections.Generic;
using CopiumEngine;

public class Bullet : CopiumScript
{
    public int Dmg { get; set; } = 1;
    public int Speed { get; set; } = 0;

    public Bullet(int dmg, int speed)
    {
        Dmg = dmg;
        Speed = speed;
    }

    void Update()
    {
        Vector3 pos = transform.position;
        pos.y += Speed;
    }
}

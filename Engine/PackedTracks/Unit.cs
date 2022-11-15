using System.Collections;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using CopiumEngine;
using static GameData;

public class Unit
{
    public string Name { get; set; }

    public int MaxHp { get; set; }
    public int Hp { get; set; }

    public int TempHp { get; set; } = 0;

    public Skill Skill { get; set; }
    public Bullet Bullet { get; set; }
    public Gun Gun { get; set; }

    public bool IsDead { get; set; } = false;

    public Unit(string name, int maxHp, Bullet bullet, Gun gun, Skill skill)
    {
        Name = name;
        MaxHp = maxHp;
        Hp = MaxHp;

        Bullet = bullet;
        Gun = gun;
        Skill = skill;
    }
}

public static class Unitf
{
    public static void DamageHeat(int dmg)
    {
        Heat -= dmg;

        if (Heat <= 0)
        {
            Heat = 0;
        }
    }
    public static void DamageHeat(Bullet bullet)
    {
        Heat -= bullet.Dmg;

        if (Heat <= 0)
        {
            Heat = 0;
        }
    }
    public static void PrepFreshUnit(Unit target)
    {
        target.Hp = target.MaxHp;
    }

    public static void DamageUnit(Unit target, int dmg)
    {
        target.Hp -= dmg;

        if (target.Hp <= 0)
        {
            target.Hp = 0;
            target.IsDead = true;
        }

    }
    public static void DamageUnit(Unit target, Bullet bullet)
    {
        target.Hp -= bullet.Dmg;

        if (target.Hp <= 0)
        {
            target.Hp = 0;
            target.IsDead = true;
        }

    }

    public static void HealUnit(Unit target, int heal)
    {
        target.Hp += heal;

        if (target.IsDead == true)
        {
            target.IsDead = false;
        }

        if (target.Hp > target.MaxHp)
        {
            target.Hp = target.MaxHp;
        }
    }
    
    public static void CombatTempHpMod(Unit target, int mod)
    {
        target.TempHp += mod;

        target.Hp += mod;
        target.MaxHp += mod;
    }
    public static void EndCombatTempHpMod(Unit target)
    {
        target.Hp -= target.TempHp;

        if (target.Hp <= 0)
        {
            target.Hp = 1;
        }

        target.MaxHp -= target.TempHp;

        if (target.MaxHp <= 0)
        {
            target.MaxHp = 1;
        }

        target.TempHp = 0;
    }
    
    public static void PermHpMod(Unit unit, int mod)
    {
        unit.Hp += mod;
        unit.MaxHp += mod;
    }

    public static void ChangeSkill(Unit unit, Skill mod)
    {
        unit.Skill = mod;
    }
    public static void ChangeGun(Unit unit, Gun mod)
    {
        unit.Gun = mod;
    }
    public static void ChangeBullet(Unit unit, Bullet mod)
    {
        unit.Bullet = mod;
    }
}

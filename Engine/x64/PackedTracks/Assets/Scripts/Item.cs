using System.Collections;
using System.Collections.Generic;
using CopiumEngine;
using static Unitf;

public class Item
{
    public enum ModType { Hp, Heal, Skill, Bullet, Gun, Shield }
    public string Name { get; set; }
    public string Desc { get; set; } = "test";

    public int Mod { get; set; } = 0;
    public ModType Type { get; set; }

    public int Mod2 { get; set; } = 0;
    public ModType Type2 { get; set; }

    public Skill skill;
    public Bullet bullet;
    public Gun gun;

    public Item(string name, string desc, ModType type,  int mod)
    {
        Name = name;
        Desc = desc;
        Type = type;
        Mod = mod;
    }

    public Item(string name, string desc, ModType type, int mod, ModType type2, int mod2)
    {
        Name = name;
        Desc = desc;
        Type = type;
        Mod = mod;
        Type2 = type;
        Mod2 = mod2;
    }

    public Item(string name, string desc, ModType type, Skill skill)
    {
        Name = name;
        Desc = desc;
        Type = type;
        this.skill = skill;
    }

    public Item(string name, string desc, ModType type, Bullet bullet)
    {
        Name = name;
        Desc = desc;
        Type = type;
        this.bullet = bullet;
    }

    public Item(string name, string desc, ModType type, Gun gun)
    {
        Name = name;
        Desc = desc;
        Type = type;
        this.gun = gun;
    }

    public Item(string name, string desc, Skill skill, Bullet bullet)
    {
        Name = name;
        Desc = desc;
        Type = ModType.Shield;
        this.skill = skill;
        this.bullet = bullet;
    }
}

public static class Itemf
{
    public static void ConsumeItem(Item item, Unit target)
    {
        switch (item.Type)
        {
            case Item.ModType.Hp:
                PermHpMod(target, item.Mod);
                break;

            case Item.ModType.Heal:
                HealUnit(target, item.Mod);
                break;

            case Item.ModType.Skill:
                target.Name = "Basic unit";
                ChangeSkill(target, item.skill);
                break;

            case Item.ModType.Shield:
                target.Name = "Shield unit";
                ChangeSkill(target, item.skill);
                ChangeBullet(target, item.bullet);
                break;

            default:
                break;
        }

        if (item.Mod2 != 0)
        {
            switch (item.Type2)
            {
                case Item.ModType.Hp:
                    PermHpMod(target, item.Mod2);
                    break;

                case Item.ModType.Heal:
                    HealUnit(target, item.Mod2);
                    break;

                case Item.ModType.Skill:
                    ChangeSkill(target, item.skill);
                    break;

                default:
                    break;
            }
        }
    }
}

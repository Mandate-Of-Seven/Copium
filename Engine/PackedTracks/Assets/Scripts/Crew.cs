using CopiumEngine;
using System;

public class Crew : CopiumScript
{
    public CrewMenu crewMenu;

    public GameObject showDeployed;
    
    public Text healthT, mentalT, hungerT;

    public Button selectBtn;
    public bool selected = false;

    public int health = 15;
    public int mental = 15;
    public int hunger = 10;

    void Start()
    {
        
    }
    void Update()
    {
        if(crewMenu.preparing)
        {
            if (selectBtn.state == ButtonState.OnClick)
                selected = !selected;
            showDeployed.SetActive(selected);
        }

        if (crewMenu.deployed)
        {
            selected = false;
            showDeployed.SetActive(selected);
        }

        UpdateStats();
    }

    void UpdateStats()
    {
        if(health > 10)
            healthT.text = "Healthy";
        else if(health > 5)
            healthT.text = "Injured";
        else if (health > 0)
            healthT.text = "Critical";
        else
            healthT.text = "Dead";

        if (mental > 10)
            mentalT.text = "Sound";
        else if (mental > 5)
            mentalT.text = "Shaken";
        else if (mental > 0)
            mentalT.text = "Deranged";
        else
            mentalT.text = "Gone";

        if (hunger > 9)
            hungerT.text = "Full";
        else if (hunger > 5)
            hungerT.text = "Satisfied";
        else if (hunger > 0)
            hungerT.text = "Hungry";
        else
            hungerT.text = "Famished";
    }
}
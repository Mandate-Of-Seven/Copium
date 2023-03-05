using CopiumEngine;
using System;
using System.Collections.Generic;

public class Crew : CopiumScript
{
    public CrewMenu crewMenu;
    public GameObject showDeployed;
    
    public Text healthT, mentalT, hungerT;

    public Button selectBtn;
    public bool selected = false;

    public int crewIndex;
    CrewMenu.Person person;

    void Start()
    {
        person = crewMenu.crew[crewIndex];
    }
    void Update()
    {
        if (crewMenu.preparing)
        {
            selectBtn.enabled = true;
            if (selectBtn.state == ButtonState.OnClick)
                selected = !selected;
            showDeployed.SetActive(selected);
        }
        else
        {
            selectBtn.enabled = false;
            selected = false;
            showDeployed.SetActive(false);
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
        person = crewMenu.crew[crewIndex];
        // Console.WriteLine(person.name);

        if (person.health > 10)
            healthT.text = "Healthy";
        else if(person.health > 5)
            healthT.text = "Injured";
        else if (person.health > 0)
            healthT.text = "Critical";
        else
            healthT.text = "Dead";

        if (person.mental > 10)
            mentalT.text = "Calm";
        else if (person.mental > 5)
            mentalT.text = "Irrational";
        else if (person.mental > 0)
            mentalT.text = "Insane";
        else
            mentalT.text = "Suicidal";

        if (person.hunger > 5)
            hungerT.text = "Satisfied";
        else if (person.hunger > 0)
            hungerT.text = "Hungry";
        else
            hungerT.text = "Famished";
    }
}
using CopiumEngine;
using System;
using System.Collections.Generic;

public class Crew : CopiumScript
{
    public CrewMenu crewMenu;
    public GameObject showDeployed;
    
    public Text healthT, mentalT, hungerT;

    public Button selectBtn;
    public ButtonWrapper selectBtnWrapper;
    public bool selected = false;
    public bool isDeployed = false;

    public int crewIndex;
    CrewMenu.Person person;

    public Image sprite;

    void Start()
    {
        person = crewMenu.crew[crewIndex];
        selectBtnWrapper = new ButtonWrapper(selectBtn,crewMenu.audioManager);
        selectBtnWrapper.SetImage(sprite);
        Disable();
    }
    void Update()
    {
        if (selectBtnWrapper.GetState() == ButtonState.OnClick)
        {
            if (selected)
                Deselect();
            else
                Select();
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
            hungerT.text = "Full";
        else if (person.hunger > 0)
            hungerT.text = "Hungry";
        else
            hungerT.text = "Famished";

        Color tmp = Color.white;
        float x = ((float)person.health/15.0f);
        tmp.r = x;
        tmp.g = x;
        tmp.b = x;

        sprite.color = tmp;
        
    }

    public void Disable()
    {
        Deselect();
        selectBtnWrapper.SetInteractable(false);
    }

    public void Enable()
    {
        selectBtnWrapper.SetInteractable(true);
    }

    public void Deselect()
    {
        selected = false;
        isDeployed = selected;
        showDeployed.SetActive(selected);
    }

    public void Select()
    {
        selected = true;
        isDeployed = selected;
        showDeployed.SetActive(selected);
    }
}
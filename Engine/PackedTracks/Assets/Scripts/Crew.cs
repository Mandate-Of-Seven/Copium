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
    Person person;

    public Image sprite;

    void Start()
    {
        person = crewMenu.crew[crewIndex];
        person.crewScript = this;
        selectBtnWrapper = new ButtonWrapper(selectBtn,crewMenu.audioManager);
        selectBtnWrapper.useDisabled = false;
        selectBtnWrapper.SetImage(sprite);
        Disable();
    }
    void Update()
    {
        if (selectBtnWrapper.GetState() == ButtonState.OnClick && person.alive)
        {
            if (selected)
                Deselect();
            else
                Select();
        }
    }

    //UPDATES AS IN IT UPDATES THE EFFECTS

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
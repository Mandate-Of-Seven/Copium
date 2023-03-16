/*!***************************************************************************************
\file			Crew.cs
\project
\author			Sean Ngo
\co-author		Shawn Tanary
\co-author		Zacharie Hong

\par			Course: GAM250
\par			Section:
\date			10/03/2023

\brief
	Contains functions needed for selecting and enabling crew members

All content � 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
using CopiumEngine;
using System;
using System.Collections.Generic;

public class Crew : CopiumScript
{
    public enum HEALTH_STATE
    {
        DEAD,
        CRITICAL,
        INJURED,
        HEALTHY,
        NONE = -1
    }
    public enum MENTAL_STATE
    {
        SUICIDAL,
        INSANE,
        IRRATIONAL,
        CALM,
        NONE = -1
    }
    public enum HUNGER_STATE
    {
        FAMISHED,
        HUNGRY,
        FULL,
        NONE = -1
    }

    public CrewMenu crewMenu;
    public GameObject showDeployed;
    
    public Text healthT, mentalT, hungerT;

    public Button selectBtn;
    public ButtonWrapper selectBtnWrapper;
    public bool selected = false;
    public bool isDeployed = false;

    public int crewIndex;
    public Person person;

    public Image sprite;

    void Start()
    {
        person = crewMenu.crew[crewIndex];
        person.crewScript = this;
        selectBtnWrapper = new ButtonWrapper(selectBtn,crewMenu.audioManager,crewMenu.crewStatusManager.tooltip);
        selectBtnWrapper.useDisabled = false;
        selectBtnWrapper.SetImage(sprite);
        selectBtnWrapper.failureText = "You need to be preparing your crew members first!";
        Disable();
    }
    void Update()
    {
        if (selectBtnWrapper.GetState() == ButtonState.OnClick)
        {
            if (selected)
            {
                Deselect();
            }
            else
            {
                Select();
            }
        }
    }

    //UPDATES AS IN IT UPDATES THE EFFECTS

    /*******************************************************************************
	/*!
	    \brief
		    Disables the hover and deselects the object
	*/
    /*******************************************************************************/
    public void Disable()
    {
        Deselect();
        selectBtnWrapper.SetInteractable(false);
    }

    /*******************************************************************************
	/*!
	    \brief
		    Enables the hover effect of the button
	*/
    /*******************************************************************************/
    public void Enable()
    {
        Debug.Log("TRYING TO ENABLE");
        if (person.alive)
        {
            selectBtnWrapper.SetInteractable(true);
        }
        else
        {
            //selectBtnWrapper.SetInteractable(false);
            selectBtnWrapper.failureText = person.name + " is dead...";
        }
    }

    /*******************************************************************************
	/*!
	    \brief
		    Deselects the object and hides the deployed text
	*/
    /*******************************************************************************/
    public void Deselect()
    {
        selected = false;
        isDeployed = selected;
        showDeployed.SetActive(selected);
    }

    /*******************************************************************************
	/*!
	    \brief
		    Selects the object and unhides the deployed text
	*/
    /*******************************************************************************/
    public void Select()
    {
        selected = true;
        isDeployed = selected;
        showDeployed.SetActive(selected);
    }
}
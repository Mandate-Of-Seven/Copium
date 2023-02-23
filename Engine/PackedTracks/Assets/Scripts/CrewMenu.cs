using CopiumEngine;
using System;

public class CrewMenu: CopiumScript
{
	public Text fuelText;
    public Text heatText;
    public Text foodText;
    public Button prepareButton;

    public int fuel,heat,food;
    //std::string

    void Start()
	{
        updateTexts();
	}
	void Update()
	{
       //if prepare button is pressed
       //show what event happened
       //update values based on event that happened
       //have condition for when certain values hit 0??
       if (prepareButton.state == ButtonState.OnRelease)
       {
        generateEvents();
       }
       updateTexts();
	}

    void updateTexts()
    {
        fuelText.text = "Fuel: " +fuel;
        heatText.text = "Heat: " +heat;
        foodText.text = "Food: " +food;
    }


     void generateEvents()
    {
        float chance = RNG.Range(0,1);
        updateValues(chance);
    }



    void updateValues(float chance)
    {
        //show event screen
        //depending on chance, change text that appears
        if (chance >=0 && chance <0.25)
        {
            //text = "Horrible event, lose 1 of every resource";
            fuel -= 1;
            food -= 1;
            heat -=1;
        }
        else if (chance >= 0.25 && chance <0.50)
        {
            //text = "Found more fuel, but required to use more food to return.\nGain 1 fuel, lose 1 food";
            fuel +=1;
            food -=1;
        }
        else if (chance >= 0.50 && chance < 0.75)
        {
            //text = "Found more food, but the bad weather reequires more heat to be consumed.\n Gain 1 food, lose 1 heat";
            food +=1;
            heat -=1;
        }
        else if(chance >= 0.75 && chance <0.95)
        {
            //text = "Gained more resources for heat, but something about losing fuel.\n Gain 1 heat, lose 1 fuel";
            fuel -=1;
            heat +=1;
        }
        else if (chance >=0.95)
        {
            //text = "Good event, gain 2 of every resource.";
            fuel +=2;
            food +=2;
            heat +=2;
        }

        if (fuel < 0)
        fuel = 0;

        if (food < 0)
        food = 0;

        if (heat < 0)
        heat = 0;

        //hide event screen when ok/return button pressed

    }

   
}

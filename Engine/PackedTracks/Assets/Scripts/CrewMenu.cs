using CopiumEngine;
using System;

public class CrewMenu: CopiumScript
{
	public Text fuelText;
    public Text heatText;
    public Text foodText;

    public Button prepareButton;
    public Button deployButton;

    public Button closeCrewButton;

    public int fuel,heat,food;

    public Button person1,person2,person3,person4;
    bool select1,select2,select3,select4 = false;
    public Text status;

    bool preparing = false;
    bool deployed = false;

    public int health1,health2,health3,health4 = 10;
    public int mental1,mental2,mental3,mental4 = 5;
    public int hunger1,hunger2,hunger3,hunger4 = 5;

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
       
       if (closeCrewButton.state == ButtonState.OnClick)
       {
            gameObject.SetActive(false);
            return;
       }
       
       if (prepareButton.state == ButtonState.OnClick)
       {
            select1 = select2 = select3 = select4 = false;
            preparing = !preparing;
            deployButton.gameObject.SetActive(preparing);
       }

       if (deployButton.gameObject.activeSelf && deployButton.state == ButtonState.OnClick)
       {
            select1 = select2 = select3 = select4 = false;
            deployed = true;
       }

       if (preparing)
       {
            int count = 0;
            if (person1.state == ButtonState.OnClick)
            {
                ++count;
                select1 = !select1;
            }
            if (person2.state == ButtonState.OnClick)
            {
                ++count;
                select2 = !select2;
            }
            if (person3.state == ButtonState.OnClick)
            {
                ++count;
                select3 = !select3;
            }
            if (person4.state == ButtonState.OnClick)
            {
                ++count;
                select4 = !select4;
            }
            if (count >= 1 && !deployButton.gameObject.activeSelf)
                deployButton.gameObject.SetActive(true);
       }

        if (preparing)
        {
            string statusText = "Preparing: ";
            if (select1)
                statusText += "Bronson ";
            if (select2)
                statusText += "Chuck ";
            if (select3)
                statusText += "Harris ";
            if (select4)
                statusText += "Danton ";
            if (status == null)
            {
                Console.WriteLine(statusText);
            }
            else
            {
                status.text = statusText;
            }
        }
        else if (deployed)
        {
            deployed = false;
        }
        //updateTexts();
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
        
        //prepareCanvas.SetActive(true);      
        //updateValues(chance);
    }



    // void updateValues(float chance)
    // {
    //     //show event screen
    //     //depending on chance, change text that appears
    //     if (chance >=0 && chance <0.25)
    //     {
    //         prepareText.text = "Horrible event, lose 1 of every resource";
    //         fuel -= 1;
    //         food -= 1;
    //         heat -=1;
    //     }
    //     else if (chance >= 0.25 && chance <0.50)
    //     {
    //         prepareText.text = "Found more fuel, but required to use more food \nto return.\n\nGain 1 fuel, lose 1 food";
    //         fuel +=1;
    //         food -=1;
    //     }
    //     else if (chance >= 0.50 && chance < 0.75)
    //     {
    //         prepareText.text = "Found more food, but the bad weather requires \nmore heat to be consumed.\n\n Gain 1 food, lose 1 heat";
    //         food +=1;
    //         heat -=1;
    //     }
    //     else if(chance >= 0.75 && chance <0.95)
    //     {
    //         prepareText.text = "Gained more resources for heat, but something about \nlosing fuel.\n\n Gain 1 heat, lose 1 fuel";
    //         fuel -=1;
    //         heat +=1;
    //     }
    //     else if (chance >=0.95)
    //     {
    //         prepareText.text = "Good event, gain 2 of every resource.";
    //         fuel +=2;
    //         food +=2;
    //         heat +=2;
    //     }

    //     if (fuel < 0)
    //     fuel = 0;

    //     if (food < 0)
    //     food = 0;

    //     if (heat < 0)
    //     heat = 0;

    //     //hide crewscreen
    //     gameObject.SetActive(false);
    // }

   
}

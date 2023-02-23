using CopiumEngine;
using System;

public class CrewMenu: CopiumScript
{
	public Text fuelText;
    public Text heatText;
    public Text foodText;

    public int fuel,heat,food;
    //std::string

    void Start()
	{
        updateTexts();
	}
	void Update()
	{
        
	}

    void updateTexts()
    {
        fuelText.text = "Fuel: " +fuel;
        heatText.text = "Heat: " +heat;
        foodText.text = "Food: " +food;
    }
}

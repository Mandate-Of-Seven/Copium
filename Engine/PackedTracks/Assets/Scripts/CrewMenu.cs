using CopiumEngine;
using System;
using System.Linq;

public class CrewMenu: CopiumScript
{
	public Text suppliesText;
    public Text titleText;

    public Button prepareButton;
    public Button deployButton;

    public Button closeCrewButton;

    public int supplies;

    public Crew harris, bronson, chuck, danton;

    public bool preparing = false;
    public bool deployed = false;

    public struct Person
    {
        public string name;
        public int health;
        public int mental;
        public int hunger;
        public float timer;
        public Person(string _name)
        {
            name = _name;
            health = 15;
            mental = 15;
            hunger = 10;
            timer = 0.0f;
        }
    }

    public Person[] crew = new Person[4]
    {
        new Person("Harris"),
        new Person("Bronson"),
        new Person("Chuck"),
        new Person("Danton")
    };

    void Start()
	{
        
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
            preparing = !preparing;

        if (deployButton.state == ButtonState.OnClick && preparing)
        {
            deployed = true;
            preparing = false;
        }
       
        //UpdateHealth();
        UpdateTexts();
    }

    void UpdateTexts()
    {
        suppliesText.text = "Supplies: " + supplies;

        if (preparing)
            titleText.text = "Selecting Members";
        else
            titleText.text = "Crew Members";
    }

    void UpdateHealth()
    {
        for (int i = 0; i < crew.Length; ++i)
        {
            if (crew[i].hunger <= 0 && crew[i].health >= 0)
            {
                if (crew[i].timer >= 1.0f)
                {
                    crew[i].health -= 1;
                }
            }
        }
    }

    void GenerateEvents()
    {
        float chance = RNG.Range(0,1);
        
        //prepareCanvas.SetActive(true);      
        UpdateValues(chance);
    }


    void UpdateValues(float chance)
    {
        //show event screen
        //depending on chance, change text that appears
        //if (chance >= 0 && chance < 0.25)
        //{
        //    status.text = "Horrible event, lose 1 of every resource";
        //    fuel -= 1;
        //    food -= 1;
        //    heat -= 1;
        //}
        //else if (chance >= 0.25 && chance < 0.50)
        //{
        //    status.text = "Found more fuel, but required to use more food \nto return.\n\nGain 1 fuel, lose 1 food";
        //    fuel += 1;
        //    food -= 1;
        //}
        //else if (chance >= 0.50 && chance < 0.75)
        //{
        //    status.text = "Found more food, but the bad weather requires \nmore heat to be consumed.\n\n Gain 1 food, lose 1 heat";
        //    food += 1;
        //    heat -= 1;
        //}
        //else if (chance >= 0.75 && chance < 0.95)
        //{
        //    status.text = "Gained more resources for heat, but something about \nlosing fuel.\n\n Gain 1 heat, lose 1 fuel";
        //    fuel -= 1;
        //    heat += 1;
        //}
        //else if (chance >= 0.95)
        //{
        //    status.text = "Good event, gain 2 of every resource.";
        //    fuel += 2;
        //    food += 2;
        //    heat += 2;
        //}

        //if (fuel < 0)
        //    fuel = 0;

        //if (food < 0)
        //    food = 0;

        //if (heat < 0)
        //    heat = 0;

        //hide crewscreen
        //gameObject.SetActive(false);
    }


}

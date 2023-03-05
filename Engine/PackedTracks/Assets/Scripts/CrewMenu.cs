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

    public Fade fader;

    public int supplies;

    public Crew harris;
    public Crew bronson;
    public Crew chuck;
    public Crew danton;

    public bool preparing = false;
    public bool deployed = false;

    float timer = 0.0f;

    float healthInterval = 5.0f; // The interval in seconds in which the crew members lose health
    float hungerInterval = 4.0f; // The interval in seconds in which the crew members get hungry

    public bool storageComparment = true; // For the food storage

    public enum STAT_TYPES
    {
        ALIVE,
        HEALTH,
        MENTAL,
        HUNGER,
    }

    public struct Person
    {
        string _name;
        bool _alive;
        int _health;
        int _mental;
        int _hunger;
        float _timer;

        public string name
        {
            get { Console.WriteLine("getting health"); return _name; }
            set { Console.WriteLine("setting health"); _name = value; }
        }

        public bool alive
        {
            get { return _alive; }
            set { _alive = value; }
        }

        public int health
        {
            get { return _health; }
            set { _health = value; }
        }

        public int mental
        {
            get { return _mental; }
            set { _mental = value; }
        }

        public int hunger
        {
            get { return _hunger; }
            set { _hunger = value; }
        }

        public float timer
        {
            get { return _timer; }
            set { _timer = value; }
        }
        public Person(string new_name)
        {
            _name = new_name;
            _alive = true;
            _health = 15;
            _mental = 15;
            _hunger = 10;
            _timer = 0.0f;
            Console.WriteLine("CONSTRUCTED PERSON!: " + _name + _health);
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
            fader.fadeIn = true;
            fader.shouldFade = true;

            GenerateEvents();
        }
       
        UpdateTexts();
    }

    public void UpdateAllStats()
    {
        CheckSupplies();
        CheckCrewHealth();
        CheckCrewStatus();
    }

    public bool CheckAllCrewAlive()
    {
        for (int i = 0; i < crew.Length; i++)
        {
            if (crew[i].alive)
                return true;
        }

        return false;
    }

    void CheckCrewStatus()
    {
        for (int i = 0; i < crew.Length; ++i)
        {
            if (!crew[i].alive)
            {
                crew[i].health = 0;
                crew[i].mental = 0;
                crew[i].hunger = 0;
            }
        }
    }

    void CheckSupplies()
    {
        if(supplies == 0)
        {
            if (timer >= hungerInterval)
            {
                for (int i = 0; i < crew.Length; i++)
                {
                    if (crew[i].hunger > 0)
                        crew[i].hunger -= 1;
                }

                timer = 0.0f;
            }

            timer += Time.deltaTime;
        }
    }

    void CheckCrewHealth()
    {
        for (int i = 0; i < crew.Length; i++)
        {
            if (crew[i].hunger <= 0)
            {
                if(crew[i].timer >= healthInterval)
                {
                    crew[i].health -= 1;
                    crew[i].timer = 0.0f;
                }

                crew[i].timer += Time.deltaTime;
            }

            if (crew[i].health <= 0)
                crew[i].alive = false;
        }
    }

    void UpdateTexts()
    {
        suppliesText.text = "Supplies: " + supplies;

        if (preparing)
            titleText.text = "Selecting Members";
        else
            titleText.text = "Crew Members";
    }

    public void SetSupplies(int amount)
    {
        // Bean: Visually show the supply reducing

        if (!storageComparment)
            return;

        supplies = amount;
    }

    public void ChangeSupplies(int amount)
    {
        // Bean: Visually show the supply reducing

        if (!storageComparment)
            return;

        supplies += amount;

        if (supplies < 0)
            supplies = 0;
    }

    public void SetAllCrew(STAT_TYPES types, int amount)
    {
        for (int i = 0; i < crew.Length; i++)
        {
            switch (types)
            {
                case STAT_TYPES.ALIVE:
                    crew[i].alive = (amount > 0) ? true : false;
                    break;
                case STAT_TYPES.HEALTH:
                    if(crew[i].health > amount)
                        crew[i].health = amount;
                    break;
                case STAT_TYPES.MENTAL:
                    if (crew[i].mental > amount)
                        crew[i].mental = amount;
                    break;
                case STAT_TYPES.HUNGER:
                    if (crew[i].hunger > amount)
                        crew[i].hunger = amount;
                    break;
            }
        }
    }

    public void ChangeAllCrew(STAT_TYPES types, int amount)
    {
        for (int i = 0; i < crew.Length; i++)
        {
            switch (types)
            {
                case STAT_TYPES.HEALTH:
                    if (crew[i].health > 0)
                        crew[i].health += amount;
                    break;
                case STAT_TYPES.MENTAL:
                    if (crew[i].mental > 0)
                        crew[i].mental += amount;
                    break;
                case STAT_TYPES.HUNGER:
                    crew[i].hunger += amount;

                    if (crew[i].hunger > 10)
                        crew[i].hunger = 10;
                    break;
            }
        }
    }

    public void SetCrew(STAT_TYPES types, int index, int amount)
    {
        switch (types)
        {
            case STAT_TYPES.ALIVE:
                crew[index].alive = (amount > 0) ? true : false;
                break;
            case STAT_TYPES.HEALTH:
                if (crew[index].health > amount)
                    crew[index].health = amount;
                break;
            case STAT_TYPES.MENTAL:
                if (crew[index].mental > amount)
                    crew[index].mental = amount;
                break;
            case STAT_TYPES.HUNGER:
                if (crew[index].hunger > amount)
                    crew[index].hunger = amount;
                break;
        }
    }

    void GenerateEvents()
    {
        float chance = RNG.Range(0,1);
        
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
        deployed = false;
        gameObject.SetActive(false);
    }


}

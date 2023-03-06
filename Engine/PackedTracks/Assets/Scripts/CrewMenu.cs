using CopiumEngine;
using System;
using System.Linq;

public class CrewMenu: CopiumScript
{
    public Text suppliesText;
    public Text titleText;

    string titleString;

    public Button prepareButton;
    public Button deployButton;

    public CrewStatusManager crewStatusManager; 

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

    //to record who deploys
    public Prepare prepareManager;
    public bool hDeploy, bDeploy, cDeploy, dDeploy;

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
        string _resultText;

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

        public string resultText
        {
            get { return _resultText; }
            set { _resultText = value; }
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
            _resultText = "1";
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
        titleString = titleText.text;
    }
	void Update()
	{
        //if prepare button is pressed
        //show what event happened
        //update values based on event that happened
        //have condition for when certain values hit 0??

        if (prepareButton.state == ButtonState.OnClick)
        {
            preparing = !preparing;
            hDeploy = harris.isDeployed;
            bDeploy = bronson.isDeployed;
            cDeploy = chuck.isDeployed;
            dDeploy = danton.isDeployed;
        }

        if (deployButton.state == ButtonState.OnClick && preparing)
        {
            deployed = true;
            preparing = false;
            fader.fadeIn = true;
            fader.shouldFade = true;

            hDeploy = harris.isDeployed;
            bDeploy = bronson.isDeployed;
            cDeploy = chuck.isDeployed;
            dDeploy = danton.isDeployed;

            StartPrepare();
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
            titleText.text = titleString;
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


    //generate a random event for each deployed
    public void StartPrepare()
    {

        if (harris.isDeployed)
        {
            prepareManager.GenerateEvents(crew[0]);
        }
        else if (bronson.isDeployed)
        {
            prepareManager.GenerateEvents(crew[1]);
        }
        else if (chuck.isDeployed)
        {
            prepareManager.GenerateEvents(crew[2]);
        }
        else if (danton.isDeployed)
        {
            prepareManager.GenerateEvents(crew[3]);
        }

        //hide crewscreen
        deployed = false;
    }

}

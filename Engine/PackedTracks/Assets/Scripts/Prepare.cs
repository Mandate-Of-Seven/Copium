using CopiumEngine;
using System;

public class Prepare : CopiumScript
{
    public CrewMenu crewManager;
    public Button closeButton;

    public Text prepareBody;
    public Button prepareButton1;
    public Button prepareButton2;
    public Text prepareOption1;
    public Text prepareOption2;

    public Crew harris;
    public Crew bronson;
    public Crew chuck;
    public Crew danton;

    public GameObject harrisPotrait;
    public GameObject bronsonPotrait;
    public GameObject chuckPotrait;
    public GameObject dantonPotrait;

    public Button harrisButton;
    public Button bronsonButton;
    public Button chuckButton;
    public Button dantonButton;
    public Text resultText;

    public GameObject prepareChoices;
    public GameObject prepareFinal;
    public GameObject prepareCanvas;


    public CrewMenu.Person currentCrewmate;
    public bool makeChoice;
    public int eventNum;
    public int choice;

    void Start()
    {

    }
    void Update()
    {
        if (makeChoice)
        {
            if (prepareButton1.state == ButtonState.OnClick)
            {
                choice = 1;
                GenerateResults();
            }
            else if (prepareButton2.state == ButtonState.OnClick)
            {
                choice = 2;
                GenerateResults();
            }
        }

        if (harrisButton.state == ButtonState.OnClick)
        {
            resultText.text = crewManager.crew[0].resultText;
        }
        else if (bronsonButton.state == ButtonState.OnClick)
        {
            resultText.text = crewManager.crew[1].resultText;
        }
        else if (chuckButton.state== ButtonState.OnClick)
        {
            resultText.text = crewManager.crew[2].resultText;
        }
        else if (dantonButton.state== ButtonState.OnClick)
        {
            resultText.text = crewManager.crew[3].resultText;
        }

        if (closeButton.state == ButtonState.OnClick)
        {
            prepareFinal.SetActive(false);
            prepareCanvas.SetActive(false);
        }
    }

    

    public void GenerateEvents(CrewMenu.Person crewmate)
    {
        prepareCanvas.SetActive(true);
        prepareChoices.SetActive(true);
        makeChoice= true;
        switch (crewmate.name)
        {
            case "Harris":
                Console.WriteLine("\n\n\nharris");
                harrisPotrait.SetActive(true);
                bronsonPotrait.SetActive(false); 
                chuckPotrait.SetActive(false); 
                dantonPotrait.SetActive(false);
                break;
            case "Bronson":
                Console.WriteLine("\n\n\n not harris");
                harrisPotrait.SetActive(false);
                bronsonPotrait.SetActive(true);
                chuckPotrait.SetActive(false);
                dantonPotrait.SetActive(false);
                break;
            case "Chuck":
                harrisPotrait.SetActive(false);
                bronsonPotrait.SetActive(false);
                chuckPotrait.SetActive(true);
                dantonPotrait.SetActive(false);
                break;
            case "Danton":
                harrisPotrait.SetActive(false);
                bronsonPotrait.SetActive(false);
                chuckPotrait.SetActive(false);
                dantonPotrait.SetActive(true);
                break;
            default:
                break;
        }

        currentCrewmate = crewmate;
        eventNum = RNG.RandInt(1,2);

        switch (eventNum)
        {
            case 1:
                prepareBody.text = "[" + crewmate.name + "] stumbled across a mysterious igloo, knowing that it is near impossible to " +
                                   "live in these harsh conditions, igloos are a rare sight.";
                prepareOption1.gameObject.SetActive(true);
                prepareOption2.gameObject.SetActive(true);
                prepareOption1.text = "Enter the igloo and look around";
                prepareOption2.text = "Leave the igloo alone";
                break;
            

            case 2:
                prepareBody.text = "While scavanging through the icy waste land, [" + crewmate.name + "] encountered a polar bear, the polar " +
                                    "bear bear its fangs at [" + crewmate.name + "]. It looks hungry and ready to attack.";
                prepareOption1.gameObject.SetActive(true);
                prepareOption2.gameObject.SetActive(true);
                prepareOption1.text = "Run!";
                prepareOption2.text = "Fight it off!";
                break;
            default:
                break;
        }
    }

    public void GenerateResults()
    {
        switch (eventNum)
        {
            case 1:
                if (choice == 1)
                {
                    crewManager.supplies += 5;
                    currentCrewmate.resultText = "Upon entering the igloo was the sight of a well preserved human corpse. " +
                                                 "Luckily there was still some dried frozen meat, ["+ currentCrewmate.name +"]" +
                                                 " took the meat and went their way.";
                }
                else if (choice == 2)
                {
                    currentCrewmate.resultText = "Knowing that it might be a trap, [" + currentCrewmate.name  +
                                                 "] decided to leave the igloo alone and head back to the train.";
                }
                break;
            case 2:
                if (choice == 1)
                {
                    currentCrewmate.resultText = "[" + currentCrewmate.name + "]bolted away from the polar bear back to the train. ";
                }
                else if (choice == 2)
                {
                    crewManager.supplies += 10;
                    currentCrewmate.health-=5;
                    currentCrewmate.resultText = "[" + currentCrewmate.name + "]becomes injured but manages to kill the polar bear, " +
                                                   "and gain some valuable food.";
                }
                break;
            default:
                break;
        }

        choice = 0;
        switch (currentCrewmate.name)
        {
            case "Harris":
                harris.isDeployed= false;
                crewManager.crew[0] = currentCrewmate;
                break;
            case "Bronson":
                bronson.isDeployed = false;
                crewManager.crew[1] = currentCrewmate;
                break;
            case "Chuck":
               chuck.isDeployed= false;
                crewManager.crew[2] = currentCrewmate;
                break;
            case "Danton":
               danton.isDeployed= false;
                crewManager.crew[3] = currentCrewmate;
                break;
            default
                : break;
        }
        crewManager.StartPrepare();

        if (!harris.isDeployed && !bronson.isDeployed && !chuck.isDeployed && !danton.isDeployed)
        {
            showFinal();
        }
    }

    public void showFinal()
    {
        prepareCanvas.SetActive(true);
        prepareChoices.SetActive(false);
        prepareFinal.SetActive(true);

        harrisButton.gameObject.SetActive(false);
        bronsonButton.gameObject.SetActive(false);
        chuckButton.gameObject.SetActive(false);
        dantonButton.gameObject.SetActive(false);

        if (crewManager.hDeploy)
        {
            harrisButton.gameObject.SetActive(true);
        }
        if (crewManager.bDeploy)
        {
            bronsonButton.gameObject.SetActive(true);
        }
        if (crewManager.cDeploy)
        {
            chuckButton.gameObject.SetActive(true);
        }
        if (crewManager.dDeploy)
        {
            dantonButton.gameObject.SetActive(true);
        }



    }
}
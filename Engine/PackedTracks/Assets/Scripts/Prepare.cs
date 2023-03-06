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
        else if (chuckButton.state == ButtonState.OnClick)
        {
            resultText.text = crewManager.crew[2].resultText;
        }
        else if (dantonButton.state == ButtonState.OnClick)
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
        currentCrewmate.resultText = "";

        prepareCanvas.SetActive(true);
        prepareChoices.SetActive(true);
        prepareOption1.gameObject.SetActive(false);
        prepareOption2.gameObject.SetActive(false);

        makeChoice = true;
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
        eventNum = RNG.RandInt(1,14);

        switch (eventNum)
        {
            case 1:
                prepareBody.text = crewmate.name + " stumbled across a mysterious igloo, knowing that it is near impossible to " +
                                   "live in these harsh conditions, igloos are a rare sight.";
                prepareOption1.gameObject.SetActive(true);
                prepareOption2.gameObject.SetActive(true);
                prepareOption1.text = "Enter the igloo and look around";
                prepareOption2.text = "Leave the igloo alone";
                break;
            

            case 2:
                prepareBody.text = "While scavanging through the icy waste land, " + crewmate.name + " encountered a polar bear, the polar " +
                                    "bear bear its fangs at " + crewmate.name + ". It looks hungry and ready to attack.";
                prepareOption1.gameObject.SetActive(true);
                prepareOption2.gameObject.SetActive(true);
                prepareOption1.text = "Run!";
                prepareOption2.text = "Fight it off!";
                break;

            case 3:
                prepareBody.text = "The search went poorly, in the poor icy conditions and limited visibility, " +
                                    crewmate.name+" seems to have gotten lost as he could not be seen returning to the meet up point. ";
                prepareOption1.gameObject.SetActive(true);
                prepareOption2.gameObject.SetActive(true);
                prepareOption1.text = "Search for " + crewmate.name;
                prepareOption2.text = "Wait a bit longer and hope for " + crewmate.name +" to return";
                break;
            case 4:
                prepareBody.text = "A abandoned town was spotted not far from the train, there might be some supplies left in the town, " +
                                   "but there might be scavenges there as well trying to get the last of its resources";
                prepareOption1.gameObject.SetActive(true);
                prepareOption2.gameObject.SetActive(true);
                prepareOption1.text = "Explore the town";
                prepareOption2.text = "Too dangerous, leave the town alone";
                break;
            case 5:
                prepareBody.text = "While searching for resources "+crewmate.name
                                    + " found a man in poor condition and seems to be passed out, he does not look like he " +
                                    "will survive any longer out in the cold.";
                prepareOption1.gameObject.SetActive(true);
                prepareOption2.gameObject.SetActive(true);
                prepareOption1.text = "Kill the man and take his meat for food";
                prepareOption2.text = "Take the man's belongings and supplies";
                break;
            case 6:
                prepareBody.text = crewmate.name + "Manage to caught sight of of a injured deer together with her fawn.";
                prepareOption1.gameObject.SetActive(true);
                prepareOption2.gameObject.SetActive(true);
                prepareOption1.text = "Kill the deer for food";
                prepareOption2.text = "Leave the deer alone";
                break;
            case 7:
                prepareBody.text = "Not too far from the train "+ crewmate.name + "found a flowing river, the river might contain some fish inside.";
                prepareOption1.gameObject.SetActive(true);
                prepareOption1.text = "Go fishing";
                break;
            case 8:
                prepareBody.text = "In the thick snow, " + crewmate.name + " miraculously found some firewood covered underneath a tarp.";
                prepareOption1.gameObject.SetActive(true);
                prepareOption1.text = "Continue";
                break;
            case 9:
                prepareBody.text = crewmate.name + " was trying to find some supplies when he saw a bunch of edible mushrooms growing on a rotting wood.";
                prepareOption1.gameObject.SetActive(true);
                prepareOption1.text = "Continue";
                break;
            case 10:
                prepareBody.text = crewmate.name + " manage to spot a old road from a far and there seem to be a few abandoned vehicles on it, " +
                                    "there might be some fuel that was left in the vehicle's tanks.";
                prepareOption1.gameObject.SetActive(true);
                prepareOption1.text = "Siphon the fuel";
                break;
            case 11:
                prepareBody.text = "Due to poor visibility and the icy conditions, " + crewmate.name +
                    " accidentally tripped and lost the supplies he was carrying on him.";
                prepareOption1.gameObject.SetActive(true);
                prepareOption1.text = "Continue";
                break;
            case 12:
                prepareBody.text = "While exploring a shallow cave near the train a frost quake occured, disorientating " + crewmate.name +
                    " causing him to fall and hit his head on the walls, suffering minor brain damage.";
                prepareOption1.gameObject.SetActive(true);
                prepareOption1.text = "Continue";
                break;
            case 13:
                prepareBody.text = "While " + crewmate.name + " was walking across a frozen river, he did not notice a patch of " +
                                   "thin ice and fell into the river, in a desperate attempt to get out and back to the train," +
                                    crewmate.name+" lost his supplies he was carrying with him.";
                prepareOption1.gameObject.SetActive(true);
                prepareOption1.text = "Continue";
                break;
            case 14:
                prepareBody.text = "While searching for resources " + crewmate.name + " chanced acrossed 2 Syberian wolves, they seem hungry and " +
                                   "desperate for food and were ready to attack";
                prepareOption1.gameObject.SetActive(true);
                prepareOption2.gameObject.SetActive(true);
                prepareOption1.text = "Feed the wolves [must have 3 supplies]";
                prepareOption2.text = "Kill the attacking wolves";
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
                                                 "Luckily there was still some dried frozen meat, " + currentCrewmate.name +
                                                 " took the meat and went their way.";
                }
                else if (choice == 2)
                {
                    currentCrewmate.resultText = "Knowing that it might be a trap, " + currentCrewmate.name  +
                                                 " decided to leave the igloo alone and head back to the train.";
                }
                break;
            case 2:
                if (choice == 1)
                {
                    currentCrewmate.resultText = currentCrewmate.name + " bolted away from the polar bear back to the train. ";
                }
                else if (choice == 2)
                {
                    crewManager.supplies += 10;
                    currentCrewmate.health-=5;
                    currentCrewmate.resultText = currentCrewmate.name + " becomes injured but manages to kill the polar bear, " +
                                                   "and gain some valuable food.";
                }
                break;
            case 3:
                if (choice == 1)
                {
                    crewManager.supplies -= 3;
                    currentCrewmate.resultText = "After hours of search, " + currentCrewmate.name + " was found in poor condition, " +
                                                 "but he was safely brought back to the train.";
                }
                else if (choice == 2)
                {
                    crewManager.supplies += 10;
                    currentCrewmate.health = 4;//critical injure
                    currentCrewmate.resultText = "Just as hope seem all lost for " + currentCrewmate.name+
                                                " to make it back, he stumbles out from the bushes grasping a few cans of soup. " +
                                                currentCrewmate.name + " seems to be in bad condition and his extremities have succumbed to frostbite.";
                }
                break;
            case 4:
                if (choice == 1)
                {
                    crewManager.supplies += 3;
                    currentCrewmate.health = 4;//critical injure
                    currentCrewmate.resultText = "While scavanging through the abandoned town, " +
                                                 currentCrewmate.name + " saw some canned food and bottles of water lying in a building. " +
                                                 "When he went to pick it up, he bumped into another scavanger, which promptly stabbed " +
                                                 currentCrewmate.name + " in the abdomen. "+currentCrewmate.name+" grabbed a metal pipe on the " +
                                                 "floor and swung the other guy's head, knocking him unconsious, before hurring off with the supplies.";
                }
                else if (choice == 2)
                {
                    currentCrewmate.resultText = "As the crew is leaving, they saw a group of armed scavengers running around the area, " +
                                                 "Bullet sounds could be heard in the distance.";
                }
                break;
            case 5:
                if (choice == 1)
                {
                    crewManager.supplies += 5;
                    currentCrewmate.resultText = currentCrewmate.name + " said his prayers before putting his knife to the man's throat, killing him. " +
                                                 currentCrewmate.name+" then took the man's supplies and skined him, cutting him up for meat to be consumed.";
                }
                else if (choice == 2)
                {
                    crewManager.supplies += 2;
                    currentCrewmate.resultText = currentCrewmate.name +" knocks the man unconsious and proceeds to take his supplies and left him in the " +
                                                 "snow before returning to the train.";
                }
                break;
            case 6:
                if (choice == 1)
                {
                    crewManager.supplies += 3;
                    currentCrewmate.resultText = currentCrewmate.name + " Took aim and shot at the deer, killing it in an instant, the confused fawn could only stand and stare as " +
                                                 currentCrewmate.name + "  skinned and carved the deer for its meet and fur.";
                }
                else if (choice == 2)
                {
                    currentCrewmate.resultText = "As "+ currentCrewmate.name + "walked away, a large polar bear jumped out of the surrounding bushes " +
                                                 "killing both the deer and her fawn in an instant.";
                }
                break;
            case 7:
                if (choice == 1)
                {
                    crewManager.supplies += 3;
                    currentCrewmate.resultText = currentCrewmate.name + " caught some fish.";
                }
                break;
            case 8:
                if (choice == 1)
                {
                    crewManager.supplies += 2;
                    currentCrewmate.resultText = currentCrewmate.name + " found some firewood.";
                }
                break;
            case 9:
                if (choice == 1)
                {
                    crewManager.supplies += 2;
                    currentCrewmate.resultText = currentCrewmate.name + " found some mushrooms.";
                }
                break;
            case 10:
                if (choice == 1)
                {
                    crewManager.supplies += 2;
                    currentCrewmate.resultText = currentCrewmate.name + " siphoned some fuel from some abandoned vehicles.";
                }
                break;
            case 11:
                if (choice == 1)
                {
                    crewManager.supplies -= 2;
                    currentCrewmate.resultText = currentCrewmate.name + " tripped and lost some supplies.";
                }
                break;
            case 12:
                if (choice == 1)
                {
                    currentCrewmate.health -= 5;
                    currentCrewmate.resultText = currentCrewmate.name + " fall and hit his head which resulted in minor damages";
                }
                break;
            case 13:
                if (choice == 1)
                {
                    crewManager.supplies -= 1;
                    currentCrewmate.resultText = currentCrewmate.name + " lost some supplies";
                }
                break;
            case 14:
                if (choice == 1)
                {
                    crewManager.supplies -= 3;
                    currentCrewmate.resultText = currentCrewmate.name + " gave out their rations to the hungry wolves, it seems like the wolves have taken a " +
                                                "liking to the crew and have started following them around.";
                }
                else if (choice == 2)
                {
                    crewManager.supplies += 2;
                    currentCrewmate.resultText = "The wolves jumped at " + currentCrewmate.name + ", attacking and trying to bite him. " +
                                                 currentCrewmate.name + " seeing no option killed both wolves and proceeded to skin and gather their meat for food.";
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
        if(!prepareFinal.activeSelf)
            resultText.text = "Click on a character icon to get more details on what they encountered.";
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
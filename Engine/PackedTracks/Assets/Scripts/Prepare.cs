/*!***************************************************************************************
\file			Prepare.cs
\project
\author			Shawn Tanary

\par			Course: GAM250
\par			Section:
\date			10/03/2023

\brief
    Contains the functions to display random events when sending crew out

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
using CopiumEngine;
using System;
using System.Linq;

public class Prepare : CopiumScript
{
    public CrewMenu crewManager;
    public Button closeButton;

    public TooltipBehaviour tooltip;

    public Text prepareBody;
    public Button prepareButton1;
    public Button prepareButton2;
    public Text prepareOption1;
    public Text prepareOption2;

    public Text suppliesChanged;
    public int suppliesChangedAmount;

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
    ButtonWrapper harrisBtnWrapper;
    ButtonWrapper bronsonBtnWrapper;
    ButtonWrapper chuckBtnWrapper;
    ButtonWrapper dantonBtnWrapper;
    public Text resultText;
    

    public GameObject prepareChoices;
    public GameObject prepareFinal;
    public GameObject prepareCanvas;

    public GameObject HHarrow;
    public GameObject HMarrow;
    public GameObject BHarrow;
    public GameObject BMarrow;
    public GameObject CHarrow;
    public GameObject CMarrow;
    public GameObject DHarrow;
    public GameObject DMarrow;


    public Person[] currentCrewmate;
    public bool makeChoice;
    public int eventNum;
    public int choice;
    public int numOfCrew = 0;

    bool option1Hover = false;
    bool option2Hover = false;

    void Start()
    {
        harrisBtnWrapper = new ButtonWrapper(harrisButton,crewManager.audioManager,tooltip);
        harrisBtnWrapper.SetImage(harrisButton.GetComponent<Image>());
        bronsonBtnWrapper = new ButtonWrapper(bronsonButton,crewManager.audioManager,tooltip);
        bronsonBtnWrapper.SetImage(bronsonButton.GetComponent<Image>());
        chuckBtnWrapper = new ButtonWrapper(chuckButton,crewManager.audioManager,tooltip);
        chuckBtnWrapper.SetImage(chuckButton.GetComponent<Image>());
        dantonBtnWrapper = new ButtonWrapper(dantonButton,crewManager.audioManager,tooltip);
        dantonBtnWrapper.SetImage(dantonButton.GetComponent<Image>());
    }
    void Update()
    {
        if (makeChoice)
        {
            if (prepareButton1.state == ButtonState.OnClick)
            {
                crewManager.audioManager.clickSFX.Play();
                choice = 1;
                GenerateResults();
            }
            else if (prepareButton1.state == ButtonState.None)
            {
                option1Hover = false;
            }

            if (prepareButton2.state == ButtonState.OnHover)
            {                
                if (!option2Hover)
                {
                    option2Hover = true;
                    crewManager.audioManager.hoverSFX.Play();
                }
            }
            else if (prepareButton2.state == ButtonState.OnClick)
            {
                crewManager.audioManager.clickSFX.Play();
                choice = 2;
                GenerateResults();
            }
            else if (prepareButton2.state == ButtonState.None)
            {
                option2Hover = false;
            }
        }

        /*
        if (harrisBtnWrapper.GetState() == ButtonState.OnClick)
        {
            resultText.text = crewManager.crew[0].resultText;
        }
        else if (bronsonBtnWrapper.GetState() == ButtonState.OnClick)
        {
            resultText.text = crewManager.crew[1].resultText;
        }
        else if (chuckBtnWrapper.GetState() == ButtonState.OnClick)
        {
            resultText.text = crewManager.crew[2].resultText;
        }
        else if (dantonBtnWrapper.GetState() == ButtonState.OnClick)
        {
            resultText.text = crewManager.crew[3].resultText;
        }
        */
    }


    /*******************************************************************************
	/*!
	    \brief
		    Generates a random event for the target crewmate
	*/
    /*******************************************************************************/
    public void GenerateEvents(Person[] crewmate)
    {
        currentCrewmate = crewmate;
        numOfCrew = 0;
        for (int i = 0; i < currentCrewmate.Length; i++, numOfCrew++)
        {
            if (currentCrewmate[i] == null) { break; }
        }
        prepareCanvas.SetActive(true);
        
        prepareChoices.SetActive(true);
        prepareFinal.SetActive(false);
        
        prepareOption1.gameObject.SetActive(false);
        prepareOption2.gameObject.SetActive(false);
        makeChoice = true;

        harrisPotrait.SetActive(false);
        bronsonPotrait.SetActive(false);
        chuckPotrait.SetActive(false);
        dantonPotrait.SetActive(false);

        for (int i = 0; i < crewmate.Length; i++)
        {
            if (crewmate[i] == null)
            {
                break;
            }
            if (crewmate[i].name =="Harris")
            {
                harrisPotrait.SetActive(true);
            }
            else if (crewmate[i].name == "Bronson")
            {
                bronsonPotrait.SetActive(true);
            }
            else if (crewmate[i].name == "Chuck")
            {
                chuckPotrait.SetActive(true);
            }
            else if (crewmate[i].name == "Danton")
            {
                dantonPotrait.SetActive(true);
            }
        }
        
        eventNum = RNG.Range(1,1);

        switch (eventNum)
        {
            case 1:
                if (numOfCrew == 1)
                {
                    prepareBody.text = currentCrewmate[0].name;
                }
                else if (numOfCrew >1)
                {
                    prepareBody.text = "The Crew";
                }

                prepareBody.text += " stumbled across a mysterious igloo, knowing that it is near impossible to " +
                                   "live in these harsh conditions, igloos are a rare sight.";
                prepareOption1.gameObject.SetActive(true);
                prepareOption2.gameObject.SetActive(true);
                prepareOption1.text = "Enter the igloo and look around";
                prepareOption2.text = "Leave the igloo alone";
                break;
            

            case 2:
                prepareBody.text = "While scavanging through the icy waste land, " + "the crew" + " encountered a polar bear, the polar " +
                                    "bear bear its fangs at " + "the crew" + ". It looks hungry and ready to attack.";
                prepareOption1.gameObject.SetActive(true);
                prepareOption2.gameObject.SetActive(true);
                prepareOption1.text = "Run!";
                prepareOption2.text = "Fight it off!";
                break;

            case 3:
                prepareBody.text = "The search went poorly, in the poor icy conditions and limited visibility, " +
                                    "the crew"+" seems to have gotten lost as he could not be seen returning to the meet up point. ";
                prepareOption1.gameObject.SetActive(true);
                prepareOption2.gameObject.SetActive(true);
                prepareOption1.text = "Search for " + "the crew";
                prepareOption2.text = "Wait a bit longer and hope for " + "the crew" +" to return";
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
                prepareBody.text = "While searching for resources "+"the crew"
                                    + " found a man in poor condition and seems to be passed out, he does not look like he " +
                                    "will survive any longer out in the cold.";
                prepareOption1.gameObject.SetActive(true);
                prepareOption2.gameObject.SetActive(true);
                prepareOption1.text = "Kill the man and take his meat for food";
                prepareOption2.text = "Take the man's belongings and supplies";
                break;
            case 6:
                prepareBody.text = "the crew" + "Manage to caught sight of of a injured deer together with her fawn.";
                prepareOption1.gameObject.SetActive(true);
                prepareOption2.gameObject.SetActive(true);
                prepareOption1.text = "Kill the deer for food";
                prepareOption2.text = "Leave the deer alone";
                break;
            case 7:
                prepareBody.text = "Not too far from the train "+ "the crew" + "found a flowing river, the river might contain some fish inside.";
                prepareOption1.gameObject.SetActive(true);
                prepareOption1.text = "Go fishing";
                break;
            case 8:
                prepareBody.text = "In the thick snow, " + "the crew" + " miraculously found some firewood covered underneath a tarp.";
                prepareOption1.gameObject.SetActive(true);
                prepareOption1.text = "Continue";
                break;
            case 9:
                prepareBody.text = "the crew" + " was trying to find some supplies when he saw a bunch of edible mushrooms growing on a rotting wood.";
                prepareOption1.gameObject.SetActive(true);
                prepareOption1.text = "Continue";
                break;
            case 10:
                prepareBody.text = "the crew" + " manage to spot a old road from a far and there seem to be a few abandoned vehicles on it, " +
                                    "there might be some fuel that was left in the vehicle's tanks.";
                prepareOption1.gameObject.SetActive(true);
                prepareOption1.text = "Siphon the fuel";
                break;
            case 11:
                prepareBody.text = "Due to poor visibility and the icy conditions, " + "the crew" +
                    " accidentally tripped and lost the supplies he was carrying on him.";
                prepareOption1.gameObject.SetActive(true);
                prepareOption1.text = "Continue";
                break;
            case 12:
                prepareBody.text = "While exploring a shallow cave near the train a frost quake occured, disorientating " + "the crew" +
                    " causing him to fall and hit his head on the walls, suffering minor brain damage.";
                prepareOption1.gameObject.SetActive(true);
                prepareOption1.text = "Continue";
                break;
            case 13:
                prepareBody.text = "While " + "the crew" + " was walking across a frozen river, he did not notice a patch of " +
                                   "thin ice and fell into the river, in a desperate attempt to get out and back to the train," +
                                    "the crew"+" lost his supplies he was carrying with him.";
                prepareOption1.gameObject.SetActive(true);
                prepareOption1.text = "Continue";
                break;
            case 14:
                prepareBody.text = "While searching for resources " + "the crew" + " chanced acrossed 2 Syberian wolves, they seem hungry and " +
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

    /*******************************************************************************
	/*!
	    \brief
		    Gives the result text and sets the effects of the choices made for the
            random event
	*/
    /*******************************************************************************/
    public void GenerateResults()
    {
        HHarrow.SetActive(false);
        HMarrow.SetActive(false);
        BHarrow.SetActive(false);
        BMarrow.SetActive(false);
        CHarrow.SetActive(false);
        CMarrow.SetActive(false);
        DHarrow.SetActive(false);
        DMarrow.SetActive(false);
        suppliesChangedAmount= 0;

        
        //which random crewmate gets affected
        int randomCrewmate = RNG.Range(0, numOfCrew - 1);

        //roll chance for event choice successs / for choices check if choiceNum<=difficulty
        int choiceNum = RNG.Range(1, 10);

        //always use currentCrewmate[0].resultText cause there has to be at least 1 deployed. The name can be currentCrewmate[randomCrewmate].name
        switch (eventNum)
        {
            case 1:
                if (choice == 1)
                {
                    if (numOfCrew == 1)
                    {
                        if (choiceNum <= 6)
                        {
                            crewManager.ChangeSupplies(5);
                            currentCrewmate[0].resultText = "Upon entering the igloo, " + currentCrewmate[randomCrewmate].name + " sees a well preserved human corpse. " +
                                                     "Luckily there was still some dried frozen meat, " + currentCrewmate[randomCrewmate].name +
                                                     " took the meat and left the igloo.";
                        }
                        else
                        {
                            currentCrewmate[0].resultText = "Upon entering the igloo, it seems that the igloo was recently abandoned. Nothing of note can be salvaged.";
                        }
                    }
                    else if (numOfCrew > 1)
                    {
                        if (choiceNum <= 8)
                        {
                            crewManager.ChangeSupplies(5);
                            currentCrewmate[0].resultText = "Upon entering the igloo, " + currentCrewmate[randomCrewmate].name + " sees a well preserved human corpse. " +
                                                     "Luckily there was still some dried frozen meat, " + currentCrewmate[randomCrewmate].name +
                                                     " took the meat and left the igloo.";
                        }
                        else
                        {
                            currentCrewmate[0].resultText = "Upon entering the igloo, it seems that the igloo was recently abandoned. Nothing of note can be salvaged.";
                        }
                    }
                }
                else if (choice == 2)
                {
                    currentCrewmate[0].resultText = "Knowing that it might be a trap, " + currentCrewmate[randomCrewmate].name  +
                                                 " decided to leave the igloo alone and head back to the train.";
                }
                break;
            case 2:
                if (choice == 1)
                {
                    currentCrewmate[0].resultText = currentCrewmate[randomCrewmate].name + " bolted away from the polar bear back to safe confines of the train. ";
                }
                else if (choice == 2)
                {
                    crewManager.ChangeSupplies(10);
                    crewManager.ChangeCrew(CrewMenu.STAT_TYPES.HEALTH, currentCrewmate[randomCrewmate].name, -1);
                    changeSummaryArrow(false,true, currentCrewmate[randomCrewmate]);
                    currentCrewmate[0].resultText = currentCrewmate[randomCrewmate].name + " suffered some injuries but managed to kill the polar bear, " +
                                                   "and gain some valuable food.";
                }
                break;
            case 3:
                if (choice == 1)
                {
                    crewManager.ChangeSupplies(-3);
                    currentCrewmate[0].resultText = "After hours of searching, " + currentCrewmate[randomCrewmate].name + " was found alive but worse for wear, " +
                                                 "he was safely brought back to the train.";
                }
                else if (choice == 2)
                {
                    crewManager.ChangeSupplies(10);
                    crewManager.SetCrew(CrewMenu.STAT_TYPES.HEALTH, currentCrewmate[randomCrewmate].name, 1);
                    changeSummaryArrow(false, true, currentCrewmate[randomCrewmate]);
                    currentCrewmate[0].resultText = "Just as hope seemed all lost for " + currentCrewmate[randomCrewmate].name +
                                                " to make it back, he stumbles out from the bushes grasping a few cans of soup. " +
                                                currentCrewmate[randomCrewmate].name + " seems to be in bad condition and his extremities have succumbed to frostbite.";
                }
                break;
            case 4:
                if (choice == 1)
                {
                    crewManager.ChangeSupplies(3);
                    crewManager.SetCrew(CrewMenu.STAT_TYPES.HEALTH, currentCrewmate[randomCrewmate].name, 1);
                    changeSummaryArrow(false, true, currentCrewmate[randomCrewmate]);
                    currentCrewmate[0].resultText = "While scavanging through the abandoned town, " +
                                                 currentCrewmate[randomCrewmate].name + " saw some canned food and bottles of water lying in a building. " +
                                                 "When he went to pick it up, he bumped into another scavenger, who promptly stabbed " +
                                                 currentCrewmate[randomCrewmate].name + " in the abdomen. "+ currentCrewmate[randomCrewmate].name+" grabbed a metal pipe on the " +
                                                 "floor and swung at the scavengers's head, knocking him unconscious, before hurrying off with the supplies.";
                }
                else if (choice == 2)
                {
                    currentCrewmate[0].resultText = "As the crew was leaving, they notice a group of armed scavengers running around the area, " +
                                                 "Bullet sounds could be heard in the distance.";
                }
                break;
            case 5:
                if (choice == 1)
                {
                    crewManager.ChangeSupplies(5);
                    currentCrewmate[0].resultText = currentCrewmate[randomCrewmate].name + " said his prayers before putting his knife to the man's throat, killing him. " +
                                                 currentCrewmate[randomCrewmate].name+" then took the man's supplies and skinned him, cutting him up for meat to be consumed.";
                }
                else if (choice == 2)
                {
                    crewManager.ChangeSupplies(2);
                    currentCrewmate[0].resultText = currentCrewmate[0].name +" knocks the man unconscious, proceeds to take his supplies and left him in the " +
                                                 "snow before returning to the train.";
                }
                break;
            case 6:
                if (choice == 1)
                {
                    crewManager.ChangeSupplies(3);
                    currentCrewmate[0].resultText = currentCrewmate[randomCrewmate].name + " took aim and shot at the deer, killing it in an instant, its confused fawn could only stand and stare as " +
                                                 currentCrewmate[randomCrewmate].name + "  skinned and carved the deer for its meat and fur.";
                }
                else if (choice == 2)
                {
                    currentCrewmate[0].resultText = "As "+ currentCrewmate[randomCrewmate].name + " walked away, a large polar bear jumped out of the surrounding bushes " +
                                                 "killing both the deer and her fawn in an instant.";
                }
                break;
            case 7:
                if (choice == 1)
                {
                    crewManager.ChangeSupplies(3);
                    currentCrewmate[0].resultText = currentCrewmate[randomCrewmate].name + " caught some fish.";
                }
                break;
            case 8:
                if (choice == 1)
                {
                    crewManager.ChangeSupplies(2);
                    currentCrewmate[0].resultText = currentCrewmate[randomCrewmate].name + " found some firewood.";
                }
                break;
            case 9:
                if (choice == 1)
                {
                    crewManager.ChangeSupplies(2);
                    currentCrewmate[0].resultText = currentCrewmate[randomCrewmate].name + " found some mushrooms.";
                }
                break;
            case 10:
                if (choice == 1)
                {
                    crewManager.ChangeSupplies(2);
                    currentCrewmate[0].resultText = currentCrewmate[randomCrewmate].name + " siphoned some fuel from some abandoned vehicles.";
                }
                break;
            case 11:
                if (choice == 1)
                {
                    crewManager.ChangeSupplies(-2);
                    currentCrewmate[0].resultText = currentCrewmate[randomCrewmate].name + " tripped and lost some supplies.";
                }
                break;
            case 12:
                if (choice == 1)
                {
                    crewManager.ChangeCrew(CrewMenu.STAT_TYPES.HEALTH, currentCrewmate[randomCrewmate].name, -1);
                    changeSummaryArrow(false, true, currentCrewmate[randomCrewmate]);
                    currentCrewmate[0].resultText = currentCrewmate[randomCrewmate].name + " fell and hit his head which resulted in minor injuries";
                }
                break;
            case 13:
                if (choice == 1)
                {
                    crewManager.ChangeSupplies(-1);
                    currentCrewmate[0].resultText = currentCrewmate[randomCrewmate].name + " lost some supplies";
                }
                break;
            case 14:
                if (choice == 1)
                {
                    crewManager.ChangeSupplies(-3);
                    currentCrewmate[0].resultText = currentCrewmate[randomCrewmate].name + " gave their rations to the hungry wolves, it seems like the wolves have taken a " +
                                                "liking to the crew and have started following them around.";
                }
                else if (choice == 2)
                {
                    crewManager.ChangeSupplies(2);
                    currentCrewmate[0].resultText = "The wolves jumped at " + currentCrewmate[randomCrewmate].name + ", attacking and trying to bite him. " +
                                                 currentCrewmate[randomCrewmate].name + " seeing no other option, killed both wolves and proceeded to skin and gather their meat for food.";
                }
                break;
            default:
                break;
        }

        choice = 0;
        for (int i = 0; i < currentCrewmate.Length; i++)
        {
            if (currentCrewmate[i] == null)
            {
                break;
            }
            switch (currentCrewmate[i].name)
            {
                case "Harris":
                    harris.isDeployed = false;
                    harris.Disable();
                    crewManager.crew[0] = currentCrewmate[i];
                    break;
                case "Bronson":
                    bronson.isDeployed = false;
                    bronson.Disable();
                    crewManager.crew[1] = currentCrewmate[i];
                    break;
                case "Chuck":
                    chuck.isDeployed = false;
                    chuck.Disable();
                    crewManager.crew[2] = currentCrewmate[i];
                    break;
                case "Danton":
                    danton.isDeployed = false;
                    danton.Disable();
                    crewManager.crew[3] = currentCrewmate[i];
                    break;
                default
                :
                    break;
            }
        }
        //crewManager.StartPrepare();

        if (!harris.isDeployed && !bronson.isDeployed && !chuck.isDeployed && !danton.isDeployed)
        {
            showFinal();
        }
    }

    /*******************************************************************************
	/*!
	    \brief
		    Displays the result text andlets you swap crewmates to see their results    
	*/
    /*******************************************************************************/
    public void showFinal()
    {
        if(!prepareFinal.activeSelf)
            resultText.text = "Click on a character icon to get more details on what they encountered.";
        prepareCanvas.SetActive(true);
        prepareChoices.SetActive(false);
        prepareFinal.SetActive(true);

        changeSuppliesText(suppliesChangedAmount);
        harrisButton.gameObject.SetActive(false);
        bronsonButton.gameObject.SetActive(false);
        chuckButton.gameObject.SetActive(false);
        dantonButton.gameObject.SetActive(false);

        resultText.text = currentCrewmate[0].resultText;
        /*
        if (!crewManager.hDeploy)
        {
            if(crewManager.harris.person.alive)
                crewManager.harris.person.resultText = "Harris is just chilling in the back";
            
            //harrisButton.gameObject.SetActive(true);
        }
        if (!crewManager.bDeploy)
        {
            if(crewManager.bronson.person.alive)
                crewManager.bronson.person.resultText = "Bronson is just chilling in the back";
            //bronsonButton.gameObject.SetActive(true);
        }
        if (!crewManager.cDeploy)
        {
            if(crewManager.chuck.person.alive)
                crewManager.chuck.person.resultText = "Chuck is just chilling in the back";
            //chuckButton.gameObject.SetActive(true);
        }
        if (!crewManager.dDeploy)
        {
            if(crewManager.danton.person.alive)
                crewManager.danton.person.resultText = "Danton is just chilling in the back";
            //dantonButton.gameObject.SetActive(true);
        }
        */
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


    public void changeSuppliesText(int amount)
    {
        if (amount == 0)
        {
            suppliesChanged.color = Color.white;
        }
        else if (amount >0)
        {
            suppliesChanged.color = Color.green;
        }
        else if (amount<0)
        {
            suppliesChanged.color = Color.red;
        }
        suppliesChanged.text = amount.ToString();
    }

    /*******************************************************************************
	/*!
	    \brief
		    Rotates the summary arrows based on the params
            
        \param increased
			Rotates the arrow up or down depending on increase
		\param health
			If Health is false change the mental instead
        \param crew
			Which crew member is affected
	*/
    /*******************************************************************************/
    public void changeSummaryArrow(bool increased, bool health,Person crew)
    {
        Vector3 temp;
        if (crew.name == "Harris")
        {
            
            if (health)
            {
                HHarrow.SetActive(true);
                if (increased)
                {
                    temp = HHarrow.transform.localRotation;
                    temp.z = 90;
                    HHarrow.transform.localRotation = temp;
                }
                else
                {
                    temp = HHarrow.transform.localRotation;
                    temp.z = -90;
                    HHarrow.transform.localRotation = temp;
                }
            }
            else 
            {
                HMarrow.SetActive(true);
                if (increased)
                {
                    temp = HMarrow.transform.localRotation;
                    temp.z = 90;
                    HMarrow.transform.localRotation = temp;
                }
                else
                {
                    temp = HMarrow.transform.localRotation;
                    temp.z = -90;
                    HMarrow.transform.localRotation = temp;
                }
            }
        }
        else if (crew.name == "Bronson")
        {
            if (health)
            {
                BHarrow.SetActive(true);
                if (increased)
                {
                    temp = BHarrow.transform.localRotation;
                    temp.z = 90;
                    BHarrow.transform.localRotation = temp;
                }
                else
                {
                    temp = BHarrow.transform.localRotation;
                    temp.z = -90;
                    BHarrow.transform.localRotation = temp;
                }
            }
            else
            {
                BMarrow.SetActive(true);
                if (increased)
                {
                    temp = BMarrow.transform.localRotation;
                    temp.z = 90;
                    BMarrow.transform.localRotation = temp;
                }
                else
                {
                    temp = BMarrow.transform.localRotation;
                    temp.z = -90;
                    BMarrow.transform.localRotation = temp;
                }
            }
        }
        else if (crew.name == "Chuck")
        {
            if (health)
            {
                CHarrow.SetActive(true);
                if (increased)
                {
                    temp = CHarrow.transform.localRotation;
                    temp.z = 90;
                    CHarrow.transform.localRotation = temp;
                }
                else
                {
                    temp = CHarrow.transform.localRotation;
                    temp.z = -90;
                    CHarrow.transform.localRotation = temp;
                }
            }
            else
            {
                CMarrow.SetActive(true);
                if (increased)
                {
                    temp = CMarrow.transform.localRotation;
                    temp.z = 90;
                    CMarrow.transform.localRotation = temp;
                }
                else
                {
                    temp = CMarrow.transform.localRotation;
                    temp.z = -90;
                    CMarrow.transform.localRotation = temp;
                }
            }
        }
        else if (crew.name == "Danton")
        {
            if (health)
            {
                DHarrow.SetActive(true);
                if (increased)
                {
                    temp = DHarrow.transform.localRotation;
                    temp.z = 90;
                    DHarrow.transform.localRotation = temp;
                }
                else
                {
                    temp = DHarrow.transform.localRotation;
                    temp.z = -90;
                    DHarrow.transform.localRotation = temp;
                }
            }
            else
            {
                DMarrow.SetActive(true);
                if (increased)
                {
                    temp = DMarrow.transform.localRotation;
                    temp.z = 90;
                    DMarrow.transform.localRotation = temp;
                }
                else
                {
                    temp = DMarrow.transform.localRotation;
                    temp.z = -90;
                    DMarrow.transform.localRotation = temp;
                }
            }
        }

    }
}
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
using System.Runtime.InteropServices;

public class Prepare : CopiumScript
{
    public CrewMenu crewManager;
    public Button closeButton;

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

    //which random crewmate gets affected
    int randomCrewmate = 0;
    void Start()
    {
        harrisBtnWrapper = new ButtonWrapper(harrisButton);
        harrisBtnWrapper.SetImage(harrisButton.GetComponent<Image>());
        bronsonBtnWrapper = new ButtonWrapper(bronsonButton);
        bronsonBtnWrapper.SetImage(bronsonButton.GetComponent<Image>());
        chuckBtnWrapper = new ButtonWrapper(chuckButton);
        chuckBtnWrapper.SetImage(chuckButton.GetComponent<Image>());
        dantonBtnWrapper = new ButtonWrapper(dantonButton);
        dantonBtnWrapper.SetImage(dantonButton.GetComponent<Image>());
    }
    void Update()
    {
        if (makeChoice)
        {
            if (prepareButton1.state == ButtonState.OnClick)
            {
                AudioManager.Instance.clickSFX.Play();
                choice = 1;
                GenerateResults();
            }
            else if (prepareButton1.state == ButtonState.None)
            {
                if(option1Hover)
                    option1Hover = false;
            }

            if (prepareButton2.state == ButtonState.OnHover)
            {                
                if (!option2Hover)
                {
                    option2Hover = true;
                    AudioManager.Instance.hoverSFX.Play();
                }
            }
            else if (prepareButton2.state == ButtonState.OnClick)
            {
                AudioManager.Instance.clickSFX.Play();
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

    //if an event cant happen go to here
    Reroll:
        eventNum = RNG.Range(1, 14);
        randomCrewmate = RNG.Range(0, numOfCrew - 1);
        string tempS = "";
        switch (eventNum)
        {
            case 1:
                if (numOfCrew == 1)
                {
                    tempS = currentCrewmate[randomCrewmate].name;
                }
                else if (numOfCrew >1)
                {
                    tempS = "The crew";
                }

                prepareBody.text = tempS + " stumbled across a mysterious igloo, knowing that it is near impossible to " +
                                   "live in these harsh conditions, igloos are a rare sight.";
                prepareOption1.gameObject.SetActive(true);
                prepareOption2.gameObject.SetActive(true);
                prepareOption1.text = "Enter the igloo and look around";
                prepareOption2.text = "Leave the igloo alone";
                break;
            

            case 2:
                if (numOfCrew == 1)
                {
                    tempS = currentCrewmate[randomCrewmate].name;
                }
                else if (numOfCrew > 1)
                {
                    tempS = "the crew";
                }

                prepareBody.text = "While scavanging through the icy waste land, " + tempS + " encountered a polar bear, the polar " +
                                    "bear bear its fangs at " + tempS + ". It looks hungry and ready to attack.";
                prepareOption1.gameObject.SetActive(true);
                prepareOption2.gameObject.SetActive(true);
                prepareOption1.text = "Run!";
                prepareOption2.text = "Fight it off!";
                break;

            case 3:
                if (numOfCrew == 1) //if alone
                {
                    prepareBody.text = "The search went poorly, in the poor icy conditions and limited visibility, " +
                                    currentCrewmate[randomCrewmate].name + " seems to have gotten lost as he could not be seen returning to the meet up point. ";
                    prepareOption1.gameObject.SetActive(true);
                    prepareOption2.gameObject.SetActive(true);
                    prepareOption1.text = "Search for " + currentCrewmate[0].name;
                    prepareOption2.text = "Wait a bit longer and hope for " + currentCrewmate[0].name + " to return";
                }
                else
                {
                    prepareBody.text = "Due to poor icy conditions and limited visibility, the crew " +
                                       "decided to to quickly head back to the train and before anything dangerous happens. ";
                    prepareOption1.gameObject.SetActive(true);
                    prepareOption1.text = "Turn back now";
                }
                break;
            case 4:
                if (numOfCrew == 1)
                {
                    tempS = currentCrewmate[0].name;
                }
                else if (numOfCrew > 1)
                {
                    tempS = "the crew";
                }
                prepareBody.text = "An abandoned town was spotted not far from the train by "+ tempS +
                                   ", there might be some supplies left in the town, but there may be scavengers there too.";
                prepareOption1.gameObject.SetActive(true);
                prepareOption2.gameObject.SetActive(true);
                prepareOption1.text = "Explore the town";
                prepareOption2.text = "Too dangerous, leave the town alone";
                break;
            case 5:
                prepareBody.text = "While searching for resources the crew finds a man in critical condition. He is unconscious " +
                                    "and does not look like he will survive much longer out in the cold.";
                prepareOption1.gameObject.SetActive(true);
                prepareOption2.gameObject.SetActive(true);
                prepareOption1.text = "Kill the man and take his meat for food";
                prepareOption2.text = "Take the man's belongings and supplies";
                break;
            case 6:
                prepareBody.text = "The crew" + " catches sight of an injured deer with her fawn at her side.";
                prepareOption1.gameObject.SetActive(true);
                prepareOption2.gameObject.SetActive(true);
                prepareOption1.text = "Kill the deer for food";
                prepareOption2.text = "Leave the deer alone";
                break;
            case 7:
                if (numOfCrew<2)
                {
                    Console.WriteLine("Rerolled");
                    goto Reroll;
                }
                prepareBody.text = "As the crew navigated the icy forest next to the train, they encounter the remains of a camp. The camp is devoid " +
                                   "of life, but supplies have been left strewn around. The crew quickly gather what supplies they can and return to the train.";
                prepareOption1.gameObject.SetActive(true);
                prepareOption1.text = "Everybody carries back what they can";
                break;
            case 8:
                prepareBody.text = "In the thick snow, " + "the crew" + " miraculously found some firewood covered underneath a tarp.";
                prepareOption1.gameObject.SetActive(true);
                prepareOption1.text = "Continue";
                break;
            case 9:
                if (numOfCrew > 1)
                {
                    Console.WriteLine("Rerolled");
                    goto Reroll;
                }
                prepareBody.text = currentCrewmate[randomCrewmate].name + " was trying to find some supplies when he saw a bunch of mushrooms growing on a block of rotting wood.";
                prepareOption1.gameObject.SetActive(true);
                prepareOption2.gameObject.SetActive(true);
                prepareOption1.text = "Cook a couple of mushrooms to test them";
                prepareOption2.text = "Leave the mushrooms behind";
                break;
            case 10:
                prepareBody.text = "The crew" + " managed to spot an old road from afar and there seem to be abandoned vehicles on it, " +
                                    "there may be some fuel left in the vehicles' tanks.";
                prepareOption1.gameObject.SetActive(true);
                prepareOption1.text = "Siphon the fuel";
                break;
            case 11:
                prepareBody.text = "In a bid to get back to the train quicker, "+ currentCrewmate[randomCrewmate].name + " opted to walk across a frozen stream. While doing so, " +
                                   "the ice beneath his feet gave way, plunging him into the icy waters. Thankfully, " +
                                   currentCrewmate[randomCrewmate].name + "was able to get ashore but was forced to let go of his bag of supplies, losing it to the depths of the stream.";
                prepareOption1.gameObject.SetActive(true);
                prepareOption1.text = "Continue";
                break;
            case 12:
                prepareBody.text = "While exploring a shallow cave near the train a frost quake occured, disorientating " + currentCrewmate[randomCrewmate].name +
                                   " causing him to fall and hit his head on the walls, suffering minor brain damage.";
                prepareOption1.gameObject.SetActive(true);
                prepareOption1.text = "Ouch";
                break;
            case 13:
                prepareBody.text = currentCrewmate[randomCrewmate].name +" managed to find a first aid kit that is in usable condition buried under some rubble. " +
                                   "He decided to use it to patch himself up.";
                prepareOption1.gameObject.SetActive(true);
                prepareOption1.text = "Continue";
                break;
            case 14:
                prepareBody.text = currentCrewmate[randomCrewmate].name + " spots a group of snow bunnies hopping around. He decided to relax for a little bit and just watch the rabbits.";
                prepareOption1.gameObject.SetActive(true);
                prepareOption1.text = "Watch the bunnies";
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
                    if (numOfCrew==1) //if alone
                    {
                        if (choiceNum <= 8)
                        {
                            crewManager.ChangeSupplies(5);
                            currentCrewmate[0].resultText = currentCrewmate[randomCrewmate].name + " bolted away from the polar bear back to safe confines of the train. ";
                        }
                        else
                        {
                            currentCrewmate[0].resultText = currentCrewmate[randomCrewmate].name + "barely manages to escape, only suffering minor injuries.";
                        }
                    }
                    else
                    {
                        if (choiceNum <= 7)
                        {
                            crewManager.ChangeSupplies(5);
                            currentCrewmate[0].resultText = currentCrewmate[randomCrewmate].name + " bolted away from the polar bear back to safe confines of the train. ";
                        }
                        else
                        {
                            currentCrewmate[0].resultText = currentCrewmate[randomCrewmate].name + "barely manages to escape, only suffering minor injuries.";
                        }
                    }
                   
                }
                else if (choice == 2)
                {

                    if (numOfCrew == 1)// if alone
                    {
                        if (choiceNum <= 3)
                        {
                            crewManager.ChangeSupplies(10);
                            crewManager.ChangeHealth(currentCrewmate[randomCrewmate].name, -1);
                            changeSummaryArrow(false, true, currentCrewmate[randomCrewmate]);
                            currentCrewmate[0].resultText = currentCrewmate[randomCrewmate].name + " suffered some injuries but managed to kill the polar bear, " +
                                                            "and gain some valuable food.";
                        }
                        else
                        {
                            crewManager.ChangeHealth(currentCrewmate[randomCrewmate].name, -1);
                            changeSummaryArrow(false, true, currentCrewmate[randomCrewmate]);
                            currentCrewmate[0].resultText = currentCrewmate[randomCrewmate].name + "attempts to fight off the polar bear but is unsuccesful and runs away.";
                        }
                    }
                    else
                    {
                        if (choiceNum <= 7)
                        {
                            crewManager.ChangeSupplies(10);
                            crewManager.ChangeHealth(currentCrewmate[randomCrewmate].name, -1);
                            changeSummaryArrow(false, true, currentCrewmate[randomCrewmate]);
                            currentCrewmate[0].resultText = currentCrewmate[randomCrewmate].name + " suffered some injuries but managed to kill the polar bear, " +
                                                            "and gain some valuable food.";
                        }
                        else
                        {
                            crewManager.ChangeHealth(currentCrewmate[randomCrewmate].name, -1);
                            changeSummaryArrow(false, true, currentCrewmate[randomCrewmate]);
                            currentCrewmate[0].resultText = currentCrewmate[randomCrewmate].name + "attempts to fight off the polar bear but is unsuccesful and runs away.";
                        }
                    }
                }
                break;
            case 3:
                if (choice == 1)
                {
                    if (numOfCrew == 1)
                    {
                        crewManager.ChangeSupplies(-3);
                        currentCrewmate[0].resultText = "After hours of searching, " + currentCrewmate[randomCrewmate].name + " was found alive but worse for wear, " +
                                                     "he was safely brought back to the train.";

                    }
                    else
                    {
                        currentCrewmate[0].resultText = "Thankfully the crew managed to get back right before the weather got any worse.";
                    }

                }
                else if (choice == 2)
                {
                    crewManager.ChangeSupplies(10);
                    crewManager.SetStat(currentCrewmate[randomCrewmate].name, HEALTH_STATE.CRITICAL);
                    changeSummaryArrow(false, true, currentCrewmate[randomCrewmate]);
                    currentCrewmate[0].resultText = "Just as hope seemed all lost for " + currentCrewmate[randomCrewmate].name +
                                                " to make it back, he stumbles out from the bushes grasping a few cans of soup. " +
                                                currentCrewmate[randomCrewmate].name + " seems to be in bad condition and his extremities have succumbed to frostbite.";
                }
                break;
            case 4:
                if (choice == 1)
                {
                    if (numOfCrew == 1 || numOfCrew == 2)
                    {
                        crewManager.ChangeSupplies(3);
                        crewManager.SetStat(currentCrewmate[randomCrewmate].name, HEALTH_STATE.CRITICAL);
                        changeSummaryArrow(false, true, currentCrewmate[randomCrewmate]);
                        currentCrewmate[0].resultText = "While scavanging through the abandoned town, " +
                                                     currentCrewmate[randomCrewmate].name + " saw some canned food and bottles of water lying in a building. " +
                                                     "When he went to pick it up, he bumped into another scavenger, who promptly stabbed " +
                                                     currentCrewmate[randomCrewmate].name + " in the abdomen. " + currentCrewmate[randomCrewmate].name + " grabbed a metal pipe on the " +
                                                     "floor and swung at the scavengers's head, knocking him unconscious, before hurrying off with the supplies.";
                    }
                    else
                    {
                        crewManager.ChangeSupplies(3);
                        currentCrewmate[0].resultText = "While scavanging through the abandoned town, " +
                                                     currentCrewmate[randomCrewmate].name + " found some canned food and bottles of water lying in a building. " +
                                                     "There were other scavengers lurking around but none dared go near your larger group of crew members.";
                    }
                }
                else if (choice == 2)
                {
                    currentCrewmate[0].resultText = "As the crew left the area, the silhouettes of armed scavengers could be seen throughout the town. " +
                                                    "The sound of distant gunshots could be heard.";
                }
                break;
            case 5:
                if (choice == 1)
                {
                    crewManager.ChangeSupplies(5);
                    crewManager.ChangeMental(currentCrewmate[randomCrewmate].name, -1);
                    changeSummaryArrow(false, false, currentCrewmate[randomCrewmate]);
                    currentCrewmate[0].resultText = currentCrewmate[randomCrewmate].name + " said his prayers before putting his knife to the man's throat, killing him. " +
                                                 currentCrewmate[randomCrewmate].name+" then took the man's supplies and skinned him, cutting him up for meat to be consumed.";
                }
                else if (choice == 2)
                {
                    crewManager.ChangeSupplies(2);
                    currentCrewmate[0].resultText = "Cautiously, " + currentCrewmate[randomCrewmate].name + " rummages through the man's belongings, taking anything of " +
                                                     "value before leaving him to die.";
                }
                break;
            case 6:
                if (choice == 1)
                {
                    if (choiceNum<=7)
                    {
                        crewManager.ChangeSupplies(3);
                        currentCrewmate[0].resultText = currentCrewmate[randomCrewmate].name + " took aim and shot at the deer, killing it in an instant, its confused fawn could only stand and stare as " +
                                                     currentCrewmate[randomCrewmate].name + "  skinned and carved the deer for its meat and fur.";
                    }
                    else
                    {
                        currentCrewmate[0].resultText = "As " + currentCrewmate[randomCrewmate].name + " took aim, the rustling of leaves could be heard. A large wolf leapt out of the bushes," +
                                                        " and viciously attacked the deer. As the wolf devoured its prey, "+ currentCrewmate[randomCrewmate].name+ " carefully retreated, not " +
                                                        "wanting to suffer the same fate as the deer.";
                    }
                }
                else if (choice == 2)
                {
                    if (choiceNum<=2)
                    {
                        crewManager.ChangeMental(currentCrewmate[randomCrewmate].name, -1);
                        changeSummaryArrow(false, false, currentCrewmate[randomCrewmate]);
                        currentCrewmate[0].resultText = "As " + currentCrewmate[randomCrewmate].name + " walked away, a large polar bear jumped out of the surrounding bushes " +
                                                 "killing both the deer and her fawn in an instant.";
                    }
                    else
                    {
                        currentCrewmate[0].resultText = currentCrewmate[randomCrewmate].name + " walks away from the scene.";
                    }
                }
                break;
            case 7:
                if (choice == 1)
                {
                    crewManager.ChangeSupplies(3);
                    currentCrewmate[0].resultText = "The crew carried back the supplies they found in that abandoned camp.";
                }
                break;
            case 8:
                if (choice == 1)
                {
                    crewManager.ChangeSupplies(2);
                    currentCrewmate[0].resultText = currentCrewmate[randomCrewmate].name + " found some usable firewood.";
                }
                break;
            case 9:
                if (choice == 1)
                {
                    if (choiceNum <= 5)
                    {
                        crewManager.ChangeHealth(currentCrewmate[randomCrewmate].name, 1);
                        changeSummaryArrow(true, true, currentCrewmate[randomCrewmate]);
                        currentCrewmate[0].resultText = currentCrewmate[randomCrewmate].name + " boiled a couple mushrooms and ate them. After eating them,he felt " +
                                                        "strangely energised and full of vigor so perhaps they were medicinal mushrooms.";
                    }
                    else
                    {
                        crewManager.ChangeHealth(currentCrewmate[randomCrewmate].name, -1);
                        changeSummaryArrow(true, false, currentCrewmate[randomCrewmate]);
                        currentCrewmate[0].resultText = currentCrewmate[randomCrewmate].name + " boiled a couple mushrooms and ate them. After eating them,he immediately " +
                                                        "felt a sharp pain in his stomach so perhaps they were poisonous.";
                    }
                }
                else if (choice == 2)
                {
                    currentCrewmate[0].resultText = "Deciding that eating random mushrooms wasnt the most sensible thing to do, "+ currentCrewmate[randomCrewmate].name +
                                                    "returned to the train. ";
                }
                break;
            case 10:
                if (choice == 1)
                {
                    crewManager.ChangeSupplies(3);
                    currentCrewmate[0].resultText = currentCrewmate[randomCrewmate].name + " siphoned some fuel from some abandoned vehicles.";
                }
                break;
            case 11:
                if (choice == 1)
                {
                    crewManager.ChangeSupplies(-2);
                    currentCrewmate[0].resultText = currentCrewmate[randomCrewmate].name + " fell into a frozen stream and lost some supplies.";
                }
                break;
            case 12:
                if (choice == 1)
                {
                    crewManager.ChangeHealth(currentCrewmate[randomCrewmate].name, -1);
                    changeSummaryArrow(false, true, currentCrewmate[randomCrewmate]);
                    currentCrewmate[0].resultText = currentCrewmate[randomCrewmate].name + " fell and hit his head which resulted in minor injuries.";
                }
                break;
            case 13:
                if (choice == 1)
                {
                    crewManager.ChangeHealth(currentCrewmate[randomCrewmate].name, 1);
                    changeSummaryArrow(true, true, currentCrewmate[randomCrewmate]);
                    currentCrewmate[0].resultText = currentCrewmate[randomCrewmate].name + " patched himself up with the first aid kit he found.";
                }
                break;
            case 14:
                if (choice == 1)
                {
                    crewManager.ChangeMental(currentCrewmate[randomCrewmate].name, 1);
                    changeSummaryArrow(true, false, currentCrewmate[randomCrewmate]);
                    currentCrewmate[0].resultText = currentCrewmate[randomCrewmate].name+ "feels better after liesurely watching the rabbits.";
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
            string name = currentCrewmate[i].name;
            switch (name)
            {
                case "Harris":
                    harris.isDeployed = false;
                    harris.Disable();
                    crewManager.crewMembers[name] = currentCrewmate[i];
                    break;
                case "Bronson":
                    bronson.isDeployed = false;
                    bronson.Disable();
                    crewManager.crewMembers[name] = currentCrewmate[i];
                    break;
                case "Chuck":
                    chuck.isDeployed = false;
                    chuck.Disable();
                    crewManager.crewMembers[name] = currentCrewmate[i];
                    break;
                case "Danton":
                    danton.isDeployed = false;
                    danton.Disable();
                    crewManager.crewMembers[name] = currentCrewmate[i];
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
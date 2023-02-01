using CopiumEngine;
using System;

public class EventManager: CopiumScript
{
    public GameManager GameManager;

    public GameObject Option_01;
    public GameObject Option_02;
    public GameObject Next_Event;

    public Text Header;
    public Text Body;

    public Text Option_01_Text;
    public Text Option_02_Text;

    public int DaySequence = 1;
    public int EventSequence = 0;
    public int LuckSequence = 0;

    int InternalLuck = 0;

    public Event_Intro Event_Intro;

    bool Event_01 = false;
    bool Event_02 = false;
    bool Event_03 = false;
    bool Event_04 = false;

    bool EngagingCombat = false;

    Button option01_btn;
    Button option02_btn;
    Button next_btn;
    void Start()
	{
        option01_btn = Option_01.GetComponent<Button>();
        option02_btn = Option_02.GetComponent<Button>();
        next_btn = Next_Event.GetComponent<Button>();
    }
	void Update()
    {
        //option01_btn = Option_01.GetComponent<Button>();
        //option02_btn = Option_02.GetComponent<Button>();
        //next_btn = Next_Event.GetComponent<Button>();
        if (next_btn.state == ButtonState.OnClick && Next_Event.activeSelf)
        {
            EventSequence++;

            // Day 02
            if (EventSequence == 5)
                DaySequence++;

            //if (EventSequence == 6)
            //    GameManager.MainScreenCanvas.SetActive(false);
        }

        // This should check if the combat screen has been activated
        if(Input.GetKeyDown(KeyCode.Enter) && EventSequence == 2)
        {
            EngagingCombat = false;
            EventSequence++;
        }
        CheckCurrentEvent();
    }

    void CheckCurrentEvent()
    {
        // Update eventsequence after every button click
        switch (EventSequence)
        {
            case 0:
                //Event_Intro.Event();
                Introduction();
                break;
            case 1:
                Event01();
                break;
            case 2:
                Event02();
                break;
            case 3:
                Event03();
                break;
            case 4:
                Event04();
                break;
            case 5:
                Event05();
                break;
            default:
                break;
        }
    }

    void Introduction()
    {
        if (Option_01.activeSelf && Option_02.activeSelf)
        {
            Option_01.SetActive(false);
            Option_02.SetActive(false);
        }

        Header.text = "AI Assistance Intro";

        Body.text = "Welcome conductor, I am AI your digital assistance for this mission. " +
            "I have a message \nfrom Captain Bob.\n\n\"Good day volunteer, \n\nYou are likely " +
            "here due to the large reward offered by the Council should you complete\n this mission. " +
            "Although tempting, I can't say I'd like to be in your position. The \nrisk is just not " +
            "worth it in my opinion.\n\nYour task is to reach the settlement at the end of this railway. " +
            "Recently we have lost \ncontact with them, and with no way to reestablish communications remotely, " +
            "we are \ngoing to send a team there to investigate. As defending the city is a priority, " +
            "we are \nunable to send the military. \n\nYou might have heard about the 'Crops Curse' that has been happening. " +
            "Thus, we are \nonly able to provide 2 days worth of rations. I'm sure you will figure something out \nwhilst traveling there. " +
            "\n\nI wish you all the best and hope to receive good news.\n\nBest regards, \nCaptain Bob Jones\"\n\nEnd of transmission. ";
    }

    // Luck in a barren wasteland
    void Event01()
    {
        if (Event_01)
        {
            if (GameManager.ReportTab.activeSelf || true)
            {
                Option_01.SetActive(false);
                Option_02.SetActive(false);
                Next_Event.SetActive(true);
            }
            Console.WriteLine("RETURNED");
            return;
        }

        if (GameManager.ReportTab.activeSelf || true)
        {
            Option_01.SetActive(true);
            Option_02.SetActive(true);
            Next_Event.SetActive(false);
        }

        Header.text = "Luck in a barren wasteland";
        Console.WriteLine("TEXT");

        Body.text = "\nReport type: Situation\n\n\nHarris spotted a abandoned town not too far off from the main track. " +
            "Crew seem \nto be in agreement to check it out. Otherwise, nothing out of the ordinary. \nTrain conductor to choose course of action. \n";

        Option_01_Text.text = "Explore abandon town";
        Option_02_Text.text = "Do not explore abandoned town";

        if (option01_btn.state == ButtonState.OnClick)
        {
            Choice01(true);
        }
        else if (option02_btn.state == ButtonState.OnClick)
        {
            Choice01(false);
        }
    }

    void Choice01(bool choice)
    {
        float chance;
        LuckSequence = 1;
        if (choice)
        {
            // 80%
            chance = RNG.Range(0, 1);
            if (chance > 0.2)
            {
                Header.text = "No Luck";

                Body.text = "\nReport type: Situation\n\nThe crew had no luck in finding any useful resources. " +
                    "Though only the first day, the crew \nseem to have let the disappointment get to them. Moral " +
                    "of the crew \nhad decrease significantly since departure.\n";
            }
            // 20%
            else
            {
                Header.text = "Found something useful";

                Body.text = "\nReport type: Situation\n\n\nThrough the thick snow and rubble the crew miraculously " +
                    "found some canned soup \nthat still looks good to eat. This minor victory seem to boost moral " +
                    "of the crew \nslightly, everyone seem to be in a much cheery mood.";
            }
        }
        else if (!choice)
        {
            // 50%
            chance = RNG.Range(0, 1);
            if (chance > 0.5)
            {
                Header.text = "What could have been";

                Body.text = "\nReport type: Situation\n\nThe crew seemed to be quite disappointed in your decision, " +
                    "all had hoped that they might be \nable to find something of use in the town. Moral of the crew " +
                    "seem to be hit\n slightly, but all still seemed to have positive outlooks on this mission.";
            }
            // 50%
            else
            {
                Header.text = "Dissatisfaction brewing";

                Body.text = "\nReport type: Situation\n\n\nChuck seem to be quite unhappy with your decision, " +
                    "and had a argument with Harris\n who tried to defend your course of action. Chuck began throwing " +
                    "punches at Harris\n but Luckily the confrontation was broken up by Danton before it could escalate further.";
                LuckSequence = 0;
            }
        }

        Event_01 = true;
    }

    // Leaving the city
    void Event02()
    {
        if (Event_02)
        {
            if (GameManager.ReportTab.activeSelf || true)
            {
                Option_01.SetActive(false);
                Option_02.SetActive(false);
                if(EngagingCombat)
                    Next_Event.SetActive(false);
                else
                    Next_Event.SetActive(true);
            }
            return;
        }

        if (GameManager.ReportTab.activeSelf || true)
        {
            Option_01.SetActive(true);
            Option_02.SetActive(true);
            Next_Event.SetActive(false);
        }

        Header.text = "Leaving the city";
        if (LuckSequence == 1)
        {
            Body.text = "Report type: Situation\n\nDanton spotted a mysterious vehicle approaching our train, " +
                "attempts at communications \nfailed. People on the vehicle boarded the train and began rummaging " +
                "through the train\n\nTrain commander to decide on course of action.";
        }
        else if (LuckSequence == 0)
        {
            Body.text = "\nReport type: Situation\n\nHarris spotted a mysterious vehicle approaching our train, " +
                "attempts at communications \nfailed. People on the vehicle boarded the train and began rummaging " +
                "through the train\nChuck seemed to be extremely eager to chase of the hostiles.\n\nTrain commander" +
                " to decide on course of action.";
        }

        Option_01_Text.text = "Fight and capture the hostiles";
        Option_02_Text.text = "Hide and wait for them to pass";

        if (option01_btn.state == ButtonState.OnClick && Option_01.activeSelf)
        {
            Choice02(true);
        }
        else if (option02_btn.state == ButtonState.OnClick && Option_02.activeSelf)
        {
            Choice02(false);
        }

    }

    void Choice02(bool choice)
    {
        if (choice)
        {
            Header.text = "Engaging";

            Body.text = "\nReport type: Situation\n\n\nWhen confronted hostiles began firing at crew " +
                "members.\nCrew members are currently engaging in fight with hostiles.\n\nStatus of " +
                "the battle can be seen from the scene at the side.";

            if(LuckSequence == 1)
                InternalLuck = 1;
            else
                InternalLuck = 0;

            LuckSequence = 1;

            EngagingCombat = true;
        }
        else if (!choice)
        {
            Header.text = "Hiding from danger";
            if (LuckSequence == 1)
            {
                Body.text = "\nReport type: Situation\n\nThe crew  followed orders and quickly hid and " +
                    "allowed the hostiles to loot the food \nfrom our supplies. \n\nFood supplies left: 2";

                LuckSequence = 11;
            }
            else if (LuckSequence == 0)
            {
                Body.text = "\nReport type: Situation\n\n\nThe crew except Chuck followed orders and quickly " +
                    "hid and allowed the hostiles to \nloot the food from our supplies. Bronson and Harris " +
                    "had to tie up and drag Chuck so \nthat he would not try and engage. \n\nAfter the hostiles " +
                    "left Chuck seem to be furious and in a fit of rage punched Danton in \nthe face, breaking" +
                    " his jaw.\n\nFood supplies left: 2";

                LuckSequence = 10;
            }

        }

        Event_02 = true;
    }

    void Event03()
    {
        if (LuckSequence == 10 || LuckSequence == 11)
        {
            if(Event_03)
            {
                return;
            }
            else
            {
                Event_03 = true;
                EventSequence++;
            }
        }
        else if (LuckSequence == 1)
        {
            float chance = RNG.Range(0, 1);
            if (chance > 0.5)
                LuckSequence = 2;
            else
                LuckSequence = 3;
        }
        else if (LuckSequence == 2)
            Event03_01();
        else if (LuckSequence == 3)
            Event03_02();
    }

    // A Devastating Defeat
    void Event03_01()
    {
        if (Event_03)
        {
            if (GameManager.ReportTab.activeSelf || true)
            {
                Option_01.SetActive(false);
                Option_02.SetActive(false);
                Next_Event.SetActive(true);
            }
            return;
        }

        if (GameManager.ReportTab.activeSelf || true)
        {
            Option_01.SetActive(false);
            Option_02.SetActive(false);
            Next_Event.SetActive(true);
        }

        Header.text = "A Devastating Defeat";
        if (InternalLuck == 1)
        {
            Body.text = "\nReport type: Situation\n\n\nThe crew was quickly neutralized by the hostiles, " +
                "luckily they seem to be only\n here because they where desperate for food. No crew was badly " +
                "hurt/injured, after looting \nthe food from our train, they quickly left and returned our firearms. " +
                "\n\nFood supplies left: 2";
        }
        else if (InternalLuck == 0)
        {
            Body.text = "\nReport type: Situation\n\n\nThe crew was quickly neutralized by the hostiles, " +
                "luckily they seem to be only\n here because they where desperate for food. No crew was badly " +
                "hurt/injured, after looting \nthe food from our train, they quickly left and returned our arms. " +
                "\n\nFood supplies left: 2";
        }

        if (InternalLuck == 1)
            LuckSequence = 11;
        else if (InternalLuck == 0)
            LuckSequence = 10;

        Event_03 = true;
    }

    // Victory But...
    void Event03_02()
    {
        if (Event_03)
        {
            if (GameManager.ReportTab.activeSelf || true)
            {
                Option_01.SetActive(false);
                Option_02.SetActive(false);
                Next_Event.SetActive(true);
            }
            return;
        }

        if (GameManager.ReportTab.activeSelf || true)
        {
            Option_01.SetActive(true);
            Option_02.SetActive(true);
            Next_Event.SetActive(false);
        }

        Header.text = "Victory, But...";
        Body.text = "\nReport type: Situation\n\nLuckily no crew members was badly injured in the fight. " +
            "However, there were 2 hostiles\n who survived. When the fight was not going in their favor, they" +
            " lay down their \narms and offered surrender in exchange for their lives. The crew is split in" +
            " deciding \nto kill or let them go. \nTrain conductor to choose course of action.";

        Option_01_Text.text = "Kill the survivors";
        Option_02_Text.text = "Let them go";

        if (option01_btn.state == ButtonState.OnClick && Option_01.activeSelf)
        {
            Choice03_02(true);
        }
        else if (option02_btn.state == ButtonState.OnClick && Option_02.activeSelf)
        {
            Choice03_02(false);
        }
    }

    void Choice03_02(bool choice)
    {
        LuckSequence = 0;
        if (choice)
        {
            Header.text = "No Mercy";

            Body.text = "\nReport type: Situation\n\nHarris and Bronson had opposed to the idea but before " +
                "they could do anything, Chuck \nhad already put a bullet into each of the survivors.\nThe crew " +
                "carried 2 remaining survivors threw their bodies out the train.\n\nBronson seem quite disturbed " +
                "with the killing of the 2 survivors.\n\nBronson Mental: Shaken";
        }
        else if (!choice)
        {
            Header.text = "Mercy";

            Body.text = "\nReport type: Situation\n\nChuck seemed quick aggravated by your decision.\nChuck had " +
                "ordered the 2 survivors to strip off all their clothing and hand \nover their weapons before he " +
                "kicked them off the moving train.\n\nBronson seem to be quite disturbed by what Chuck had done." +
                "\n\nBronson Mental: Shaken";
        }

        if (GameManager.ReportTab.activeSelf || true)
        {
            Option_01.SetActive(false);
            Option_02.SetActive(false);
            Next_Event.SetActive(true);
        }

        Event_03 = true;
    }

    // First Night
    void Event04()
    {
        if (LuckSequence == 0)
            Event04_01();
        else if (LuckSequence == 10 || LuckSequence == 11)
            Event04_02();
    }

    void Event04_01()
    {
        if (Event_04)
        {
            if (GameManager.ReportTab.activeSelf || true)
            {
                Option_01.SetActive(false);
                Option_02.SetActive(false);
                Next_Event.SetActive(true);
            }
            return;
        }

        if (GameManager.ReportTab.activeSelf || true)
        {
            Option_01.SetActive(true);
            Option_02.SetActive(true);
            Next_Event.SetActive(false);
        }

        Header.text = "First Night";
        if (InternalLuck == 1)
        {
            Body.text = "\nReport type: Daily\n\nThe crew seem tried and fatigued as the day comes to an end, " +
                "except Chuck \nwho seem awfully cheerful after what had happened in the day.\n\n[Insert Food, " +
                "Health and Mental state]\n\nDo you wish to ration out your supplies?\n*Note each crew requires 0.5 food.";
        }
        else if (InternalLuck == 0)
        {
            Body.text = "\nReport type: Daily\n\nDaily evening report\n\nThe crew seem tried and fatigued as " +
                "the day comes to an end.\n\n[Insert Food, Health and Mental state]\n\nDo you wish to ration " +
                "out your supplies?\n*Note each crew requires 0.5 food.";
        }

        Option_01_Text.text = "Yes";
        Option_02_Text.text = "No";

        if (option01_btn.state == ButtonState.OnClick && Option_01.activeSelf)
        {
            Choice04_01(true);
        }
        else if (option02_btn.state == ButtonState.OnClick && Option_02.activeSelf)
        {
            Choice04_01(false);
        }
    }

    void Choice04_01(bool choice)
    {
        if (choice)
        {
            Header.text = "Delegate Food";

            Body.text = "\nReport type: End Of Day\n\n\n Food was distributed...";
        }
        else if (!choice)
        {
            Header.text = "Starving";

            Body.text = "\nReport type: End Of Day\n\n\nEveryone starved...";
        }

        if (GameManager.ReportTab.activeSelf || true)
        {
            Option_01.SetActive(false);
            Option_02.SetActive(false);
            Next_Event.SetActive(true);
        }

        Event_04 = true;
    }

    void Event04_02()
    {
        if (Event_04)
        {
            if (GameManager.ReportTab.activeSelf || true)
            {
                Option_01.SetActive(false);
                Option_02.SetActive(false);
                Next_Event.SetActive(true);
            }
            return;
        }

        if (GameManager.ReportTab.activeSelf || true)
        {
            Option_01.SetActive(true);
            Option_02.SetActive(true);
            Next_Event.SetActive(false);
        }

        Header.text = "First Night";
        if (LuckSequence == 11)
        {
            Body.text = "\nReport type: Daily\n\nThe crew seem to be demoralized by the events that " +
                "transpired the day and all \nseemed to be heading to bed early.\n\n[Insert Food, Health " +
                "and Mental state]\n\nDo you wish to ration out your supplies?\n*Note each crew requires 0.5 food.";
        }
        else if (LuckSequence == 10)
        {
            Body.text = "\nReport type: Situation\n\nDaily evening report\n\nThe crew seem to be " +
                "demoralized by the events that transpired the day and all\n seemed to be heading to bed early." +
                "\n\n[Insert Food, Health and Mental state]\nSupplies: 2/4\n\n\n\nDo you wish to " +
                "ration out your supplies?\n*Note each crew requires 0.5 food.";
        }

        Option_01_Text.text = "Yes";
        Option_02_Text.text = "No";

        if (option01_btn.state == ButtonState.OnClick && Option_01.activeSelf)
        {
            Choice04_02(true);
        }
        else if (option02_btn.state == ButtonState.OnClick && Option_02.activeSelf)
        {
            Choice04_02(false);
        }
    }

    void Choice04_02(bool choice)
    {
        if (choice)
        {
            Header.text = "Delegate Food";

            Body.text = "\nReport type: End Of Day\n\n\n Food was distributed...";
        }
        else if (!choice)
        {
            Header.text = "Starving";

            Body.text = "\nReport type: End Of Day\n\n\nEveryone starved...";
        }

        if (GameManager.ReportTab.activeSelf || true)
        {
            Option_01.SetActive(false);
            Option_02.SetActive(false);
            Next_Event.SetActive(true);
        }

        Event_04 = true;
    }



    // New Day
    void Event05()
    {
        if (Option_01.activeSelf && Option_02.activeSelf)
        {
            Option_01.SetActive(false);
            Option_02.SetActive(false);
        }

        Header.text = "Day 02";

        Body.text = "To Be Continued...";
    }
    
}

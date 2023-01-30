using CopiumEngine;
using System;

public class GameManager: CopiumScript
{
	public GameObject TrainCanvas;
	public GameObject MainScreenCanvas;
	public GameObject CombatCanvas;
    public GameObject Option_01;
    public GameObject Option_02;
    public GameObject Next_Event;

    public Button ReportScreenBtn;
    public Button CombatScreenBtn;
    public Button OtherScreenBtn;

    public AudioSource audio;

	public Text Header;
	public Text Body;

	public Text Option_01_Text;
	public Text Option_02_Text;

	public int EventSequence = 0;
    public int LuckSequence = 0;

    bool isReportScreenOn = false;
    bool Event_01 = false;
    bool Event_02 = false;

    Button option01_btn;
    Button option02_btn;
    Button next_btn;

	void Start()
	{
        isReportScreenOn = false;
        option01_btn = Option_01.GetComponent<Button>();
        option02_btn = Option_02.GetComponent<Button>();
        next_btn = Next_Event.GetComponent<Button>();

        audio = GetComponent<AudioSource>();

        UpdateCanvases();
    }
	void Update()
	{
        if(next_btn.state == ButtonState.OnClick && Next_Event.activeSelf)
        {
            EventSequence++;
        }

        if (ReportScreenBtn.state == ButtonState.OnClick)
        {
            isReportScreenOn = true;
            UpdateCanvases();
        }

        if (Input.GetKeyDown(KeyCode.Escape))
        {
            if (isReportScreenOn)
            {
                isReportScreenOn = false;
                UpdateCanvases();
            }
        }

        CheckCurrentEvent();
    }

    void UpdateCanvases()
    {
        if(isReportScreenOn)
        {
            if(!MainScreenCanvas.activeSelf)
                MainScreenCanvas.SetActive(true);

            if(CombatCanvas.activeSelf)
                CombatCanvas.SetActive(false);
        }
        else
        {
            if (!TrainCanvas.activeSelf)
                TrainCanvas.SetActive(true);

            if (MainScreenCanvas.activeSelf)
                MainScreenCanvas.SetActive(false);

            if (CombatCanvas.activeSelf)
                CombatCanvas.SetActive(false);
        }
    }

	void CheckCurrentEvent()
	{
        // Update eventsequence after every button click
        switch (EventSequence)
        {
            case 0:
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

        Header.text = "AI assistance Intro";

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
        if(Event_01)
        {
            if (isReportScreenOn)
            {
                Option_01.SetActive(false);
                Option_02.SetActive(false);
                Next_Event.SetActive(true);
            }
            return;
        }

        if (isReportScreenOn)
        {
            Option_01.SetActive(true);
            Option_02.SetActive(true);
            Next_Event.SetActive(false);
        }
        
        Header.text = "Luck in a barren wasteland";

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

	void Choice01(bool choice01)
	{
        float chance;
        LuckSequence = 1;
        if (choice01)
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
        else if(!choice01)
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
            if (isReportScreenOn)
            {
                Option_01.SetActive(false);
                Option_02.SetActive(false);
                Next_Event.SetActive(true);
            }
            return;
        }

        if (isReportScreenOn)
        {
            Option_01.SetActive(true);
            Option_02.SetActive(true);
            Next_Event.SetActive(false);
        }

        Header.text = "Leaving the city";
        if(LuckSequence == 1)
        {
            Body.text = "Report type: Situation\n\nBronson spotted a mysterious vehicle approaching our train, " +
                "attempts at communications \nfailed. People on the vehicle boarded the train and began rummaging " +
                "through the train\n\nTrain commander to decide on course of action.";
        }
        else if(LuckSequence == 0)
        {
            Body.text = "\nReport type: Situation\n\nBronson spotted a mysterious vehicle approaching our train, " +
                "attempts at communications \nfailed. People on the vehicle boarded the train and began rummaging " +
                "through the train\nChuck seemed to be extremely eager to chase of the hostiles.\n\nTrain commander" +
                " to decide on course of action.";
        }

        Option_01_Text.text = "Fight and capture the hostiles";
        Option_02_Text.text = "Hide and wait for them to pass";

        if (option01_btn.state == ButtonState.OnClick && Option_01.activeSelf)
        {
            Console.WriteLine("Run");
            Choice02(true);
        }
        else if (option02_btn.state == ButtonState.OnClick && Option_02.activeSelf)
        {
            Console.WriteLine("Run2");
            Choice02(false);
        }

    }

    void Choice02(bool choice01)
    {

        if (choice01)
        {
            Header.text = "Engaging";

            Body.text = "\nReport type: Situation\n\n\nWhen confronted hostiles began firing at crew " +
                "members.\nCrew members are currently engaging in fight with hostiles.\n\nStatus of " +
                "the battle can be seen from the scene at the side.";
        }
        else if (!choice01)
        {
            Header.text = "Hiding from danger";
            if (LuckSequence == 1)
            {
                Body.text = "\nReport type: Situation\n\nThe crew  followed orders and quickly hid and " +
                    "allowed the hostiles to loot the food \nfrom our supplies. \n\nFood supplies left: 2";
            }
            else if(LuckSequence == 0)
            {
                Body.text = "\nReport type: Situation\n\n\nThe crew except Chuck followed orders and quickly " +
                    "hid and allowed the hostiles to \nloot the food from our supplies. Bronson and Harris " +
                    "had to tie up and drag Chuck so \nthat he would not try and engage. \n\nAfter the hostiles " +
                    "left Chuck seem to be furious and in a fit of rage punched Danton in \nthe face, breaking" +
                    " his jaw.\n\nFood supplies left: 2";
            }
        
        }

        if (isReportScreenOn)
        {
            Option_01.SetActive(false);
            Option_02.SetActive(false);
            Next_Event.SetActive(true);
        }

        Event_02 = true;
    }

    void Event03()
	{

	}
}

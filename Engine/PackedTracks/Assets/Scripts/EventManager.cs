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
        option01_btn = Option_01.GetComponent<Button>();
        option02_btn = Option_02.GetComponent<Button>();
        next_btn = Next_Event.GetComponent<Button>();
    }
	void Update()
	{
        if (next_btn.state == ButtonState.OnClick && Next_Event.activeSelf)
        {
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
                
                break;
            case 1:
                
                break;
            case 2:
                
                break;
            case 3:
                
                break;
            default:
                break;
        }
    }
}

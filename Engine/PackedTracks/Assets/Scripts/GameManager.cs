/*!***************************************************************************************
\file			GameManager.cs
\project
\author			Zacharie Hong
\co-author		Sean Ngo
                Shawn Tanary
                Matthew Lau

\par			Course: GAM200
\par			Section:
\date			26/11/2022

\brief
	GameManager manages the game logic.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
using CopiumEngine;
using System;
using System.Collections.Generic;
using System.Security.Cryptography;

public class GameManager: CopiumScript
{
    public static GameManager Instance = null;

	public GameObject TrainCanvas;
    public GameObject PauseCanvas;

    public GameObject ManualPopUp;
    public GameObject MainPage;
    public GameObject Page2;
    public GameObject Page3;
    public GameObject Page4;
    public GameObject prevButtonObject;
    public GameObject cityBuilding;
    public HowtoPlayMenu htpmScript;

    public Button ManualBtn;
    public Button ManualPopUpBtn;

    public Button BackstoryBtn;
    public GameObject BackstoryPopUp;

    public TrainManager trainManager;
    public ReportScreenManager reportScreenManager;
    public CrewStatusManager crewStatusManager;
    public ResultManager resultManager;
    public PauseMenu pauseMenu;

    public float distanceLeft = 400.0f;

    public float distanceInterval = 1.0f;
    float foodTimer = 0.0f;
    float timer = 0.0f;
    float distancePerEvent = 100.0f;

    bool updateEvent = false;
    bool updateHunger = false;
    public bool gameEnd = false;

    bool moving = false;

    public int eventSequence = 0;

    void Awake()
    {
        Instance = this;
    }
    void Start()
	{
        eventSequence = 0;
        //UpdateCanvases();
    }

	void Update()
    {
        ButtonInputs();
        if (pauseMenu.isPaused)
            return;

        if (Input.GetKeyDown(KeyCode.D1))
            CrewMenu.Instance.SetStat("Harris", HEALTH_STATE.DEAD);

        if (Input.GetKeyDown(KeyCode.D2))
            CrewMenu.Instance.SetStat("Bronson", HEALTH_STATE.DEAD);

        if (Input.GetKeyDown(KeyCode.D3))
            CrewMenu.Instance.SetStat("Chuck",HEALTH_STATE.DEAD);

        if (Input.GetKeyDown(KeyCode.D4))
            CrewMenu.Instance.SetStat("Danton", HEALTH_STATE.DEAD);

        if (Input.GetKeyDown(KeyCode.S))
            SceneManager.LoadScene("Ending");

        if (Input.GetKeyDown(KeyCode.F))
        {
            distanceLeft = 50.0f;
        }

        // Toggle canvases
        //CanvasManager();

        // Game ends
        if (CheckForGameEndCondition())
            return;

        if (CrewMenu.Instance.CheckAllCrewDead() && eventSequence >= 0)
            eventSequence = -3;
        else if (distanceLeft < 0.99f && eventSequence >= 0)
        {
            int numCrewAlive = 0;
            foreach (Person person in CrewMenu.Instance.crewMembers.Values)
            {
                if (person.alive)
                    numCrewAlive++;
            }

            eventSequence = (numCrewAlive == 2 || numCrewAlive == 3) ? -6 : -4;
        }


        // Cant deploy if the train is moving
        if ((moving && trainManager.currentSpeed <= 0f) || (!moving && trainManager.currentSpeed > 0f))
        {
            ToggleMoving();
        }

        if(distanceLeft < 50.0f)
        {
            cityBuilding.SetActive(true);

            float size = (1.0f - distanceLeft / 50.0f) * 4.0f;
            Vector3 scale = new Vector3(size, size, 1.0f);
            cityBuilding.transform.localScale = scale;

            Vector3 pos = cityBuilding.transform.position;
            pos.y = 1.75f + (1.0f - distanceLeft / 50.0f) * 1.0f;
            cityBuilding.transform.position = pos;
        }

        // When train is moving and can still travel
        MoveTrain();

        // Reducing supplies for crew to consume
        SupplyCounter();

        if(trainManager.accelerate)
            CrewMenu.Instance.UpdateAllStats();
    }

	/**************************************************************************/
	/*!
	    \brief
		    Check for game end conditions and close neccessary panels

        \return
            gameEnd status
	*/
	/**************************************************************************/
    bool CheckForGameEndCondition()
    {
        if(!gameEnd && eventSequence < 0)
        {
            trainManager.FlickLever(false);
            crewStatusManager.ReturnToCockpit(false);
            reportScreenManager.OpenPanel();

            if (EventsManager.Instance.Done())
            {
                gameEnd = true;
                EventsManager.Instance.UpdateCurrentEvent();
            }
        }

        return gameEnd;
    }

	/**************************************************************************/
	/*!
	    \brief
		    Updates all the canvases
	*/
	/**************************************************************************/
    void CanvasManager()
    {
        reportScreenManager.UpdateCanvas();
        crewStatusManager.UpdateCanvas();
        resultManager.UpdateCanvas();
    }

	/**************************************************************************/
	/*!
	    \brief
		    Toggle the moving status of the train
	*/
	/**************************************************************************/
    public void ToggleMoving()
    {
        moving = !moving;
        CrewMenu.Instance.SetClickable(!moving);
    }

	/**************************************************************************/
	/*!
	    \brief
		    Move the train
	*/
	/**************************************************************************/
    void MoveTrain()
    {
        if (trainManager.currentSpeed > 0.1f && distanceLeft > 0)
        {
            while (timer >= distanceInterval) // Every few distance interval
            {
                distanceLeft -= trainManager.currentSpeed / 3.0f; // Reduce the distance left

                // Only update event if distance per event is activated
                if (distanceLeft > 1.0f && distanceLeft % distancePerEvent < 1.0f && !updateEvent)
                {
                    DisableInteractions();
                    // Show notifications (Visual & Audio)
                    reportScreenManager.alert.enabled = true;
                    crewStatusManager.alert.enabled = true;
                    eventSequence++;
                    updateEvent = true; // Trigger only once
                    EventsManager.Instance.UpdateCurrentEvent();
                }
                // Right now if the distance left to the next event is reseted, we can update event again
                else if (distanceLeft % distancePerEvent > 1.0f)
                    updateEvent = false;

                //ZACH: Handle this with your choice timer
                // Close to the next event and has yet to select a choice, select default choice
                //if (distanceLeft % distancePerEvent < 5.0f && EventManager.EventSequence > 0 && !updateEvent)
                    //EventManager.SelectDefaultChoice();

                // Reduce hunger every few km
                float remainder = distanceLeft % (distancePerEvent / 2.1f);
                if (remainder < 1.0f && !updateHunger)
                {
                    updateHunger = true;
                    CrewMenu.Instance.UpdateHunger();
                }
                else if (remainder > 1.0f)
                    updateHunger = false;

                timer -= distanceInterval;
            }

            timer += Time.deltaTime;
        }
    }

	/**************************************************************************/
	/*!
	    \brief
		    Updates the supple counter
	*/
	/**************************************************************************/
    void SupplyCounter()
    {
        if (trainManager.accelerate && distanceLeft > 1.0f)
        {
            if (foodTimer >= 5.0f && CrewMenu.Instance.supplies != 0)
            {
                Console.WriteLine("decrement supplies");
                foreach (Person person in CrewMenu.Instance.crewMembers.Values)
                {
                    if (!person.alive)
                        continue;
                    if (person.hunger < HUNGER_STATE.FULL)
                    {
                        person.hunger += 1;
                        CrewMenu.Instance.ChangeSupplies(-1);
                    }
                }
                foodTimer = 0.0f;
            }
            foodTimer += Time.deltaTime;
        }
    }

	/**************************************************************************/
	/*!
	    \brief
		    Checks for button inputs and updates depending on the button inputs
	*/
	/**************************************************************************/
    void ButtonInputs()
    {
        if (ManualBtn.state == ButtonState.OnRelease)
        {
            //audioManager.paperSFX.Play();
            ManualPopUp.SetActive(true);   
            //audioManager.fileOpenSFX.Play();

        }
        if (ManualPopUpBtn.state == ButtonState.OnRelease && ManualPopUp.activeSelf)
        {
            CloseManual();
        }

        if (BackstoryBtn.state == ButtonState.OnRelease)
        {
            BackstoryPopUp.SetActive(true);
        }
    }

	/**************************************************************************/
	/*!
	    \brief
		    Close the how to play manual. 
	*/
	/**************************************************************************/
    public void CloseManual()
    {
        //audioManager.fileCloseSFX.Play();

        ManualPopUp.SetActive(false);
        MainPage.SetActive(true);
        Page2.SetActive(false);
        Page3.SetActive(false);
        Page4.SetActive(false);
        prevButtonObject.SetActive(false);

        htpmScript.page = 1;
    }

    public void DisableInteractions()
    {
        trainManager.DisableInteractions();
        crewStatusManager.DisableInteractions();
        resultManager.DisableInteractions();
        reportScreenManager.DisableInteractions();
    }

    public void EnableInteractions()
    {
        trainManager.EnableInteractions();
        crewStatusManager.EnableInteractions();
        resultManager.EnableInteractions();
        reportScreenManager.EnableInteractions();
    }
}

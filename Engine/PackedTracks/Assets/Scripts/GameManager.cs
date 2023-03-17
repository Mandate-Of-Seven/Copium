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

public class GameManager: CopiumScript
{
    public EventManager EventManager;
    public AudioManager audioManager;

	public GameObject TrainCanvas;
    public GameObject PauseCanvas;

    public GameObject ManualPopUp;
    public GameObject MainPage;
    public GameObject Page2;
    public GameObject Page3;
    public GameObject Page4;
    public GameObject prevButtonObject;
    public HowtoPlayMenu htpmScript;

    public Button ManualBtn;
    public Button ManualPopUpBtn;

    public CrewMenu crewMenuScript;
    public TrainManager trainManager;
    public ReportScreenManager reportScreenManager;
    public CrewStatusManager crewStatusManager;
    public ResultManager resultManager;
    public PauseMenu pauseMenu;

    public float distanceLeft = 200;

    public float distanceInterval = 1.0f;
    float foodTimer = 0.0f;
    float hungerTimer = 0.0f;
    float timer = 0.0f;
    float distancePerEvent = 10.0f;

    bool updateEvent = false;
    bool updateHunger = false;
    public bool gameEnd = false;

    bool moving = false;

    public Text testText;
    public StringTypeWriterEffect effect;

    void Start()
	{
        effect = new StringTypeWriterEffect("", "Fire roared through the bifurcated city of Ankh-Morpork. Where it\n licked the Wizards’ Quarter it burned blue and green and was even\n laced with strange sparks of the eighth color, octarine; where its\n outriders found their way into the vats and oil stores all along\n Merchant Street it progressed in a series of blazing fountains and explosions; in the streets of the perfume blenders it burned with a\n sweetness; where it touched bundles of rare and dry herbs in the\n storerooms of the drugmasters it made men go mad and talk to God.", 0.05f);
        //UpdateCanvases();
    }

	/**************************************************************************/
	/*!
	    \brief
		    Open the report screen
	*/
	/**************************************************************************/
    void OpenReportScreen()
    {
        
        audioManager.clickSFX.Play();
    }

	void Update()
    {
        testText.text = effect.Write();
        ButtonInputs();
        if (pauseMenu.isPaused)
            return;

        if (Input.GetKeyDown(KeyCode.C))
            crewMenuScript.SetCrew(CrewMenu.STAT_TYPES.ALIVE, 2, 0);

        if (Input.GetKeyDown(KeyCode.B))
            crewMenuScript.SetCrew(CrewMenu.STAT_TYPES.ALIVE, 1, 0);

        if (Input.GetKeyDown(KeyCode.D))
            crewMenuScript.SetCrew(CrewMenu.STAT_TYPES.ALIVE, 3, 0);

        // Toggle canvases
        //CanvasManager();

        // Game ends
        if (CheckForGameEndCondition())
            return;

        // Cant deploy if the train is moving
        if ((moving && trainManager.currentSpeed <= 0f) || (!moving && trainManager.currentSpeed > 0f))
        {
            ToggleMoving();
        }

        // When train is moving and can still travel
        MoveTrain();

        // Reducing supplies for crew to consume
        SupplyCounter();

        if(trainManager.accelerate)
            crewMenuScript.UpdateAllStats();
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
        if(!gameEnd && EventManager.EventSequence < 0)
        {
            gameEnd = true;
            trainManager.FlickLever(false);
            crewStatusManager.ClosePanel(false);
            reportScreenManager.OpenPanel();
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
        crewMenuScript.SetClickable(!moving);
    }

	/**************************************************************************/
	/*!
	    \brief
		    Move the train
	*/
	/**************************************************************************/
    void MoveTrain()
    {
        if (trainManager.accelerate && distanceLeft > 0)
        {
            if (timer >= distanceInterval) // Every few distance interval
            {
                distanceLeft -= trainManager.currentSpeed / 3.0f; // Reduce the distance left

                // Only update event if distance per event is activated
                if (distanceLeft % distancePerEvent < 1.0f && !updateEvent)
                {
                    // Show notifications (Visual & Audio)
                    reportScreenManager.alert.enabled = true;
                    crewStatusManager.alert.enabled = true;

                    updateEvent = true; // Trigger only once
                    EventManager.UpdateEventSequence();
                }
                // Right now if the distance left to the next event is reseted, we can update event again
                else if (distanceLeft % distancePerEvent > 1.0f)
                    updateEvent = false;

                // Close to the next event and has yet to select a choice, select default choice
                if (distanceLeft % distancePerEvent < 5.0f && EventManager.EventSequence > 0 && !updateEvent)
                    EventManager.SelectDefaultChoice();

                // Reduce hunger every few km
                float remainder = distanceLeft % (distancePerEvent / 2.0f);
                if (remainder < 1.0f && !updateHunger)
                {
                    updateHunger = true;
                    crewMenuScript.UpdateHunger();
                }
                else if (remainder > 1.0f)
                    updateHunger = false;

                timer = 0.0f;
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
        if (trainManager.accelerate && distanceLeft < 200.0f)
        {
            if (foodTimer >= 5.0f && crewMenuScript.supplies != 0)
            {
                crewMenuScript.ChangeSupplies(-1);
                crewMenuScript.ChangeAllCrew(CrewMenu.STAT_TYPES.HUNGER, 1);

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
            audioManager.paperSFX.Play();
            ManualPopUp.SetActive(true);   
            audioManager.fileOpenSFX.Play();

        }
        if (ManualPopUpBtn.state == ButtonState.OnRelease && ManualPopUp.activeSelf)
        {
            CloseManual();
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
        audioManager.fileCloseSFX.Play();

        ManualPopUp.SetActive(false);
        MainPage.SetActive(true);
        Page2.SetActive(false);
        Page3.SetActive(false);
        Page4.SetActive(false);
        prevButtonObject.SetActive(false);

        htpmScript.page = 1;
    }
}

/*!***************************************************************************************
\file			Event_Ending.cs
\project
\author			Sean Ngo
\co-author		Shawn Tanary

\par			Course: GAM250
\par			Section:
\date			10/03/2023

\brief
	Contains functions needed for the endings of the game

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
using CopiumEngine;
using System;

public class Event_Ending: CopiumScript
{
    public EventManager EventManager;
    CrewMenu cm;

    //int resolutionTextNum = 0;

    void Start()
    {
        cm = EventManager.crewMenu;
    }
    void Update()
    {

    }

    /*******************************************************************************
    /*!
        \brief
            Calculates what ending the player should get
    */
    /*******************************************************************************/
    public void Ending(int requirement)
    {
        if (requirement == 1)// Some alive or all alive
        {
            int numCrewAlive = 0;
            for (int i = 0; i < cm.crew.Length; i++)
            {
                if (cm.crew[i].alive)
                    numCrewAlive++;
            }

            if (numCrewAlive == 1)
            {
                int choice = 3;
                for (int i = 0; i < cm.crew.Length; i++)
                {
                    if (cm.crew[i].alive)
                    {
                        choice += i;
                        break;
                    }

                }
                Result(choice);
            }
            else if (numCrewAlive == 2 || numCrewAlive == 3)
                Result(2);
            else if (numCrewAlive == 4)
                Result(7);


            //resolutionTextNum = 2;
        }
        else if (requirement == 2) // All dead
        {
            //resolutionTextNum = 1;
            Result(1);
        }
        else if(requirement == 3) // Mid game ending
        {
            // Nothing for now
        }
    }

    /*******************************************************************************
    /*!
        \brief
            Displays the result of the ending as well as plays the corrisponding
            audio
    */
    /*******************************************************************************/
    public void Result(int choice)
    {
        if(choice == 1) // All dead
        {
            // Screen fade and audio narration
            cm.audioManager.endingAllDiedVO.Play();
            cm.fader.shouldFade = true;
            EventManager.Body.text = "All crew members are dead...";
        }
        else if(choice == 2) // 2 or 3 alive
        {
            // Audio narration
            cm.audioManager.endingGenericVO.Play();
            EventManager.Body.text = "The city seems very tiny to me.\nFor what reason are they firing onto us?!\n" +
                "They must be making an extreme effort to halt the train.\n\nOH NO!";
        }
        else if (choice == 3) // Harris alive
        {
            // Audio narration
            cm.audioManager.endingHarrisVO.Play();
            EventManager.Body.text = "Harris, while being disturbed by the event, seems positive and eager to go with the task\n" +
                "I'll provide an update once we've reached the city.";
        }
        else if (choice == 4) // Bronson alive
        {
            // Audio narration
            cm.audioManager.endingBronsonVO.Play();
            EventManager.Body.text = "While it seems that the mission was successful, Bronson is having a hard time adjusting " +
                "to the loss of the other members of the crew. \n\nBronson: \"ITS ALL UR FAULT!\"\n\nh....h...e....l......p...";
        }
        else if (choice == 5) // Chuck alive
        {
            // Audio narration
            cm.audioManager.endingChuckVO.Play();
            EventManager.Body.text = "It seems that the operation was successful. The loss of the crew members doesn't appear to" +
                " be affecting Chuck too much.\n\n\nChuck: \"Sorry cap, but the reward money is " + "all mine\"";
        }
        else if (choice == 6) // Danton alive
        {
            // Audio narration
            cm.audioManager.endingDantonVO.Play();
            EventManager.Body.text = "It seems that the operation was successful. Is Danton doing well? I hope he is coping " +
                "with the loss of the crew.";
        }
        else if (choice == 7) // All alive
        {
            // Audio narration
            EventManager.Body.text = "Its strange, the city... it seems so small. The guards they are looking at us funny. " +
                "Did we come to the wrong place?";
        }
    }
}

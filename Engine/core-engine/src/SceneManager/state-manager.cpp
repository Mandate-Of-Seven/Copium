/*!***************************************************************************************
****
\file			state-manager.cpp
\project		
\author			Matthew Lau
\co-authors		

\par			Course: GAM250
\par			Section: 
\date			12/07/2022

\brief
	Contains:
		1. Declarations of functions and extern variables that allow for the management of Engine States
		2. Enums for all the Engine States

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/
#include "pch.h"
#include "SceneManager/state-manager.h"

int esCurrent, esNext, esPrevious;

/*******************************************************************************
/*!
* 
\brief
	Initialise Engine State Manager with specified initial state

\param _initialState
	The initial state the engine is to start on

\return
	void
*/
/*******************************************************************************/
void init_statemanager(EngineState _initialState) 
{
	esCurrent = esNext = _initialState;
	std::cout << "Engine State ID: " << _initialState << std::endl;
}

/*******************************************************************************
/*!
*
\brief
	Change the Engine's state to a specified state

\param _nextState
	The new state that the engine is to change to

\return
	void
*/
/*******************************************************************************/
void change_enginestate(EngineState _nextState) 
{
	esCurrent = _nextState;
}

/*******************************************************************************
/*!
*
\brief
	Specifically quit the engine

\return
	void
*/
/*******************************************************************************/
void quit_engine()
{
	esCurrent = esQuit;
}

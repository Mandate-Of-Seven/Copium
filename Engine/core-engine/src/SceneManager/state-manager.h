/*!***************************************************************************************
\file			state-manager.h
\project		
\author			Matthew Lau
\co-authors		

\par			Course: GAM200
\par			Section: 
\date			12/07/2022

\brief
	Contains:
		1. Declarations of functions and extern variables that allow for the management of Engine States
		2. Enums for all the Engine States

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/
#pragma once
#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <iostream>
#include <string>

enum EngineState 
{
	esIdle = 0,
	esActive,
	esQuit,
	esRestart
};

extern int esCurrent;

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
void init_statemanager(EngineState _initialState);
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
void change_enginestate(EngineState _nextState);

/*******************************************************************************
/*!
*
\brief
	Specifically quit the engine

\return
	void
*/
/*******************************************************************************/
void quit_engine();

#endif
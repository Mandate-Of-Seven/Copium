/*!***************************************************************************************
****
\file			StateManager.h
\project		
\author			Matthew Lau
\co-authors		

\par			Course: GAM200
\par			Section: 
\date			12/07/2022

\brief
	Contains:
		1. Declarations of functions and extern variables that allow for the management of Game States
		2. Enums for all the Game States

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************
****/
#pragma once
#ifndef STATEMANAGER_H
#define STATEMANAGER_H

#include <iostream>

//GAMESTATE ID - int type
enum GameState {
	gsTest = 0,
	gsQuit, 
	gsRestart,
	gsSandbox
};

//Alias for Game State Function ptrs
using gsFNPTR = void(*)();

//Extern variables
extern gsFNPTR gsLoad, gsInit, gsUpdate, gsDraw, gsFree, gsUnload;
extern int gsCurrent, gsPrevious, gsNext;

/*******************************************************************************
/*!
\brief
	Initialize the state manager with specified state.

\param _initialState
	The initial game state that the application is to start in.

\return 
	void
*/
/*******************************************************************************/
void init_statemanager(int _initialState);

/*******************************************************************************
/*!
\brief
	Update the gs function pointers to point to the specified game state's functions.
	Note: If a new game state is added to the game, please add a switch case to the function definition

\return
	void
*/
/*******************************************************************************/
void update_statemanager();

/*******************************************************************************
/*!
\brief
	Change the game state to the specified game state

\param _nextGameState
	The game state to change to.

\return 
	void
*/
/*******************************************************************************/
void change_gamestate(GameState _nextGameState);

/*
//Trying out something new - ignore for now pls
class GSM {
	int current, next, previous;

	GSM(){}
	GSM(int initialState){}
};

GSM::GSM() {
	printf("Game State Manager default init\n");

}

GSM::GSM(int initialState) {
	printf("Game State Manager initialised to state %d", initialState);
	current = next = previous = initialState;
}
*/
#endif
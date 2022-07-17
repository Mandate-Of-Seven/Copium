/*!***************************************************************************************
****
\file			StateManager.cpp
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
#include "statemanager.h"

gsFNPTR gsLoad = nullptr, gsInit = nullptr, gsUpdate = nullptr, 
		gsDraw = nullptr, gsFree = nullptr, gsUnload = nullptr;

int gsCurrent = 0, gsPrevious = 0, gsNext = 0;

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
void init_statemanager(int _initialState) {
	gsCurrent = gsPrevious = gsNext = _initialState;
	std::cout << "State Manager Initialised...\n" << "Current State ID: " << _initialState << std::endl;
}

/*******************************************************************************
/*!
\brief
	Update the gs function pointers to point to the specified game state's functions.
	Note: If a new game state is added to the game, please add a switch case to the function definition

\return
	void
*/
/*******************************************************************************/
void update_statemanager() {
	std::cout << "Updating State Manager...\n";
	switch (gsCurrent) {
		case(gsTest):
			break;
	}

}

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
void change_gamestate(GameState _nextGameState) {
	gsNext = _nextGameState;
}

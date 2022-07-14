/*!***************************************************************************************
****
\file			StateManager.cpp
\project		?
\author			Matthew Lau
\co-authors		

\par			Course: CSD????
\par			Section: ?
\date			12-7-2022

\brief
	Contains definitions for the functions and variables that manage the game states.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************
****/
#include "StateManager.h"

gsFNPTR gsLoad = nullptr, gsInit = nullptr, gsUpdate = nullptr, 
		gsDraw = nullptr, gsFree = nullptr, gsUnload = nullptr;

int gsCurrent = 0, gsPrevious = 0, gsNext = 0;

/*!*****************************************************************************
Initialize the state manager with specified state.

@param [in] initialState
The game state the game should begin on

@return void
*******************************************************************************/
void initStateManager(int initialState) {
	gsCurrent = gsPrevious = gsNext = initialState;
	std::cout << "State Manager Initialised...\n" << "Current State ID: " << initialState << std::endl;
}

/*!*****************************************************************************
Update the gs function pointers to point to the specified game state's functions
Note: If a new game state is added to the game, please add a switch case in here

@return void
*******************************************************************************/
void updateStateManager() {
	std::cout << "Updating State Manager...\n";
	switch (gsCurrent) {
		case(gsTest):
			break;
	}

}

/*!*****************************************************************************
Change the game state to the specified game state

@param [in] nextGameState
The Game State to change to 

@return void
*******************************************************************************/
void changeGameState(GameState nextGameState) {
	gsNext = nextGameState;
}

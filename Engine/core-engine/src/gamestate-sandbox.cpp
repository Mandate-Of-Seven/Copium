/*!***************************************************************************************
****
\file			gamestate-sandbox.cpp
\project
\author			Matthew Lau
\co-authors

\par			Course: GAM200
\par			Section:
\date			28/07/2022

\brief
	Contains definitions for the Sandbox Scene which inherits from Scene class.
	The Scene contains:
		1. load, init, update, draw, free, unload functions

	Note: load, init, free and unload functions MUST be defined by scene sub-classes

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************
****/
#include "gamestate-sandbox.h"

SceneSandbox::SceneSandbox(std::string& _filepath) : Scene(_filepath) {

}

void SceneSandbox::loadScene() {
	std::cout << "load sandbox" << std::endl;
}
void SceneSandbox::initScene() {
	std::cout << "init sandbox" << std::endl;
}
void SceneSandbox::updateScene() {
	std::cout << "update sandbox" << std::endl;
}
void SceneSandbox::drawScene() {
	std::cout << "draw sandbox" << std::endl;
}
void SceneSandbox::freeScene() {
	std::cout << "free sandbox" << std::endl;
}
void SceneSandbox::unloadScene() {
	std::cout << "unload sandbox" << std::endl;
}

/*!***************************************************************************************
\file			frame-rate-controller.cpp
\project
\author			Matthew Lau
\co-authors

\par			Course: GAM200
\par			Section:
\date			05/09/2022

\brief
	Contains the definitions for FrameRateController class, its constructor and 
	member functions.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/
#include "pch.h"
#include "Debugging/frame-rate-controller.h"
#include "Windows/windows-system.h"

namespace Copium {
	FrameRateController::FrameRateController(double _maxFPS) :
		frameCount{0}, maxFrameRate{ _maxFPS }, frameRate{ _maxFPS }, minFrameTime{ 1 / _maxFPS },
		frameStart{ 0.0 }, frameEnd{ 0.0 }, dt{ 0.0 }, accumulatedTime{ 0.0 }, stepCount{ 0 } {}

	/*******************************************************************************
	/*!
	*
	\brief
		sets the time that the frame started at.
		Note: this function should be called at the beginning of each game loop

	\return
		void
	*/
	/*******************************************************************************/
	void FrameRateController::start()
	{
		frameStart = glfwGetTime();

	}

	void FrameRateController::update()
	{
		stepCount = 0;
		frameEnd = glfwGetTime();
		dt = frameEnd - frameStart;
		// For the next game loop calculation
		frameStart = glfwGetTime();

		accumulatedTime += dt;

		while (accumulatedTime >= minFrameTime)
		{
			accumulatedTime -= minFrameTime;
			++stepCount;
		}

		//std::cout << "StepCount: " << stepCount << std::endl;


	}

	void FrameRateController::end()
	{
		//frameEnd = glfwGetTime();
		WindowsSystem::Instance()->update_time(0.16);
		frameRate = WindowsSystem::Instance()->get_fps();
		++frameCount;
	}

}



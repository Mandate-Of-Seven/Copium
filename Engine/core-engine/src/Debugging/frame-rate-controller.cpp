/*!***************************************************************************************
****
\file			frameratecontroller.cpp
\project
\author			Matthew Lau
\co-authors

\par			Course: GAM200
\par			Section:
\date			05/09/2022

\brief
	Contains definitions for FrameRateController class, its constructor and member functions.

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/
#include "pch.h"
#include "Debugging/frame-rate-controller.h"
#include "Windows/windows-system.h"

namespace Copium {
	FrameRateController::FrameRateController(double _maxFPS) :
		frameCount{0}, maxFrameRate{ _maxFPS }, frameRate{ _maxFPS }, minFrameTime{ 1 / _maxFPS },
		frameStart{ 0.0 }, frameEnd{ 0.0 }, dt{ 0.0 }, accumulatedTime{ 0.0 }, stepCount{ 0 } {}

	void FrameRateController::start()
	{
		frameStart = glfwGetTime();

	}

	void FrameRateController::update()
	{
		stepCount = 0;

		dt = frameEnd - frameStart;
		// For the next game loop calculation
		frameStart = glfwGetTime();

		accumulatedTime += dt;

		while (accumulatedTime >= minFrameTime)
		{
			accumulatedTime -= minFrameTime;
			++stepCount;
		}

		// Cap the number of steps
		if (stepCount > MAX_STEP_COUNT)
			stepCount = MAX_STEP_COUNT;

		//std::cout << "StepCount: " << stepCount << std::endl;


	}

	void FrameRateController::end()
	{
		frameEnd = glfwGetTime();
		Windows::WindowsSystem::Instance()->update_time(10.0);
		frameRate = Windows::WindowsSystem::Instance()->get_fps();
		++frameCount;
	}

}



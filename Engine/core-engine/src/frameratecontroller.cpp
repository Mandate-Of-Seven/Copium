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

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/
#include "pch.h"
#include "frameratecontroller.h"
#include "windows-system.h"

// Bean: remove after encapsulating the namespace
namespace Copium {
	FrameRateController::FrameRateController(double _maxFPS) :
		frameCount{0}, maxFrameRate{ _maxFPS }, frameRate{ _maxFPS }, minFrameTime{ 1 / _maxFPS },
		frameStart{ 0.0 }, frameEnd{ 0.0 }, frameTime{ 0.0 } {}

	void FrameRateController::start()
	{
		frameStart = glfwGetTime();

	}

	double FrameRateController::end()
	{
		while ((glfwGetTime() - frameStart) < minFrameTime) 
		{
			frameEnd = glfwGetTime();
			frameTime = frameEnd - frameStart;

		}
		GLHelper::update_time(10.0);
		frameRate = GLHelper::fps;
		++frameCount;
		return GLHelper::fps;
	}
}



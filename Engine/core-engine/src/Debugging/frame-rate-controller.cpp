
#include "pch.h"
#include "Debugging/frame-rate-controller.h"
#include "Windows/windows-system.h"

// Bean: remove after encapsulating the namespace
using namespace Copium;

FrameRateController::FrameRateController(double _maxFPS) :
	frameCount{0}, maxFrameRate{ _maxFPS }, frameRate{ _maxFPS }, minFrameTime{ 1 / _maxFPS },
	frameStart{ 0.0 }, frameEnd{ 0.0 }, frameTime{ 0.0 } {}

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

/*******************************************************************************
/*!
*
\brief
	End the controller for the frame. Computes the frame rate, frame time. Increments the frame counter.
	Updates the current FPS. 
	Note: this function should be called at the end of each game loop

\return
	the current FPS
*/
/*******************************************************************************/
double FrameRateController::end()
{
	while ((glfwGetTime() - frameStart) < minFrameTime) 
	{
		frameEnd = glfwGetTime();
		frameTime = frameEnd - frameStart;

	}
	Windows::WindowsSystem::Instance()->update_time(0.016);
	//Windows::windowsSystem.update_time(10.0);
	frameRate = Windows::WindowsSystem::Instance()->get_fps();
	++frameCount;
	return Windows::WindowsSystem::Instance()->get_fps();
}

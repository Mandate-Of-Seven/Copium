/*!***************************************************************************************
\file			frameratecontroller.h
\project
\author			Matthew Lau
\co-authors

\par			Course: GAM200
\par			Section:
\date			05/09/2022

\brief
	Contains declarations for FrameRateController class, its constructor and member functions.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/
#ifndef FRAMERATECONTROLLER_H
#define FRAMERATECONTROLLER_H
#include "CopiumCore/system-interface.h"

#define MAX_STEP_COUNT 2

namespace Copium {
	
	class FrameRateController {
	public:
		FrameRateController(double _maxFPS = 60.0);
		/*******************************************************************************
		/*!
		*
		\brief
			Start the frame rate controller
			Note: this function should be called at the start of each game loop

		\return
			void
		*/
		/*******************************************************************************/
		void start();
		/*******************************************************************************
		/*!
		*
		\brief
			Updates the frame rate controller. Calculates the amount of steps required for current game loop's calculations
			Note: this function should be called BEFORE updating the systems

		\return
			void
		*/
		/*******************************************************************************/
		void update();
		/*******************************************************************************
		/*!
		*
		\brief
			End the frame rate controller for the frame. 
			Sets the frameEnd time and increments the frame count.
			Note: this function should be called at the end of each game loop

		\return
			void
		*/
		/*******************************************************************************/
		void end();
	private:
		int frameCount;
		double frameStart, frameEnd;
		double minFrameTime;
		double maxFrameRate;
		double frameRate;
		double dt;
		double accumulatedTime;
		int stepCount;
	};
}



#endif //FRAMERATECONTROLLER_H

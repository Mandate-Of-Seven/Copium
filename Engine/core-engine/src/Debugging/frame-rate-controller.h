/*!***************************************************************************************
\file			frameratecontroller.h
\project
\author			Matthew Lau
\co-authors

\par			Course: GAM250
\par			Section:
\date			05/09/2022

\brief
	Contains declarations for FrameRateController class, its constructor and member functions.

All content � 2023 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/

#define MyFrameRateController (*Copium::FrameRateController::Instance())

#ifndef FRAMERATECONTROLLER_H
#define FRAMERATECONTROLLER_H
#include "CopiumCore/system-interface.h"

#define MAX_STEP_COUNT 2

namespace Copium {
	
	class FrameRateController : public Singleton<FrameRateController>{
	public:
		void init(double _maxFPS = 60.0);
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
			End the frame rate controller for the frame. 
			Sets the frameEnd time and increments the frame count.
			Note: this function should be called at the end of each game loop

		\return
			void
		*/
		/*******************************************************************************/
		void end();
	private:
		double frameCount {0};
		double frameStart {0}, frameEnd {0};
		double deltaTime {0};
		double fixedDeltaTime {0};
		double frameRate {0};
		double maxFrameRate {0};
		double accumulatedTime {0};
		size_t steps{0};
	public:
		double getDt() const { return deltaTime; }
		double getFixedDt() const{ return fixedDeltaTime; }
		double getFPS() const { return frameRate; }
		size_t getSteps() const{ return steps; }
	};
}



#endif //FRAMERATECONTROLLER_H

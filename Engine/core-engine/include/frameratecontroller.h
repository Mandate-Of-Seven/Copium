/*!***************************************************************************************
****
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
			End the frame rate controller for the frame. Ensures that the minimum frame time is met.
			Updates the current frame count, frame rate and frame time of the current frame.
			Note: this function should be called at the end of each game loop

		\return
			the currrent FPS
		*/
		/*******************************************************************************/
		double end();
	private:
		int frameCount;
		double frameStart, frameEnd;
		double minFrameTime;
		double maxFrameRate;
		double frameRate;
		double frameTime;
	};
}



#endif //FRAMERATECONTROLLER_H

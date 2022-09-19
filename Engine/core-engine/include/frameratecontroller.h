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

class FrameRateController {
public:
	FrameRateController(double _maxFPS = 60.0);
	void start();
	double end();
private:
	int frameCount;
	double frameStart, frameEnd;
	double minFrameTime;
	double maxFrameRate;
	double frameRate;
	double frameTime;
};


#endif //FRAMERATECONTROLLER_H

/*!***************************************************************************************
\file			thread-system.cpp
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			26/09/2022

\brief
	Defines Thread-system class functions

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#include "pch.h"
#include "Utilities/thread-system.h"

namespace Copium
{
	void ThreadSystem::init()
	{
		systemFlags |= FLAG_RUN_ON_EDITOR | FLAG_RUN_ON_PLAY;
		quit = false;
	}

	void ThreadSystem::update()
	{

	}

	void ThreadSystem::addThread(std::thread* _thread)
	{
		threads.push_back(_thread);
	}

	void ThreadSystem::exit()
	{
		quit = true;
		PRINT("Exiting all threads...");
		for (std::thread* thread : threads)
		{
			thread->join();
			delete thread;
		}
		PRINT("All threads exited");
	}

	bool ThreadSystem::Quit() const
	{
		return quit;
	}
}
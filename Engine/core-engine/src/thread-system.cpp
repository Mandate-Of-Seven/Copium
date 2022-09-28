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
#include "thread-system.h"

namespace Copium::Thread
{
	void ThreadSystem::init()
	{
		quit = false;
		using namespace Message;
		MessageSystem::Instance()->subscribe(MESSAGE_TYPE::MT_ENGINE_EXIT, this);
	}

	void ThreadSystem::update()
	{

	}

	void ThreadSystem::addThread(const char* _name, std::thread* _thread)
	{
		PRINT("Thread: " << _name << " added!");
		threads.push_back(std::make_pair(_name, _thread));
	}

	void ThreadSystem::exit()
	{
		for (namedThread& thread : threads)
		{
			delete thread.second;
		}
	}

	bool ThreadSystem::Quit() const
	{
		return quit;
	}

	void ThreadSystem::handleMessage(Message::MESSAGE_TYPE mType)
	{
		quit = true;
		//Quit
		PRINT("Quitting Threads");
		for (namedThread& thread : threads)
		{
			thread.second->join();
		}
		PRINT("All threads detached");
	}


}
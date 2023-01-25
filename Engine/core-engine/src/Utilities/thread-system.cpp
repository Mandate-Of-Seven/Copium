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
#include <Events/events-system.h>

namespace Copium
{
	void ThreadSystem::Awake()
	{
		MyEventSystem.subscribe(this, &ThreadSystem::CallbackCreateThread);
		MyEventSystem.subscribe(this, &ThreadSystem::CallbackGetThreadState);
		MyEventSystem.subscribe(this, &ThreadSystem::CallbackAcquireMutex);
		MyEventSystem.subscribe(this, &ThreadSystem::CallbackReturnMutex);
		threads.reserve(10);
	}

	void ThreadSystem::Init()
	{
		systemFlags |= FLAG_RUN_ON_EDITOR | FLAG_RUN_ON_PLAY;
	}

	void ThreadSystem::Update()
	{

	}

	void ThreadSystem::CallbackCreateThread(CreateThreadEvent* pEvent)
	{
		threadsStates[pEvent->rThread.get_id()] = true;
		threads.push_back(std::move(pEvent->rThread));
	}

	void ThreadSystem::CallbackGetThreadState(GetThreadStateEvent* pEvent)
	{
		pEvent->state = threadsStates[pEvent->threadID];
	}

	void ThreadSystem::CallbackAcquireMutex(AcquireMutexEvent* pEvent)
	{
		//Mutex is in use
		if (mutexes[pEvent->mutexType] == true)
		{
			pEvent->result = false;
		}
		else
		{
			mutexes[pEvent->mutexType] = true;
			pEvent->result = true;
		}
	}

	void ThreadSystem::CallbackReturnMutex(ReturnMutexEvent* pEvent)
	{
		mutexes[pEvent->mutexType] = false;
	}

	void ThreadSystem::Exit()
	{
		PRINT("Exiting all threads...");
		for (auto& threadIdState : threadsStates)
		{
			threadIdState.second = false;
		}
		for (std::thread& thread : threads)
		{
			thread.join();
		}
		PRINT("All threads exited");
	}
}
/*!***************************************************************************************
\file			thread-system.h
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			26/09/2022

\brief
	Contains declarations for ThreadSystem that mainly tells running threads to quit

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/

#pragma once
#include <thread>
#include <utility>
#include <unordered_map>
#include <Events/events.h>

namespace Copium
{
	CLASS_SYSTEM(ThreadSystem)	{
	public:
		void Awake();

		/*******************************************************************************
		/*!
		*
			\brief
				Initialise the system at engine start
		*/
		/*******************************************************************************/
		void Init();
		/*******************************************************************************
		/*!
		*
			\brief
				Runs at engine update
		*/
		/*******************************************************************************/
		void Update();
		/*******************************************************************************
		/*!
		*
			\brief
				Runs when engine exits
		*/
		/*******************************************************************************/
		void Exit();

		void CallbackCreateThread(CreateThreadEvent* pEvent);
		void CallbackGetThreadState(GetThreadStateEvent* pEvent);
		void CallbackAcquireMutex(AcquireMutexEvent* pEvent);
		void CallbackReturnMutex(ReturnMutexEvent* pEvent);
	private:
		std::vector<std::thread> threads;
		std::unordered_map<std::thread::id,bool> threadsStates;
		std::unordered_map<std::string, bool> mutexes;
	};
}
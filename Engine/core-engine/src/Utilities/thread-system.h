/*!***************************************************************************************
\file			thread-system.h
\project
\author			Zacharie Hong

\par			Course: GAM250
\par			Section:
\date			26/09/2022

\brief
	Contains declarations for ThreadSystem that mainly tells running threads to quit

All content � 2023 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/

#pragma once
#include "Messaging/message-system.h"
#include "Messaging/message-types.h"
#include <thread>
#include <utility>
#include <unordered_map>

namespace Copium
{
	enum class MutexType
	{
		FileSystem,
		None
	};


	CLASS_SYSTEM(ThreadSystem)	{
	public:
		/*******************************************************************************
		/*!
		*
			\brief
				Initialise the system at engine start
		*/
		/*******************************************************************************/
		void init();
		/*******************************************************************************
		/*!
		*
			\brief
				Runs at engine update
		*/
		/*******************************************************************************/
		void update();
		/*******************************************************************************
		/*!
		*
			\brief
				Runs when engine exits
		*/
		/*******************************************************************************/
		void exit();

		/*******************************************************************************
		/*!
		*
			\brief
				Runs when engine exits
			\param _name
				Thread name
			\param _thread
				Thread to keep track of
		*/
		/*******************************************************************************/
		void addThread(std::thread* _thread);


		/*******************************************************************************
		/*!
		*
			\brief
				Gets a mutex for a thread to run its critical section
			\param mutexType
				Type of mutex to acquire
			\return
				True if acquired
		*/
		/*******************************************************************************/
		bool acquireMutex(MutexType mutexType)
		{
			if (mutexes[mutexType] == 1)
				return 0;
			mutexes[mutexType] = 1;
			return 1;
		}

		/*******************************************************************************
		/*!
		*
			\brief
				Returns a mutex
			\param mutexType
				Type of mutex to return
		*/
		/*******************************************************************************/
		void returnMutex(MutexType mutexType)
		{
			mutexes[mutexType] = 0;
		}

		/*******************************************************************************
		/*!
		*
			\brief
				Checks whether thread system has been told to exit
			\return
				Returns the value of quit member variable
		*/
		/*******************************************************************************/
		bool Quit() const;

		/*******************************************************************************
		/*!
		*
			\brief
				Tracks when to quit and exit all threads
		*/
		/*******************************************************************************/
		void handleMessage(MESSAGE_TYPE);
	private:
		std::vector<std::thread*> threads;
		std::unordered_map<MutexType, bool> mutexes;
		bool quit = false;
	};
}
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
#include "message-system.h"
#include "message-types.h"
#include <thread>
#include <utility>

namespace Copium::Thread
{
	CLASS_SYSTEM(ThreadSystem), public Message::IReceiver
	{
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
		void addThread(const char* _name, std::thread* _thread);

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
		void handleMessage(Message::MESSAGE_TYPE);
	private:
		using namedThread = std::pair < const char*, std::thread*>;
		std::vector<namedThread> threads;
		bool quit;
	};
}
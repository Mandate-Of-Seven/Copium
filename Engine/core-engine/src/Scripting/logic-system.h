﻿/*!***************************************************************************************
\file			logic-system.h
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			30/10/2022

\brief
	This file declares the functions for the Logic System

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#pragma once

#ifndef LOGIC_SYSTEM_H

#define LOGIC_SYSTEM_H

//INCLUDES
#include "CopiumCore/system-interface.h"
#include "Messaging/message-system.h"
#include <Events/events-system.h>
//USING

namespace Copium
{

	CLASS_SYSTEM(LogicSystem) , public IReceiver
	{
	public:
		/**************************************************************************/
		/*!
			\brief
				Inits logic system
		*/
		/**************************************************************************/
		void init();
		/**************************************************************************/
		/*!
			\brief
				Calls the update function of UI buttons and Script Components
		*/
		/**************************************************************************/
		void update();
		/**************************************************************************/
		/*!
			\brief
				Cleanup of logic system
		*/
		/**************************************************************************/
		void exit();
		/**************************************************************************/
		/*!
			\brief
				Listens to Start play mode so it can call the awake and start of all scripts

			\param mType
				Type of message dispatched
		*/
		/**************************************************************************/
		void handleMessage(MESSAGE_TYPE mType);

		/**************************************************************************/
		/*!
			\brief
				Creates a script using the parameter

			\param pEvent
				pointer to the ScriptCreatedEvent which will be used to create the script

			\return 
				void
		*/
		/**************************************************************************/
		void CallbackScriptCreated(ScriptCreatedEvent* pEvent);
		/**************************************************************************/
		/*!
			\brief
				Destroy a script

			\param pEvent
				pointer to the ScriptCreatedEvent which will be used to destroy the script

			\return
				void
		*/
		/**************************************************************************/
		void CallbackScriptDestroyed(ScriptDestroyedEvent* pEvent);
	};
}



#endif // !LOGIC_SYSTEM_H

/*!***************************************************************************************
\file			copium-core.h
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			27/09/2022

\brief
	This file holds the definitions of functions for copium-core.cpp

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#pragma once
#include "CopiumCore/system-interface.h"
#include "Windows/windows-system.h"
#include "Windows/windows-input.h"
#include "Messaging/message-system.h"
#include "Files/assets-system.h"
#include "Editor/editor-system.h"
#include "Scripting/scripting-system.h"
#include "Physics/physics-system.h"
#include "Graphics/graphics-system.h"
#include "Utilities/thread-system.h"
#include "Debugging/frame-rate-controller.h"
#include "SceneManager/sm.h"
#include "Debugging/logging-system.h"
#include "Audio/sound-system.h"
#include "Scripting/logic-system.h"

namespace Copium
{
	CLASS_SYSTEM(CopiumCore)
	{
	public:
		CopiumCore() : frc{ nullptr } {}

		/**************************************************************************/
		/*!
		  \brief
			Initializes all the systems that are Instantiated under vector systems
		*/
		/**************************************************************************/
		void init()
		{
			systems =
			{
				//Put in sequence of calls
				WindowsSystem::Instance(),
				LoggingSystem::Instance(),
				NewSceneManager::Instance(),
				MessageSystem::Instance(),
				FileSystem::Instance(),
				AssetsSystem::Instance(),
				InputSystem::Instance(),
				EditorSystem::Instance(),
				LogicSystem::Instance(),
				PhysicsSystem::Instance(),
				GraphicsSystem::Instance(),
				SoundSystem::Instance()	,
				ScriptingSystem::Instance(),
				ThreadSystem::Instance()
			};
			for (ISystem* pSystem : systems)
			{
				pSystem->init();
			}

			frc = new FrameRateController;

		}

		/**************************************************************************/
		/*!
		  \brief
			Calls the update function of all the systems in vector systems
		*/
		/**************************************************************************/
		void update()
		{
			frc->update();
			for (ISystem* pSystem : systems)
			{
				//Time start
				//PRINT("UPDATE: " << typeid(*pSystem).name());
				pSystem->update();
				//Time End
			}
			frc->end();
		}

		/**************************************************************************/
		/*!
		  \brief
			Calls the exit function of all the systems in vector systems, to be 
			called at engine exit
		*/
		/**************************************************************************/
		void exit()
		{
			for (int i = systems.size() - 1; i >= 0; --i)
			{
				systems[i]->exit();
			}

			delete frc;
			frc = nullptr;
		}
	private:
		std::vector<ISystem*> systems;
		FrameRateController* frc;
	};
}
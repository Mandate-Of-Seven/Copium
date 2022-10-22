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
#include "../Messaging/message-system.h"
#include "Editor/editor-system.h"
#include "Scripting/scripting-system.h"
#include "Physics/physics-system.h"
#include "Graphics/graphics-system.h"
#include "SceneManager/sm.h"
#include "Utilities/thread-system.h"
#include "Debugging/frame-rate-controller.h"

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
				Windows::WindowsSystem::Instance(),
				Message::MessageSystem::Instance(),
				Editor::EditorSystem::Instance(),
				Files::FileSystem::Instance(),
				NewSceneManager::Instance(),
				Physics::Physics::Instance(),
				Graphics::GraphicsSystem::Instance(),
				SoundSystem::Instance(),
				Scripting::ScriptingSystem::Instance(),
				Thread::ThreadSystem::Instance(),
			};
			for (ISystem* pSystem : systems)
			{
				pSystem->init();
			}

			frc = new FrameRateController;

			// Testing archetype registering at init
			double start = glfwGetTime();
			NewSceneManager::Instance()->get_gof().register_archetypes("Data/Archetypes");
			double timeTaken = glfwGetTime() - start;
			std::cout << "Time taken to register all archetypes: " << timeTaken << std::endl;
			NewSceneManager::Instance()->get_gof().build_gameobject("Default");

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
				pSystem->update();
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
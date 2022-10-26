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

			// Testing archetype registering at init
			//double start = glfwGetTime();
			//NewSceneManager::Instance()->get_gof().register_archetypes("Data/Archetypes");
			//double timeTaken = glfwGetTime() - start;
			//std::cout << "Time taken to register all archetypes: " << timeTaken << std::endl;
			//NewSceneManager::Instance()->get_gof().build_gameobject("Default");

		}

		/**************************************************************************/
		/*!
		  \brief
			Calls the update function of all the systems in vector systems
		*/
		/**************************************************************************/
		void update()
		{
			double totalUpdateTime = 0;
			frc->update();
			for (ISystem* pSystem : systems)
			{
				double startTime = glfwGetTime();
				pSystem->update();
				pSystem->updateTime = glfwGetTime() - startTime;
				totalUpdateTime += pSystem->updateTime;
			}

			if (displayPerformance)
			{
				std::cout<<"Start\n";
				for (ISystem* pSystem : systems)
				{

					pSystem->updateTimePercent = (pSystem->updateTime / totalUpdateTime) * 100;
					std::cout << typeid(*pSystem).name() << ": " << pSystem->updateTimePercent << "%\n";

				}
				std::cout << "End\n\n";
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

		void toggle_display_peformance() {displayPerformance = !displayPerformance;}
		//bool get_display_peformance() {return displayPerformance}
	private:
		std::vector<ISystem*> systems;
		FrameRateController* frc;
		bool displayPerformance = false;
	};
}
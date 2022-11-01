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
//#include "string.h"

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
				MessageSystem::Instance(),
				LoggingSystem::Instance(),
				NewSceneManager::Instance(),
				SoundSystem::Instance()	,
				FileSystem::Instance(),
				AssetsSystem::Instance(),
				InputSystem::Instance(),
				EditorSystem::Instance(),
				LogicSystem::Instance(),
				PhysicsSystem::Instance(),
				GraphicsSystem::Instance(),
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
			Calls the update function of all the systems in vector systems depending
			if it should only be update in play mode or not
		*/
		/**************************************************************************/
		void update()
		{
			double totalUpdateTime = 0;
			frc->update();
			for (ISystem* pSystem : systems)
			{
				if (pSystem->systemFlags & FLAG_RUN_ON_PLAY && inPlayMode)
				{
					double startTime = glfwGetTime();
					pSystem->update();
					pSystem->updateTime = glfwGetTime() - startTime;
					totalUpdateTime += pSystem->updateTime;
				}
				else if (pSystem->systemFlags & FLAG_RUN_ON_EDITOR &&!inPlayMode)
				{
					pSystem->update();
					continue;
				}
			}

			if (displayPerformance)
			{
				if (performanceCounter >= 0.05f)
				{
					//std::cout<<"Start\n";
					std::string temp = "\n";
					for (ISystem* pSystem : systems)
					{

						pSystem->updateTimePercent = (pSystem->updateTime / totalUpdateTime) * 100;
						temp += typeid(*pSystem).name();
						temp += ": ";
						temp += std::to_string(pSystem->updateTimePercent);
						temp += "%%\n\n";
						//std::cout << typeid(*pSystem).name() << ": " << pSystem->updateTimePercent << "%\n";
					}
					Window::EditorConsole::editorLog.set_performancetext(temp);
					//std::cout << "End\n\n";
					performanceCounter = 0;
				}
				else
				{
					performanceCounter += (float)Copium::WindowsSystem::Instance()->get_delta_time();
				}
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
			for(int i = (int)systems.size() - 1; i >= 0; --i)
			{
				systems[i]->exit();
			}

			delete frc;
			frc = nullptr;
		}

		// getter /setters
		void toggle_display_peformance() { displayPerformance = !displayPerformance; performanceCounter = 0.05f; }
		void toggle_inplaymode() { inPlayMode = !inPlayMode; }
		bool get_inplaymode() { return inPlayMode; }
	private:
		std::vector<ISystem*> systems;
		FrameRateController* frc;
		float performanceCounter = 0;
		bool displayPerformance = false;
		bool inPlayMode = false;
	};
}
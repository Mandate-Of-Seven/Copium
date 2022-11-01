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
	CLASS_SYSTEM(CopiumCore) , public IReceiver
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
			MessageSystem* pMessageSystem = MessageSystem::Instance();
			systems =
			{
				//Put in sequence of calls
				WindowsSystem::Instance(),
				pMessageSystem,
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

			pMessageSystem->subscribe(MESSAGE_TYPE::MT_START_PREVIEW, this);
			pMessageSystem->subscribe(MESSAGE_TYPE::MT_STOP_PREVIEW, this);
			pMessageSystem->subscribe(MESSAGE_TYPE::MT_TOGGLE_PERFORMANCE_VIEW, this);

			frc = new FrameRateController;

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
			for(int i = (int)systems.size() - 1; i >= 0; --i)
			{
				systems[i]->exit();
			}

			delete frc;
			frc = nullptr;
		}

		void handleMessage(MESSAGE_TYPE mType)
		{
			switch (mType)
			{
				case MESSAGE_TYPE::MT_START_PREVIEW:
				{
					inPlayMode = true;
					break;
				}
				case MESSAGE_TYPE::MT_STOP_PREVIEW:
				{
					inPlayMode = false;
					break;
				}
				case MESSAGE_TYPE::MT_TOGGLE_PERFORMANCE_VIEW:
				{
					displayPerformance = !displayPerformance;
					break;
				}
			}
		}

		bool get_inplaymode() { return inPlayMode; }
	private:
		std::vector<ISystem*> systems;
		FrameRateController* frc;
		bool displayPerformance = false;
		bool inPlayMode = false;
	};
}
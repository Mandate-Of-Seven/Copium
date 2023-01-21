/*!***************************************************************************************
\file			copium-core.h
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			27/09/2022

\brief
	This file holds the definitions of functions for copium-core.cpp

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef COPIUM_CORE_H
#define COPIUM_CORE_H

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
#include "SceneManager/scene-manager.h"
#include "Debugging/logging-system.h"
#include "Audio/sound-system.h"
#include "Scripting/logic-system.h"
#include <Debugging/frame-rate-controller.h>
#include "Animation/animation-system.h"
//#include "string.h"

namespace Copium
{
	CLASS_SYSTEM(CopiumCore) , public IReceiver
	{
	public:
		CopiumCore() {}

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
				SoundSystem::Instance(),
				FileSystem::Instance(),
				AssetsSystem::Instance(),
				SceneManager::Instance(),
				ScriptingSystem::Instance(),
				InputSystem::Instance(),
				EditorSystem::Instance(),
				LogicSystem::Instance(),
				PhysicsSystem::Instance(),
				GraphicsSystem::Instance(),
				ThreadSystem::Instance(),
				AnimationSystem::Instance()
			};
			for (ISystem* pSystem : systems)
			{
				pSystem->init();
			}

			pMessageSystem->subscribe(MESSAGE_TYPE::MT_START_PREVIEW, this);
			pMessageSystem->subscribe(MESSAGE_TYPE::MT_STOP_PREVIEW, this);
			pMessageSystem->subscribe(MESSAGE_TYPE::MT_TOGGLE_PERFORMANCE_VIEW, this);
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
			for (ISystem* pSystem : systems)
			{
				if (pSystem->systemFlags & FLAG_RUN_ON_PLAY && inPlayMode)
				{
					double startTime = glfwGetTime();
					pSystem->update();
					pSystem->updateTime = glfwGetTime() - startTime;
					totalUpdateTime += pSystem->updateTime;
				}
				else if (pSystem->systemFlags & FLAG_RUN_ON_EDITOR && !inPlayMode)
				{
					double startTime = glfwGetTime();
					pSystem->update();
					pSystem->updateTime = glfwGetTime() - startTime;
					totalUpdateTime += pSystem->updateTime;
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
						pSystem->updateTimePercent = (pSystem->updateTime<=0) ? 0:((pSystem->updateTime / totalUpdateTime) * 100);
						//std::cout<< pSystem->updateTime << "\n";
						temp += typeid(*pSystem).name();
						temp += ": ";
						temp += std::to_string(pSystem->updateTimePercent);
						temp += "%%\n\n";
						//std::cout << typeid(*pSystem).name() << ": " << pSystem->updateTimePercent << "%\n";
					}
					//std::cout << temp;
					Window::EditorConsole::editorLog.set_performancetext(temp);
					//std::cout << "End\n\n";
					performanceCounter = 0;
				}
				else
				{
					performanceCounter += (float)MyFrameRateController.getDt();
				}
			}
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
					performanceCounter = 0.05f;
					break;
				}
			}
		}

		bool get_inplaymode() { return inPlayMode; }
	private:
		std::vector<ISystem*> systems;
		float performanceCounter = 0;
		bool displayPerformance = false;
		bool inPlayMode = false;
	};
}
#endif // !COPIUM_CORE_H
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

namespace Copium
{
	CLASS_SYSTEM(CopiumCore)
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
			systems =
			{
				//Put in sequence of calls
				Windows::WindowsSystem::Instance(),
				Message::MessageSystem::Instance(),
				Editor::EditorSystem::Instance(),
				Files::FileSystem::Instance(),
				Scripting::ScriptingSystem::Instance(),
				Physics::Physics::Instance(),
				Graphics::GraphicsSystem::Instance(),
				NewSceneManager::Instance(),
				Thread::ThreadSystem::Instance()
			};
			for (ISystem* pSystem : systems)
			{
				pSystem->init();
			}
		}

		/**************************************************************************/
		/*!
		  \brief
			Calls the update function of all the systems in vector systems
		*/
		/**************************************************************************/
		void update()
		{
			for (ISystem* pSystem : systems)
			{
				pSystem->update();
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
			for (int i = systems.size()-1; i > 0; --i)
			{
				systems[i]->exit();
			}
		}
	private:
		std::vector<ISystem*> systems;
	};
}
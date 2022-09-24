#pragma once
#include "system-interface.h"
#include "windows-system.h"
#include "message-system.h"
#include "editor-system.h"
#include "scripting-system.h"
#include "graphics-system.h"

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
				Scripting::ScriptingSystem::Instance(),
				Graphics::GraphicsSystem::Instance()
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
			for (ISystem* pSystem : systems)
			{
				pSystem->exit();
			}
		}
	private:
		std::vector<ISystem*> systems;
	};
}
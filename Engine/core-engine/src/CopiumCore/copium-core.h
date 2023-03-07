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
#include <Events/events.h>


namespace Copium
{
	CLASS_SYSTEM(CopiumCore)
	{
	public:

		/**************************************************************************/
		/*!
		  \brief
			Initializes all the systems that are Instantiated under vector systems
		*/
		/**************************************************************************/
		void init();

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
					//double startTime = glfwGetTime();
					pSystem->update();
					//PRINT(typeid(*pSystem).name() << ": update!");
					//pSystem->updateTime = glfwGetTime() - startTime;
					//totalUpdateTime += pSystem->updateTime;
				}
				else if (pSystem->systemFlags & FLAG_RUN_ON_EDITOR && !inPlayMode)
				{
					//double startTime = glfwGetTime();
					pSystem->update();
					//PRINT(typeid(*pSystem).name() << ": update!");
					//pSystem->updateTime = glfwGetTime() - startTime;
					//totalUpdateTime += pSystem->updateTime;
					continue;
				}
			}

			if (displayPerformance)
			{
				if (performanceCounter >= 0.05f)
				{
					//PRINT("Start");
					std::string temp = "\n";
					for (ISystem* pSystem : systems)
					{
						pSystem->updateTimePercent = (pSystem->updateTime <= 0) ? 0 : ((pSystem->updateTime / totalUpdateTime) * 100);
						//PRINT(pSystem->updateTime);
						temp += typeid(*pSystem).name();
						temp += ": ";
						temp += std::to_string(pSystem->updateTimePercent);
						temp += "%%\n\n";
						PRINT(typeid(*pSystem).name() << ": " << pSystem->updateTimePercent << "%");
					}
					//PRINT(temp);
					//Window::EditorConsole::editorLog.set_performancetext(temp);
					//PRINT("End\n");
					performanceCounter = 0;
				}
				else
				{
					//performanceCounter += (float)MyFrameRateController.getDt();
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

		//void handleMessage(MESSAGE_TYPE mType)
		//{
		//	switch (mType)
		//	{
		//		case MESSAGE_TYPE::MT_START_PREVIEW:
		//		{
		//			inPlayMode = true;
		//			break;
		//		}
		//		case MESSAGE_TYPE::MT_STOP_PREVIEW:
		//		{
		//			inPlayMode = false;
		//			break;
		//		}
		//		case MESSAGE_TYPE::MT_TOGGLE_PERFORMANCE_VIEW:
		//		{
		//			displayPerformance = !displayPerformance;
		//			performanceCounter = 0.05f;
		//			break;
		//		}
		//	}
		//}


		void CallbackStartPreview(StartPreviewEvent* pEvent);

		void CallbackStopPreview(StopPreviewEvent* pEvent);

		bool get_inplaymode() { return inPlayMode; }
	private:
		std::vector<ISystem*> systems;
		float performanceCounter = 0;
		bool displayPerformance = false;
		bool inPlayMode = false;
		std::string configFilePath = "Data\\config.json";
	};
}
#endif // !COPIUM_CORE_H
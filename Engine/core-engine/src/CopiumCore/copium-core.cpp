/*!***************************************************************************************
\file			copium-core.cpp
\project
\author			Zacharie Hong
\co-author		Matthew Lau

\par			Course: GAM250
\par			Section:
\date			27/01/2023

\brief
	This file holds the definitions of functions.

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include <pch.h>
#include <CopiumCore/copium-core.h>
#include "Windows/windows-system.h"
#include "Windows/windows-input.h"
#include "Messaging/message-system.h"
#include "Files/assets-system.h"
#include "Editor/editor-system.h"
#include "Scripting/scripting-system.h"
#include "Physics/physics-system.h"
#include "Graphics/graphics-system.h"
#include "Utilities/thread-system.h"
//#include "SceneManager/scene-manager.h"
//#include "Debugging/logging-system.h"
#include "Audio/sound-system.h"
#include <Files/file-system.h>
#include "Scripting/logic-system.h"
#include <Debugging/frame-rate-controller.h>
#include "Animation/animation-system.h"
//#include "string.h"
#include <Events/events-system.h>

namespace Copium
{

	void CopiumCore::init()
	{
		MessageSystem* pMessageSystem = MessageSystem::Instance();
		systems =
		{
			//Put in sequence of calls
			MyEventSystem,
			WindowsSystem::Instance(),
			pMessageSystem,
			LoggingSystem::Instance(),
			SoundSystem::Instance(),
			FileSystem::Instance(),
			AssetsSystem::Instance(),
			ScriptingSystem::Instance(),
			SceneManager::Instance(),
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
			//PRINT(typeid(*pSystem).name() << ": init!");
		}

		MyEventSystem->subscribe(this, &CopiumCore::CallbackStartPreview);
		MyEventSystem->subscribe(this, &CopiumCore::CallbackStopPreview);
		MyEventSystem->subscribe(this, &CopiumCore::CallbackPerformanceView);
		MyEventSystem->subscribe(this, &CopiumCore::CallbackOffPerformanceView);
		//while (MyScriptingSystem.compilingState == CompilingState::Compiling);
		//MySceneManager.load_scene(Paths::assetPath+"\\Scenes\\SplashScreen.scene");

	}

	void CopiumCore::CallbackStartPreview(StartPreviewEvent* pEvent)
	{
		inPlayMode = true;
	}

	void CopiumCore::CallbackStopPreview(StopPreviewEvent* pEvent)
	{
		inPlayMode = false;
	}

	void CopiumCore::CallbackPerformanceView(ActivatePerformanceViewerEvent* pEvent)
	{
		displayPerformance = true;
		if(!displayPerformance)
			performanceCounter = 2.5f;
		pEvent->textBuffer = tmpBuffer;
	}

	void CopiumCore::CallbackOffPerformanceView(OffPerformanceViewerEvent* pEvent)
	{
		displayPerformance = false;
		performanceCounter = 0.f;
		tmpBuffer.clear();
	}
}
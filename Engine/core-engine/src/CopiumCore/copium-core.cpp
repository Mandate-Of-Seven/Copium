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
			//std::cout << typeid(*pSystem).name() << ": init!\n";
		}

		MyEventSystem->subscribe(this, &CopiumCore::CallbackStartPreview);
		MyEventSystem->subscribe(this, &CopiumCore::CallbackStopPreview);
		//pMessageSystem->subscribe(MESSAGE_TYPE::MT_TOGGLE_PERFORMANCE_VIEW, this);
		//while (MyScriptingSystem.compilingState == CompilingState::Compiling);
		//MySceneManager.load_scene(Paths::assetPath+"\\Scenes\\MainMenu.scene");
		//MySceneManager.load_scene("C:\\Users\\FLESH\\Desktop\\Copium\\Engine\\x64\\PackedTracks\\Assets\\Scenes\\Demo.scene");

	}

	void CopiumCore::CallbackStartPreview(StartPreviewEvent* pEvent)
	{
		inPlayMode = true;
	}

	void CopiumCore::CallbackStopPreview(StopPreviewEvent* pEvent)
	{
		inPlayMode = false;
	}
}
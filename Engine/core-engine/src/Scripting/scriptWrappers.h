#pragma once

#include "Windows\windows-input.h"
#include "SceneManager\sm.h"
#include <glm/vec3.hpp>

namespace Copium::Scripting
{

	extern "C"
	{
		void mono_add_internal_call(const char*, const void*);
	}

	#define Register(METHOD) mono_add_internal_call("CopiumEngine.InternalCalls::"#METHOD,METHOD)

	#pragma region Input

		namespace
		{
			Copium::InputSystem& inputSystem{ *Copium::InputSystem::Instance() };
		}

		static bool GetKeyDown(int keyCode)
		{
			return inputSystem.is_key_pressed(keyCode);
		}

		static bool GetKeyUp(int keyCode)
		{
			return false;
		}

		static bool GetKey(int keyCode)
		{
			return inputSystem.is_key_held(keyCode);
		}

	#pragma endregion Input
	
	#pragma region Transform

		namespace
		{
			Copium::NewSceneManager& sceneManager{ *Copium::NewSceneManager::Instance() };
		}

		static void GetTranslation(GameObjectID _ID, Math::Vec3* translation)
		{
			GameObject* gameObj = sceneManager.findGameObjByID(_ID);
			COPIUM_ASSERT(!gameObj, "Could not find game object with ID");
			*translation = gameObj->Transform().position;
		}

		static void SetTranslation(GameObjectID _ID, Math::Vec3* val)
		{
			GameObject* gameObj = sceneManager.findGameObjByID(_ID);
			COPIUM_ASSERT(!gameObj, "Could not find game object with ID");
			gameObj->Transform().position = *val;
		}


	#pragma endregion Transform

	static void registerScriptWrappers()
	{
		Register(GetKey);
		Register(GetTranslation);
		Register(SetTranslation);
	}
}



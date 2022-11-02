#pragma once

#include "Windows\windows-input.h"
#include "SceneManager\sm.h"
#include <glm/vec3.hpp>

#include "mono/metadata/object.h"
#include "mono/metadata/reflection.h"

namespace Copium
{
	#define Register(METHOD) mono_add_internal_call("CopiumEngine.InternalCalls::"#METHOD,METHOD)

	#pragma region Input

		namespace
		{
			InputSystem& inputSystem{ *InputSystem::Instance() };
		}

		//static bool GetKeyDown(int keyCode)
		//{
		//	return inputSystem.is_key_pressed(keyCode);
		//}

		//static bool GetKeyUp(int keyCode)
		//{
		//	return false;
		//}

		static bool GetKey(int keyCode)
		{
			return inputSystem.is_key_held(keyCode);
		}

	#pragma endregion Input
	
	#pragma region Transform

		namespace
		{
			NewSceneManager& sceneManager{ *NewSceneManager::Instance() };
		}

		static void GetTranslation(GameObjectID _ID, Math::Vec3* translation)
		{
			GameObject* gameObj = sceneManager.findGameObjByID(_ID);
			if (gameObj == nullptr)
			{
				return;
			}
			*translation = gameObj->Transform().position;
		}

		static void SetTranslation(GameObjectID _ID, Math::Vec3* val)
		{
			GameObject* gameObj = sceneManager.findGameObjByID(_ID);
			if (gameObj == nullptr)
			{
				return;
			}
			gameObj->Transform().position = *val;
		}

		static GameObjectID FindGameObjByName(MonoString* name)
		{
			char* nameCStr = mono_string_to_utf8(name);
			GameObject* gameObj = sceneManager.findGameObjByName(nameCStr);
			mono_free(nameCStr);
			if (gameObj == nullptr)
			{
				return -1;
			}
			return gameObj->id;
		}

	#pragma endregion Transform



	static void registerScriptWrappers()
	{
		Register(GetKey);
		Register(GetTranslation);
		Register(SetTranslation);
		Register(FindGameObjByName);
	}
}



﻿/*!***************************************************************************************
\file			script-wrappers.h
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			30/10/2022

\brief
	This file helps register static functions be used as internal calls in C#

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#include "Windows\windows-input.h"
#include "SceneManager\sm.h"
#include "GameObject/Components/physics-components.h"
#include <glm/vec3.hpp>
#include <Scripting/scripting-system.h>
#include <Messaging/message-system.h>
#include <Windows/windows-system.h>
#include <SceneManager/state-manager.h>
#include <cstring>

#include "mono/metadata/object.h"
#include "mono/metadata/reflection.h"

#ifndef SCRIPT_WRAPPERS_H
#define SCRIPT_WRAPPERS_H



namespace Copium
{
	#define Register(METHOD) mono_add_internal_call("CopiumEngine.InternalCalls::"#METHOD,METHOD)

	static std::unordered_map<MonoType*, ComponentType> s_EntityHasComponentFuncs;


	namespace
	{
		InputSystem& inputSystem{ *InputSystem::Instance() };
		NewSceneManager& sceneManager{ *NewSceneManager::Instance() };
		MessageSystem& messageSystem{ *MessageSystem::Instance() };
	}

	//static bool GetKeyDown(int keyCode)
	//{
	//	return inputSystem.is_key_pressed(keyCode);
	//}

	static bool GetKeyUp(int keyCode)
	{
		return false;
	}

	/*******************************************************************************
	/*!
	\brief
		Checks if a key was held
	\param keyCode
		Keycode to listen to
	\return
		True if key held
	*/
	/*******************************************************************************/
	static bool GetKey(int keyCode)
	{
		return inputSystem.is_key_held(keyCode);
	}

	/*******************************************************************************
	/*!
	\brief
		Gets the position of a GameObject via its id
	\param _ID
		_ID of gameObject to look for
	\param[out] translation
		Stores the value of translation of the queried gameObj
	*/
	/*******************************************************************************/
	static void GetTranslation(GameObjectID _ID, Math::Vec3* translation)
	{
		GameObject* gameObj = sceneManager.findGameObjByID(_ID);
		if (gameObj == nullptr)
		{
			return;
		}
		*translation = gameObj->transform.position;
	}

	/*******************************************************************************
	/*!
	\brief
		Sets the position of a GameObject via its id
	\param _ID
		_ID of gameObject to look for
	\param val
		Value of Vec3 to set
	*/
	/*******************************************************************************/
	static void SetTranslation(GameObjectID _ID, Math::Vec3* val)
	{
		GameObject* gameObj = sceneManager.findGameObjByID(_ID);
		if (gameObj == nullptr)
		{
			return;
		}
		gameObj->transform.position = *val;
	}

	/*******************************************************************************
	/*!
	\brief
		Finds GameObject via its name
	\param name
		Name of gameObject to look for
	\return
		ID of gameObject found by name
	*/
	/*******************************************************************************/
	static GameObjectID FindGameObjByName(MonoString* name)
	{
		char* nameCStr = mono_string_to_utf8(name);
		GameObject* gameObj = sceneManager.findGameObjByName(nameCStr);
		mono_free(nameCStr);
		if (gameObj == nullptr)
		{
			return GameObjectID(- 1);
		}
		return gameObj->id;
	}

	static void RigidbodyAddForce(GameObjectID _ID, Math::Vec2* force)
	{
		GameObject* gameObj = sceneManager.findGameObjByID(_ID);
		if (gameObj == nullptr)
		{
			return;
		}
		auto rb = gameObj->getComponent<Rigidbody2D>();
		if (rb == nullptr)
		{
			//LOG SOMETHING TO CONSOLE LIKE THIS OBJ HAS NOT RB
			return;
		}
		rb->add_force(*force);
	}

	static float GetDeltaTime()
	{
		return WindowsSystem::Instance()->get_delta_time();
	}

	static void RigidbodySetVelocity(GameObjectID _ID, Math::Vec2* velocity)
	{
		GameObject* gameObj = sceneManager.findGameObjByID(_ID);
		if (gameObj == nullptr)
		{
			return;
		}
		auto rb = gameObj->getComponent<Rigidbody2D>();
		if (rb == nullptr)
		{
			//LOG SOMETHING TO CONSOLE LIKE THIS OBJ HAS NOT RB
			return;
		}
		rb->set_vel(*velocity);
	}

	static void RigidbodyGetVelocity(GameObjectID _ID, Math::Vec2* velocity)
	{
		GameObject* gameObj = sceneManager.findGameObjByID(_ID);
		if (gameObj == nullptr)
		{
			return;
		}
		auto rb = gameObj->getComponent<Rigidbody2D>();
		if (rb == nullptr)
		{
			//LOG SOMETHING TO CONSOLE LIKE THIS OBJ HAS NOT RB
			return;
		}
		*velocity = rb->get_vel();
	}

	static bool HasComponent(GameObjectID _ID, MonoReflectionType* componentType)
	{
		GameObject* gameObj = sceneManager.findGameObjByID(_ID);
		if (gameObj == nullptr)
		{
			return false;
		}
		MonoType* managedType = mono_reflection_type_get_type(componentType);
		ComponentType cType = s_EntityHasComponentFuncs[managedType];
		PRINT(MAP_COMPONENT_TYPE_NAME[cType]);
		return gameObj->hasComponent(cType);
	}
	
	static void GetLocalScale(GameObjectID _ID, Math::Vec3* scale)
	{
		GameObject* gameObj = sceneManager.findGameObjByID(_ID);
		if (gameObj == nullptr)
		{
			return;
		}
		*scale = gameObj->transform.scale;
	}

	static void SetLocalScale(GameObjectID _ID, Math::Vec3* scale)
	{
		GameObject* gameObj = sceneManager.findGameObjByID(_ID);
		if (gameObj == nullptr)
		{
			return;
		}
		gameObj->transform.scale = *scale;
	}

	static void SetActive(GameObjectID _ID, bool _active)
	{
		GameObject* gameObj = sceneManager.findGameObjByID(_ID);
		if (gameObj == nullptr)
		{
			return;
		}
		gameObj->active = _active;
	}

	static bool GetActive(GameObjectID _ID)
	{
		GameObject* gameObj = sceneManager.findGameObjByID(_ID);
		if (gameObj == nullptr)
		{
			return false;
		}
		return gameObj->active;
	}

	static void QuitGame()
	{
		#ifdef GAMEMODE
		quit_engine();
		#else
		if (sceneManager.endPreview())
			messageSystem.dispatch(MESSAGE_TYPE::MT_STOP_PREVIEW);
		#endif
		//Scene manager quit
	}

	/*******************************************************************************
	/*!
	\brief
		Registers all defined internal calls with mono
	*/
	/*******************************************************************************/
	static void registerScriptWrappers()
	{
		Register(GetKey);
		Register(GetTranslation);
		Register(SetTranslation);
		Register(FindGameObjByName);
		Register(HasComponent);
		Register(RigidbodyAddForce);
		Register(RigidbodyGetVelocity);
		Register(RigidbodySetVelocity);
		Register(SetLocalScale);
		Register(GetLocalScale);
		Register(GetDeltaTime);
		Register(SetActive);
		Register(GetActive);
		Register(GetKeyUp);
		Register(QuitGame);
	}

	static void registerComponents()
	{
		s_EntityHasComponentFuncs.clear();
		int i{ 0 };
		int end{ (int)ComponentType::None };
		while (i != end)
		{
			std::string name = "CopiumEngine." + MAP_COMPONENT_TYPE_NAME[(ComponentType)i];
			MonoType* mType = ScriptingSystem::Instance()->getMonoTypeFromName(name);
			if (mType != nullptr)
			{
				s_EntityHasComponentFuncs.insert(std::make_pair(mType, (ComponentType)i));
			}
			++i;
		}
	}
}



#endif // !SCRIPT_WRAPPERS_H
/*!***************************************************************************************
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
#include "SceneManager\scene-manager.h"
#include "GameObject/Components/physics-components.h"
#include <glm/vec3.hpp>
#include <Scripting/scripting-system.h>
#include <Messaging/message-system.h>
#include <Windows/windows-system.h>
#include <Debugging/frame-rate-controller.h>
#include <SceneManager/state-manager.h>
#include <GameObject/Components/ui-components.h>
#include <cstring>

#include "mono/metadata/object.h"
#include "mono/metadata/reflection.h"

#ifndef SCRIPT_WRAPPERS_H
#define SCRIPT_WRAPPERS_H



namespace Copium
{
	#define Register(METHOD) mono_add_internal_call("CopiumEngine.InternalCalls::"#METHOD,METHOD)

	static std::unordered_map<std::string, ComponentType> s_EntityHasComponentFuncs;


	namespace
	{
		InputSystem& inputSystem{ *InputSystem::Instance() };
		SceneManager& sceneManager{ *SceneManager::Instance() };
		MessageSystem& messageSystem{ *MessageSystem::Instance() };
		ScriptingSystem& scriptingSystem{ *ScriptingSystem::Instance() };
	}

	//static bool GetKeyDown(int keyCode)
	//{
	//	return inputSystem.is_key_pressed(keyCode);
	//}

	static bool GetKeyUp(int keyCode)
	{
		return  inputSystem.is_key_pressed(keyCode);
	}

	static bool GetKeyDown(int keyCode)
	{
		return  inputSystem.is_key_pressed(keyCode);
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

	/*******************************************************************************
	/*!
	\brief
		Adds force to a gameobject with a rigidbody component
	\param _ID
		ID of gameObject to look for
	\param force
		Force to add
	*/
	/*******************************************************************************/
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

	/*******************************************************************************
	/*!
	\brief
		Gets the delta time from the engine
	\return
		Delta time
	*/
	/*******************************************************************************/
	static float GetDeltaTime()
	{
		return (float)MyFrameRateController.getDt();
	}

	/*******************************************************************************
	/*!
	\brief
		Sets the velocity of a rigidbody
	\param _ID
		GameObject of ID with a rigidbody
	\param velocity
		Velocity to set rigidbody to
	*/
	/*******************************************************************************/
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

	/*******************************************************************************
	/*!
	\brief
		Gets the velocity of a rigidbody
	\param _ID
		GameObject of ID with a rigidbody
	\param velocity
		Velocity to store rigidbody's velocity
	*/
	/*******************************************************************************/
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

	/*******************************************************************************
	/*!
	\brief
		Checks if a gameobject of given ID has a component
	\param _ID
		GameObject of ID to check for component
	\param componentType
		Type of component
	\return 
		True if gameobject of ID has component of type
	*/
	/*******************************************************************************/
	static bool HasComponent(GameObjectID _ID, MonoReflectionType* componentType)
	{
		GameObject* gameObj = sceneManager.findGameObjByID(_ID);
		if (gameObj == nullptr)
		{
			PRINT("CANT FIND GAMEOBJECT");
			return false;
		}
		MonoType* managedType = mono_reflection_type_get_type(componentType);
		ComponentType cType = s_EntityHasComponentFuncs[mono_type_get_name(managedType)];
		return gameObj->hasComponent(cType);
	}

	static ComponentID AddComponent(GameObjectID _ID, MonoReflectionType* componentType)
	{
		GameObject* gameObj = sceneManager.findGameObjByID(_ID);
		if (gameObj == nullptr)
		{
			PRINT("CANT FIND GAMEOBJECT");
			return false;
		}
		MonoType* managedType = mono_reflection_type_get_type(componentType);
		ComponentType cType = s_EntityHasComponentFuncs[mono_type_get_name(managedType)];
		return gameObj->addComponent(cType)->id;
	}
	
	/*******************************************************************************
	/*!
	\brief
		Checks if a gameobject of given ID has a component
	\param _ID
		GameObject of ID to check for component
	\param componentType
		Type of component
	\return
		True if gameobject of ID has component of type
	*/
	/*******************************************************************************/
	static void GetLocalScale(GameObjectID _ID, Math::Vec3* scale)
	{
		GameObject* gameObj = sceneManager.findGameObjByID(_ID);
		if (gameObj == nullptr)
		{
			return;
		}
		*scale = gameObj->transform.scale;
	}

	/*******************************************************************************
	/*!
	\brief
		Checks if a gameobject of given ID has a component
	\param _ID
		GameObject of ID to check for component
	\param componentType
		Type of component
	\return
		True if gameobject of ID has component of type
	*/
	/*******************************************************************************/
	static void SetLocalScale(GameObjectID _ID, Math::Vec3* scale)
	{
		GameObject* gameObj = sceneManager.findGameObjByID(_ID);
		if (gameObj == nullptr)
		{
			return;
		}
		gameObj->transform.scale = *scale;
	}

	/*******************************************************************************
	/*!
	\brief
		Set active a gameObject
	\param _ID
		GameObject of ID to set active
	\param _active
		Bool to set active to
	*/
	/*******************************************************************************/
	static void SetActive(GameObjectID _ID, bool _active)
	{
		GameObject* gameObj = sceneManager.findGameObjByID(_ID);
		if (gameObj == nullptr)
		{
			return;
		}
		gameObj->setActive(_active);
	}

	/*******************************************************************************
	/*!
	\brief
		Get active a gameObject
	\param _ID
		GameObject of ID to get active
	\return 
		Bool to whether gameobject was active
	*/
	/*******************************************************************************/
	static bool GetActive(GameObjectID _ID)
	{
		GameObject* gameObj = sceneManager.findGameObjByID(_ID);
		if (gameObj == nullptr)
		{
			return false;
		}
		return gameObj->isActive();
	}

	/*******************************************************************************
	/*!
	\brief
		Quits the game or preview mode depending on macros defined
	*/
	/*******************************************************************************/
	static void QuitGame()
	{
		if (sceneManager.endPreview())
			messageSystem.dispatch(MESSAGE_TYPE::MT_STOP_PREVIEW);
		//quit_engine();
		#ifdef GAMEMODE
		quit_engine();
		#else
		
		#endif
		//Scene manager quit
	}

	/*******************************************************************************
	/*!
	\brief
		Gets the string of a text component
	\param gameObjID
		ID of gameObject that has this component
	\param compID
		Component ID of text component
	\param str
		String of text component to store
	*/
	/*******************************************************************************/
	static void GetTextString(GameObjectID gameObjID, ComponentID compID, MonoString*& str)
	{
		GameObject* gameObj = sceneManager.findGameObjByID(gameObjID);
		if (gameObj == nullptr)
			return;
		for (Text* text : gameObj->getComponents<Text>())
		{
			if (text->id == compID)
			{
				PRINT("GETTING " << text->content);
				str = scriptingSystem.createMonoString(text->content);
				return;
			}
		}
	}

	/*******************************************************************************
	/*!
	\brief
		Sets the string of a text component
	\param gameObjID
		ID of gameObject that has this component
	\param compID
		Component ID of text component
	\param str
		String to set text component
	*/
	/*******************************************************************************/
	static void SetTextString(GameObjectID gameObjID, ComponentID compID, MonoString* str)
	{
		GameObject* gameObj = sceneManager.findGameObjByID(gameObjID);
		if (gameObj == nullptr)
			return;
		PRINT(gameObj->get_name() << ": ");
		for (Text* text : gameObj->getComponents<Text>())
		{
			if (text->id == compID)
			{
				char* monoStr = mono_string_to_utf8(str);
				PRINT(monoStr);
				strcpy(text->content, monoStr);
				mono_free(monoStr);
				break;
			}
		}
	}

	static char GetButtonState(GameObjectID gameObjID)
	{
		GameObject* gameObj = sceneManager.findGameObjByID(gameObjID);
		if (gameObj == nullptr)
			return 0;
		return (char)gameObj->getComponent<Button>()->GetState();
	}

	/*******************************************************************************
	/*!
	\brief
		Clones a gameObject
	\param ID
		ID of gameObject to be cloned
	\return
		GameObject ID of the cloned gameObject
	*/
	/*******************************************************************************/
	static GameObjectID CloneGameObject(GameObjectID ID)
	{
		GameObject* toBeCloned = MySceneManager.findGameObjByID(ID);
		if (toBeCloned)
		{
			GameObject* clone = MyGOF.clone(*toBeCloned);
			if (clone)
				return clone->id;
		}
		return 0;
	}

	/*******************************************************************************
	/*!
	\brief
		Creates a blank gameObject
	\return
		GameObject ID of the new gameObject
	*/
	/*******************************************************************************/
	static GameObjectID InstantiateGameObject()
	{
		GameObject* clone = MyGOF.instantiate();
		if (clone)
			return clone->id;
		return 0;
	}

	/*******************************************************************************
	/*!
	\brief
		Destroys a gameobject by ID
	\param ID
		GameObject ID of the gameObject to delete
	*/
	/*******************************************************************************/
	static void DestroyGameObject(GameObjectID ID)
	{
		COPIUM_ASSERT(!MyGOF.destroy(ID), "GameObject could not be destroyed");
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
		Register(GetKeyUp);
		Register(GetKeyDown);
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
		Register(GetTextString);
		Register(SetTextString);
		Register(CloneGameObject);
		Register(InstantiateGameObject);
		Register(DestroyGameObject);
		Register(QuitGame);
		Register(GetButtonState);
		Register(AddComponent);
	}

	/*******************************************************************************
	/*!
	\brief
		Registers component type names in mono to C++ defined component types
	*/
	/*******************************************************************************/
	static void registerComponents()
	{
		s_EntityHasComponentFuncs.clear();
		int i{ 0 };
		int end{ (int)ComponentType::None };
		PRINT("REGISTERING: ");
		while (i != end)
		{
			std::string name = "CopiumEngine." + MAP_COMPONENT_TYPE_NAME[(ComponentType)i];
			MonoType* mType = ScriptingSystem::Instance()->getMonoTypeFromName(name);
			if (mType != nullptr)
			{
				PRINT(name);
				s_EntityHasComponentFuncs.insert(std::make_pair(name, (ComponentType)i));
			}
			++i;
		}
	}
}



#endif // !SCRIPT_WRAPPERS_H
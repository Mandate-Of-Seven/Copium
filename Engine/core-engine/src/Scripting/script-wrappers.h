/*!***************************************************************************************
\file			script-wrappers.h
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			30/10/2022

\brief
	This file helps register static functions be used as internal calls in C#

All content � 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#include "Windows\windows-input.h"
#include "SceneManager\scene-manager.h"
#include <glm/vec3.hpp>
#include <Scripting/scripting-system.h>
#include <Messaging/message-system.h>
#include <Windows/windows-system.h>
#include <Debugging/frame-rate-controller.h>
#include <SceneManager/state-manager.h>
#include <cstring>
#include <Animation/animation-system.h>
#include <GameObject/components.h>

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
		AnimationSystem& animationSystem{ *AnimationSystem::Instance()};
	}

	//static bool GetKeyDown(int keyCode)
	//{
	//	return inputSystem.is_key_pressed(keyCode);
	//}

	/*******************************************************************************
	/*!
	\brief
		To be implemented

	\param keyCode

	\return
	*/
	/*******************************************************************************/
	static bool GetKeyUp(int keyCode)
	{
		return  inputSystem.is_key_pressed(keyCode);
	}
	/*******************************************************************************
	/*!
	\brief
		To be implemented

	\param keyCode

	\return

	*/
	/*******************************************************************************/
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
	static void GetTranslation(UUID _ID, Math::Vec3* translation)
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
	static void SetTranslation(UUID _ID, Math::Vec3* val)
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
	static UUID FindGameObjByName(MonoString* name)
	{
		char* nameCStr = mono_string_to_utf8(name);
		GameObject* gameObj = sceneManager.findGameObjByName(nameCStr);
		mono_free(nameCStr);
		if (gameObj == nullptr)
		{
			return UUID(- 1);
		}
		return gameObj->uuid;
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
	static void RigidbodyAddForce(UUID _ID, Math::Vec2* force)
	{
		GameObject* gameObj = sceneManager.findGameObjByID(_ID);
		if (gameObj == nullptr)
		{
			return;
		}
		auto rb = gameObj->GetComponent<Rigidbody2D>();
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
	static void RigidbodySetVelocity(UUID _ID, Math::Vec2* velocity)
	{
		GameObject* gameObj = sceneManager.findGameObjByID(_ID);
		if (gameObj == nullptr)
		{
			return;
		}
		auto rb = gameObj->GetComponent<Rigidbody2D>();
		if (rb == nullptr)
		{
			//LOG SOMETHING TO CONSOLE LIKE THIS OBJ HAS NOT RB
			return;
		}
		
		rb->velocity=*velocity;
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
	static void RigidbodyGetVelocity(UUID _ID, Math::Vec2* velocity)
	{
		GameObject* gameObj = sceneManager.findGameObjByID(_ID);
		if (gameObj == nullptr)
		{
			return;
		}
		auto rb = gameObj->GetComponent<Rigidbody2D>();
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
	static bool HasComponent(UUID UUID, MonoReflectionType* componentType)
	{
		GameObject* gameObj = sceneManager.findGameObjByID(UUID);
		if (gameObj == nullptr)
		{
			PRINT("CANT FIND GAMEOBJECT");
			return false;
		}
		MonoType* managedType = mono_reflection_type_get_type(componentType);
		ComponentType cType = s_EntityHasComponentFuncs[mono_type_get_name(managedType)];
		return gameObj->hasComponent(cType);
	}

	static UUID AddComponent(UUID UUID, MonoReflectionType* componentType)
	{
		GameObject* gameObj = sceneManager.findGameObjByID(UUID);
		if (gameObj == nullptr)
		{
			PRINT("CANT FIND GAMEOBJECT");
			return false;
		}
		MonoType* managedType = mono_reflection_type_get_type(componentType);
		ComponentType cType = s_EntityHasComponentFuncs[mono_type_get_name(managedType)];
		return gameObj->AddComponent(cType)->id;
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
	static void GetLocalScale(UUID _ID, Math::Vec3* scale)
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
	static void SetLocalScale(UUID _ID, Math::Vec3* scale)
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
	static void SetActive(UUID _ID, bool _active)
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
	static bool GetActive(UUID _ID)
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
		Get if component is enabled

	\param gid
		id of the game object to check

	\param cid
		id of the component to check

	\return
		whether the component is enabled
	*/
	/*******************************************************************************/
	static bool GetComponentEnabled(UUID gid, UUID cid)
	{
		Component* component = sceneManager.findComponentByID(cid);
		return component.enabled;
	}

	/*******************************************************************************
	/*!
	\brief
		Set the component's enabled value

	\param gid
		id of the game object to set

	\param cid
		id of the component to set

	\param val
		the value to set

	\return
		void
	*/
	/*******************************************************************************/
	static void SetComponentEnabled(UUID gid, UUID cid, bool val)
	{
		Component* component = sceneManager.findComponentByID(cid);
		if (component)
			component->Enabled(val);
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
		quit_engine();
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
	static void GetTextString(UUID gameObjID, UUID compID, MonoString*& str)
	{
		GameObject* gameObj = sceneManager.findGameObjByID(gameObjID);
		if (gameObj == nullptr)
			return;
		for (Text* text : gameObj->GetComponents<Text>())
		{
			if (text->uuid == compID)
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
	static void SetTextString(UUID gameObjID, UUID compID, MonoString* str)
	{
		Component* component = sceneManager.findComponentByID(compID);
		if (component == nullptr)
			return;
		char* monoStr = mono_string_to_utf8(str);
		PRINT("COMPONENT FOUND! : " << monoStr);
		strcpy(reinterpret_cast<Text*>(component)->content, monoStr);
		mono_free(monoStr);
	}

	/*******************************************************************************
	/*!
	\brief
		Get the button state

	\param gameObjID
		ID of gameObject that has this component

	\return
		the state of the button

	*/
	/*******************************************************************************/
	static char GetButtonState(UUID gameObjID)
	{
		GameObject* gameObj = sceneManager.findGameObjByID(gameObjID);
		if (gameObj == nullptr)
			return 0;
		return (char)gameObj->GetComponent<Button>()->GetState();
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
	static UUID CloneGameObject(UUID ID)
	{
		GameObject* toBeCloned = MySceneManager.findGameObjByID(ID);
		if (toBeCloned)
		{
			GameObject* clone = MyGOF.clone(*toBeCloned);
			PRINT("CLONED OBJECT: " << clone->uuid);
			if (clone)
				return clone->uuid;
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
	static UUID InstantiateGameObject()
	{
		GameObject* clone = MyGOF.instantiate();
		if (clone)
			return clone->uuid;
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
	static void DestroyGameObject(UUID ID)
	{
		COPIUM_ASSERT(!MyGOF.destroy(ID), "GameObject could not be destroyed");
	}


	static float GetFPS()
	{
		return MyFrameRateController.getFPS();
	}

	/*******************************************************************************
	/*!
	\brief
		Play sound

	\param ID
		id of the game object that contains an audio source component

	\return
		void 
	*/
	/*******************************************************************************/
	static void AudioSourcePlay(UUID ID)
	{
		GameObject* gameObj = sceneManager.findGameObjByID(ID);
		if (gameObj == nullptr)
			return;
		gameObj->GetComponent<AudioSource>()->play_sound();
	}

	/*******************************************************************************
	/*!
	\brief
		Pause all animations

	\return
		void
	*/
	/*******************************************************************************/
	static void PauseAllAnimation()
	{
		animationSystem.PauseAllAnimation();
	}

	/*******************************************************************************
	/*!
	\brief
		Play all animations

	\return
		void
	*/
	/*******************************************************************************/
	static void PlayAllAnimation()
	{
		animationSystem.PlayAllAnimation();
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
		Register(AudioSourcePlay);
		Register(PauseAllAnimation);
		Register(PlayAllAnimation);
		Register(GetComponentEnabled);
		Register(SetComponentEnabled);
		Register(GetFPS);
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
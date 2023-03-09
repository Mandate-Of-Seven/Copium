﻿/*!***************************************************************************************
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
#include <GameObject/game-object-factory.h>
#include <Editor/editor-system.h>

#include "mono/metadata/object.h"
#include "mono/metadata/reflection.h"

#ifndef SCRIPT_WRAPPERS_H
#define SCRIPT_WRAPPERS_H



namespace Copium
{
	#define Register(METHOD) mono_add_internal_call("CopiumEngine.InternalCalls::"#METHOD,METHOD)

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
		GameObject* gameObj = sceneManager.FindGameObjectByID(_ID);
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
		GameObject* gameObj = sceneManager.FindGameObjectByID(_ID);
		if (gameObj == nullptr)
		{
			return;
		}
		gameObj->transform.position = *val;
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
		GameObject* gameObj = sceneManager.FindGameObjectByID(_ID);
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
		rb->force += *force;
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
		Gets the delta time from the engine
	\return
		Delta time
	*/
	/*******************************************************************************/
	static void SetParent(UUID newParentID, UUID childID)
	{
		GameObject* child = sceneManager.FindGameObjectByID(childID);
		GameObject* parent = sceneManager.FindGameObjectByID(newParentID);
		if (parent)
			child->transform.SetParent(&parent->transform);
		else
			child->transform.SetParent(nullptr);
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
		GameObject* gameObj = sceneManager.FindGameObjectByID(_ID);
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
		GameObject* gameObj = sceneManager.FindGameObjectByID(_ID);
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
		*velocity = rb->velocity;
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
		GameObject* gameObj = sceneManager.FindGameObjectByID(UUID);
		if (gameObj == nullptr)
		{
			PRINT("CANT FIND GAMEOBJECT");
			return false;
		}
		MonoType* managedType = mono_reflection_type_get_type(componentType);
		mono_class_from_mono_type(managedType);
		const auto& pair = MyScriptingSystem.reflectionMap.find(managedType);
		if (pair != MyScriptingSystem.reflectionMap.end())
		{

		}
		else
		{

		}
		ComponentType cType = MyScriptingSystem.reflectionMap[managedType];
		return gameObj->HasComponent(cType);
	}

	static UUID AddComponent(UUID UUID, MonoReflectionType* componentType)
	{
		//GameObject* gameObj = sceneManager.FindGameObjectByID(UUID);
		//if (gameObj == nullptr)
		//{
		//	PRINT("CANT FIND GAMEOBJECT");
		//	return false;
		//}
		//MonoType* managedType = mono_reflection_type_get_type(componentType);
		//ComponentType cType = s_EntityHasComponentFuncs[mono_type_get_name(managedType)];
		//return gameObj->AddComponent(cType)->id;
		return 0;
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
		GameObject* gameObj = sceneManager.FindGameObjectByID(_ID);
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
		GameObject* gameObj = sceneManager.FindGameObjectByID(_ID);
		if (gameObj == nullptr)
		{
			return;
		}
		gameObj->transform.scale = *scale;
	}

	static void GetRotation(UUID _ID, Math::Vec3* rotation)
	{
		GameObject* gameObj = sceneManager.FindGameObjectByID(_ID);
		if (gameObj == nullptr)
		{
			return;
		}
		gameObj->transform.rotation = *rotation;
	}

	static void SetRotation(UUID _ID, Math::Vec3* rotation)
	{
		GameObject* gameObj = sceneManager.FindGameObjectByID(_ID);
		if (gameObj == nullptr)
		{
			return;
		}
		gameObj->transform.rotation = *rotation;
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
		GameObject* gameObj = sceneManager.FindGameObjectByID(_ID.GetUUID());
		//PRINT("SetActive: " << gameObj->name << " " << _active);
		if (gameObj == nullptr)
		{
			return;
		}
		gameObj->SetActive(_active);
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
		GameObject* gameObj = sceneManager.FindGameObjectByID(_ID);
		if (gameObj == nullptr)
		{
			return false;
		}
		return gameObj->active;
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
	static bool GetComponentEnabled(UUID cid, MonoReflectionType* componentType)
	{
		auto pair = scriptingSystem.reflectionMap.find(mono_reflection_type_get_type(componentType));
		if (pair == scriptingSystem.reflectionMap.end())
		{
			return false;
		}
		ComponentType cType = pair->second;
		Component* component{ nullptr };
		switch (cType)
		{
		case(ComponentType::Animator):
		{
			component = sceneManager.FindComponentByID<Animator>(cid);
			break;
		}
		case(ComponentType::AudioSource):
		{
			component = sceneManager.FindComponentByID<AudioSource>(cid);
			break;
		}
		case(ComponentType::BoxCollider2D):
		{
			component = sceneManager.FindComponentByID<BoxCollider2D>(cid);
			break;
		}
		case(ComponentType::Button):
		{
			component = sceneManager.FindComponentByID<Button>(cid);
			break;
		}
		case(ComponentType::Camera):
		{
			component = sceneManager.FindComponentByID<Camera>(cid);
			break;
		}
		case(ComponentType::Image):
		{
			component = sceneManager.FindComponentByID<Image>(cid);
			break;
		}
		case(ComponentType::Rigidbody2D):
		{
			component = sceneManager.FindComponentByID<Rigidbody2D>(cid);
			break;
		}
		case(ComponentType::SpriteRenderer):
		{
			component = sceneManager.FindComponentByID<SpriteRenderer>(cid);
			break;
		}
		case(ComponentType::Script):
		{
			//Different scripts
			component = sceneManager.FindComponentByID<Script>(cid);
			break;
		}
		case(ComponentType::Text):
		{
			component = sceneManager.FindComponentByID<Text>(cid);
			break;
		}
		case(ComponentType::SortingGroup):
		{
			component = sceneManager.FindComponentByID<SortingGroup>(cid);
			break;
		}
		}
		if (component)
			return component->enabled;
		return false;
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
	static void SetComponentEnabled(UUID cid, bool val, MonoReflectionType* componentType)
	{
		auto pair = scriptingSystem.reflectionMap.find(mono_reflection_type_get_type(componentType));
		if (pair == scriptingSystem.reflectionMap.end())
		{
			return;
		}
		ComponentType cType = pair->second;
		Component* component{ nullptr };
		switch (cType)
		{
		case(ComponentType::Animator):
		{
			component = sceneManager.FindComponentByID<Animator>(cid);
			break;
		}
		case(ComponentType::AudioSource):
		{
			component = sceneManager.FindComponentByID<AudioSource>(cid);
			break;
		}
		case(ComponentType::BoxCollider2D):
		{
			component = sceneManager.FindComponentByID<BoxCollider2D>(cid);
			break;
		}
		case(ComponentType::Button):
		{
			component = sceneManager.FindComponentByID<Button>(cid);
			break;
		}
		case(ComponentType::Camera):
		{
			component = sceneManager.FindComponentByID<Camera>(cid);
			break;
		}
		case(ComponentType::Image):
		{
			component = sceneManager.FindComponentByID<Image>(cid);
			break;
		}
		case(ComponentType::Rigidbody2D):
		{
			component = sceneManager.FindComponentByID<Rigidbody2D>(cid);
			break;
		}
		case(ComponentType::SpriteRenderer):
		{
			component = sceneManager.FindComponentByID<SpriteRenderer>(cid);
			break;
		}
		case(ComponentType::Script):
		{
			//Different scripts
			component = sceneManager.FindComponentByID<Script>(cid);
			break;
		}
		case(ComponentType::Text):
		{
			component = sceneManager.FindComponentByID<Text>(cid);
			break;
		}
		case(ComponentType::SortingGroup):
		{
			component = sceneManager.FindComponentByID<SortingGroup>(cid);
			break;
		}
		}
		if (component)
			component->enabled = val;
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
		GameObject* gameObj = sceneManager.FindGameObjectByID(gameObjID);
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
		Scene* pScene = sceneManager.get_current_scene();
		if (!pScene)
			return;
		for (Text& text : pScene->componentArrays.GetArray<Text>())
		{
			if (text.uuid == compID)
			{
				char* monoStr = mono_string_to_utf8(str);
				strcpy(text.content, monoStr);
				mono_free(monoStr);
				return;
			}
		}
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
	static char GetButtonState(UUID buttonID)
	{
		Scene* pScene = sceneManager.get_current_scene();
		for (Button& button : pScene->componentArrays.GetArray<Button>())
		{
			if (button.uuid == buttonID)
				return (char)button.state;
		}
		return 0;
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
		GameObject* toBeCloned = MySceneManager.FindGameObjectByID(ID);
		if (toBeCloned)
		{
			//GameObject* clone = MyGOF.(*toBeCloned);
			//PRINT("CLONED OBJECT: " << clone->uuid);
			//if (clone)
			//	return clone->uuid;
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
		Scene* scene = MySceneManager.get_current_scene();
		COPIUM_ASSERT(!scene, "SCENE NOT LOADED");
		GameObject& clone = MyGOF.Instantiate(*scene);
		return clone.uuid;
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
		MyGOF.Destroy(ID,MySceneManager.get_current_scene()->gameObjects);
	}

	/*******************************************************************************
	/*!
	\brief
		Destroys a gameobject by ID
	\param ID
		GameObject ID of the gameObject to delete
	*/
	/*******************************************************************************/
	static void LoadScene(MonoString* str)
	{
		static std::string name{};
		char* monoStr = mono_string_to_utf8(str);
		name = monoStr;
		name += ".scene";
		namespace fs = std::filesystem;
		for (const fs::directory_entry& p : fs::recursive_directory_iterator(Paths::projectPath))
		{
			const fs::path& pathRef{ p.path() };
			if (pathRef.extension() != ".scene")
				continue;
			if (pathRef.filename().string() == name)
			{
				PRINT("LOADING " << pathRef.string());
				MyEditorSystem.sceneChangeName = pathRef.string();
				return;
			}
		}
		PRINT("NO SCENE WITH THE NAME COULD BE FOUND");
	}


	static float GetFPS()
	{
		float tmp{ 0.f };

		if (MyFrameRateController.getFPS() > std::numeric_limits<float>::max())
			tmp = std::numeric_limits<float>::max();
		else
			tmp = (float)MyFrameRateController.getFPS();

		return tmp;
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
		GameObject* gameObj = sceneManager.FindGameObjectByID(ID);
		if (gameObj == nullptr)
			return;
		gameObj->GetComponent<AudioSource>()->play_sound();
	}

	static void AudioSourceStop(UUID ID)
	{
		GameObject* gameObj = sceneManager.FindGameObjectByID(ID);
		if (gameObj == nullptr)
			return;
		gameObj->GetComponent<AudioSource>()->stop_sound();
	}

	static void AudioSourceSetVolume(UUID ID, float volume)
	{
		GameObject* gameObj = sceneManager.FindGameObjectByID(ID);
		if (gameObj == nullptr)
			return;
		gameObj->GetComponent<AudioSource>()->volume = volume;
	}

	static float AudioSourceGetVolume(UUID ID)
	{
		GameObject* gameObj = sceneManager.FindGameObjectByID(ID);
		if (gameObj == nullptr)
			return 0;
		return gameObj->GetComponent<AudioSource>()->volume;
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

	static void GetTextColor(UUID ID, glm::vec4* color)
	{
		Scene* pScene = sceneManager.get_current_scene();
		if (!pScene)
			return;
		for (Text& text : pScene->componentArrays.GetArray<Text>())
		{
			if (text.uuid == ID)
			{
				color = &text.color;
				return;
			}
		}
	}

	static void SetTextColor(UUID ID, glm::vec4* color)
	{
		Scene* pScene = sceneManager.get_current_scene();
		if (!pScene)
			return;
		for (Text& text : pScene->componentArrays.GetArray<Text>())
		{
			if (text.uuid == ID)
			{
				text.color = *color;
				return;
			}
		}
	}

	static void GetSpriteRendererColor(UUID ID, glm::vec4* color)
	{
		GameObject* gameObj = sceneManager.FindGameObjectByID(ID);
		if (gameObj == nullptr)
			return;
		*color = gameObj->GetComponent<SpriteRenderer>()->sprite.color;
	}

	static void SetSpriteRendererColor(UUID ID, glm::vec4* color)
	{
		GameObject* gameObj = sceneManager.FindGameObjectByID(ID);
		if (gameObj == nullptr)
			return;
		gameObj->GetComponent<SpriteRenderer>()->sprite.color = *color;
	}

	static void GetImageColor(UUID ID, glm::vec4* color)
	{
		GameObject* gameObj = sceneManager.FindGameObjectByID(ID);
		if (gameObj == nullptr)
			return;
		*color = gameObj->GetComponent<Image>()->sprite.color;
	}

	static void SetImageColor(UUID ID, glm::vec4* color)
	{
		GameObject* gameObj = sceneManager.FindGameObjectByID(ID);
		if (gameObj == nullptr)
			return;
		gameObj->GetComponent<Image>()->sprite.color = *color;
	}

	static void PlayAnimation(UUID ID)
	{
		GameObject* gameObj = sceneManager.FindGameObjectByID(ID);
		if (gameObj == nullptr)
			return;
		gameObj->GetComponent<Animator>()->PlayAnimation();
	}

	static void PauseAnimation(UUID ID)
	{
		GameObject* gameObj = sceneManager.FindGameObjectByID(ID);
		if (gameObj == nullptr)
			return;
		gameObj->GetComponent<Animator>()->PauseAnimation();
	}

	static void SetAnimatorDelay(UUID componentID, float timeDelay)
	{
		Scene* pScene = sceneManager.get_current_scene();
		if (!pScene)
			return;
		for (Animator& animator : pScene->componentArrays.GetArray<Animator>())
		{
			if (animator.uuid == componentID)
			{
				animator.animations[0].timeDelay = timeDelay;
			}
		}
	}

	static float GetAnimatorDelay(UUID componentID)
	{
		Scene* pScene = sceneManager.get_current_scene();
		if (!pScene)
			return 0;
		for (Animator& animator : pScene->componentArrays.GetArray<Animator>())
		{
			if (animator.uuid == componentID)
			{
				return (float)animator.animations[0].timeDelay;
			}
		}

		return 0;
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
		Register(HasComponent);
		Register(RigidbodyAddForce);
		Register(RigidbodyGetVelocity);
		Register(RigidbodySetVelocity);
		Register(SetLocalScale);
		Register(GetLocalScale);
		Register(GetRotation);
		Register(SetRotation);
		Register(GetDeltaTime);
		Register(SetActive);
		Register(GetActive);
		Register(GetTextString);
		Register(SetTextString);
		Register(LoadScene);
		Register(CloneGameObject);
		Register(InstantiateGameObject);
		Register(DestroyGameObject);
		Register(QuitGame);
		Register(GetButtonState);
		Register(AddComponent);
		Register(AudioSourcePlay);
		Register(AudioSourceStop);
		Register(AudioSourceSetVolume);
		Register(AudioSourceGetVolume);
		Register(PauseAllAnimation);
		Register(PlayAllAnimation);
		Register(GetComponentEnabled);
		Register(SetComponentEnabled);
		Register(SetParent);
		Register(GetFPS);
		Register(GetSpriteRendererColor);
		Register(SetSpriteRendererColor);
		Register(GetImageColor);
		Register(SetImageColor);
		Register(GetTextColor);
		Register(SetTextColor);
		Register(PlayAnimation);
		Register(PauseAnimation);
		Register(SetAnimatorDelay);
		Register(GetAnimatorDelay);
	}
}
#endif // !SCRIPT_WRAPPERS_H
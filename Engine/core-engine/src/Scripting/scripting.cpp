/*!***************************************************************************************
\file			scripting.cpp
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			27/09/2022

\brief
	This file holds the definition of functions for scripting

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#include "pch.h"
#include "Scripting/scripting.h"
#include "GameObject/game-object.h"

#define DEFAULT_SCRIPT_NAME "NewScript"
#include <mono/jit/jit.h>
namespace Copium
{
	using namespace Scripting;
	ScriptingSystem& ScriptComponent::sS{ *ScriptingSystem::Instance() };

	ScriptComponent::ScriptComponent(GameObject& _gameObj) :
		mObject{ nullptr }, pScriptClass{ nullptr }, Component(_gameObj, Component::Type::Script), name{ DEFAULT_SCRIPT_NAME }
	{
		Message::MessageSystem::Instance()->subscribe(Message::MESSAGE_TYPE::MT_SCRIPTING_UPDATED, this);
		pScriptClass = sS.getScriptClass(name);
		instantiate();
	}

	ScriptComponent::~ScriptComponent()
	{
	}

	void ScriptComponent::instantiate()
	{
		if (pScriptClass != nullptr)
		{
			mObject = sS.instantiateClass(pScriptClass->mClass);
			GameObjectID id = gameObj.get_id();
			void* param = &id;
			//mono_object_init
			if (!pScriptClass->mOnCreate)
			{
				PRINT("ON CREATE DOES NOT EXIST");
			}
			sS.invoke(mObject, pScriptClass->mOnCreate, &param);
		}
	}

	void ScriptComponent::handleMessage(Message::MESSAGE_TYPE mType)
	{
		//MT_SCRIPTING_UPDATED
		pScriptClass = sS.getScriptClass(name.c_str());
		instantiate();
	}

	const std::string& ScriptComponent::Name() const
	{
		return name;
	}

	void ScriptComponent::Name(const std::string& _name)
	{
		name = _name;		
		pScriptClass = sS.getScriptClass(name);
		instantiate();
	}

	void ScriptComponent::Awake()
	{
		if (pScriptClass && pScriptClass->mAwake)
			sS.invoke(mObject, pScriptClass->mAwake);
	}

	void ScriptComponent::Start()
	{
		if (pScriptClass && pScriptClass->mStart)
			sS.invoke(mObject, pScriptClass->mStart);
	}

	void ScriptComponent::Update()
	{
		if (pScriptClass && pScriptClass->mUpdate)
			sS.invoke(mObject, pScriptClass->mUpdate);
	}

	void ScriptComponent::LateUpdate()
	{
		if (pScriptClass && pScriptClass->mLateUpdate)
			sS.invoke(mObject, pScriptClass->mLateUpdate);
	}

	void ScriptComponent::OnCollisionEnter()
	{
		if (pScriptClass && pScriptClass->mOnCollisionEnter)
			sS.invoke(mObject, pScriptClass->mOnCollisionEnter);
	}
}


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

#define DEFAULT_SCRIPT_NAME "NewScript"
#include <mono/jit/jit.h>

namespace Copium
{
	using namespace Scripting;
	ScriptingSystem& ScriptComponent::sS{ *ScriptingSystem::Instance() };

	ScriptComponent::ScriptComponent(const char* _name) :
		mObject{ nullptr }, spScriptClass{ nullptr }, name{ _name }, Component()
	{
		Message::MessageSystem::Instance()->subscribe(Message::MESSAGE_TYPE::MT_SCRIPTING_UPDATED, this);
		spScriptClass = sS.getScriptClass(_name);
		if (spScriptClass != nullptr)
		{
			mObject = sS.createMonoObject(spScriptClass.get()->mClass);
		}
	}

	ScriptComponent::ScriptComponent(MonoClass* _mClass) :
		name{ mono_class_get_name(_mClass) },
		spScriptClass{ sS.getScriptClass(name.c_str()) }
	{
		Message::MessageSystem::Instance()->subscribe(Message::MESSAGE_TYPE::MT_SCRIPTING_UPDATED, this);
		mObject = sS.createMonoObject(spScriptClass.get()->mClass);
	}

	ScriptComponent::~ScriptComponent()
	{
		PRINT("Script Component Destroyed");
	}

	void ScriptComponent::handleMessage(Message::MESSAGE_TYPE mType)
	{
		//MT_SCRIPTING_UPDATED
		spScriptClass = sS.getScriptClass(name.c_str());
		//If mono class couldnt be loaded
		if (spScriptClass == nullptr)
		{
			PRINT("Please delete " << name << " Instance as base script could not be found anymore!");
			return;
		}
		mObject = sS.createMonoObject(spScriptClass.get()->mClass);
	}

	void ScriptComponent::Awake()
	{
		if (spScriptClass && spScriptClass->mAwake)
			sS.invoke(mObject, spScriptClass->mAwake);
	}

	void ScriptComponent::Start()
	{
		if (spScriptClass && spScriptClass->mStart)
			mono_runtime_invoke(spScriptClass->mStart, mObject, nullptr, nullptr);
	}

	void ScriptComponent::Update()
	{
		if (spScriptClass && spScriptClass->mUpdate)
			mono_runtime_invoke(spScriptClass->mUpdate, mObject, nullptr, nullptr);
	}

	void ScriptComponent::LateUpdate()
	{
		if (spScriptClass && spScriptClass->mLateUpdate)
			mono_runtime_invoke(spScriptClass->mLateUpdate, mObject, nullptr, nullptr);
	}

	void ScriptComponent::OnCollisionEnter()
	{
		if (spScriptClass && spScriptClass->mOnCollisionEnter)
			mono_runtime_invoke(spScriptClass->mOnCollisionEnter, mObject, nullptr, nullptr);
	}
}


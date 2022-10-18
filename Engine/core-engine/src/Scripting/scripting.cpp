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
	}

	void ScriptComponent::handleMessage(Message::MESSAGE_TYPE mType)
	{
		//MT_SCRIPTING_UPDATED
		spScriptClass = sS.getScriptClass(name.c_str());
		//If mono class couldnt be loaded
		if (spScriptClass == nullptr)
			return;
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
			sS.invoke(mObject, spScriptClass->mStart);
	}

	void ScriptComponent::Update()
	{
		if (spScriptClass && spScriptClass->mUpdate)
			sS.invoke(mObject, spScriptClass->mUpdate);
	}

	void ScriptComponent::LateUpdate()
	{
		if (spScriptClass && spScriptClass->mLateUpdate)
			sS.invoke(mObject, spScriptClass->mLateUpdate);
	}

	void ScriptComponent::OnCollisionEnter()
	{
		if (spScriptClass && spScriptClass->mOnCollisionEnter)
			sS.invoke(mObject, spScriptClass->mOnCollisionEnter);
	}
}


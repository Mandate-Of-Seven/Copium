/*!***************************************************************************************
\file			scripting.cpp
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			27/09/2022

\brief
	This file holds the definition of functions for scripting

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#include "pch.h"
#include "scripting.h"
#include "file-system.h"

#define DEFAULT_SCRIPT_NAME "NewScript"
#include <mono/jit/jit.h>

namespace Copium
{
	using namespace Scripting;
	std::vector<ScriptComponent*> ScriptComponent::instances{};
	ScriptingSystem& ScriptComponent::sS{ *ScriptingSystem::Instance() };

	ScriptComponent::ScriptComponent(const char* _name) :
		mObject{ nullptr }, spScriptClass{ nullptr }, name{ _name }, Component()
	{
		Message::MessageSystem::Instance()->subscribe(Message::MESSAGE_TYPE::MT_SCRIPTING_UPDATED, this);
		spScriptClass = sS.getScriptClass(_name);
		if (spScriptClass == nullptr)
		{
			const std::string scriptPath = Files::Paths::projectPath + "\\" + _name + ".cs";
			std::ofstream scriptFile(scriptPath);
			scriptFile << "using CopiumEngine;\n\n";
			scriptFile << "public class " << _name << " : CopiumScript" << std::endl;
			scriptFile << "{\n\tvoid Start()\n\t{\n\n\t}\n\n\tvoid Update()\n\t{\n\n\t}\n}" << std::endl;
			scriptFile.close();
		}
		else
		{
			mObject = sS.createMonoObject(spScriptClass.get()->mClass);
		}
		instances.push_back(this);
	}

	ScriptComponent::ScriptComponent(MonoClass* _mClass) :
		name{ mono_class_get_name(_mClass) },
		spScriptClass{ sS.getScriptClass(name.c_str()) }
	{
		Message::MessageSystem::Instance()->subscribe(Message::MESSAGE_TYPE::MT_SCRIPTING_UPDATED, this);
		mObject = sS.createMonoObject(spScriptClass.get()->mClass);
		instances.push_back(this);
	}

	ScriptComponent::~ScriptComponent()
	{
		for (ScriptComponent* instance : instances)
		{
			if (instance == this)
			{
				instance = instances[instances.size()];
				instances.pop_back();
				break;
			}
		}
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
		mono_runtime_invoke(spScriptClass->mAwake, mObject, nullptr, nullptr);
	}

	void ScriptComponent::Start()
	{
		mono_runtime_invoke(spScriptClass->mStart, mObject, nullptr, nullptr);
	}

	void ScriptComponent::Update()
	{
		mono_runtime_invoke(spScriptClass->mUpdate, mObject, nullptr, nullptr);
	}

	void ScriptComponent::LateUpdate()
	{
		mono_runtime_invoke(spScriptClass->mLateUpdate, mObject, nullptr, nullptr);
	}

	void ScriptComponent::OnCollisionEnter()
	{
		mono_runtime_invoke(spScriptClass->mOnCollisionEnter, mObject, nullptr, nullptr);
	}
}


#include "pch.h"
#include "scripting.h"

#define DEFAULT_SCRIPT_NAME "NewScript"
#include <mono/jit/jit.h>

ScriptComponent::ScriptComponent(const std::string& _name)
	: name{ _name }, Component(Type::Script)
{

}

void ScriptComponent::refreshMethods()
{
	//monoClass = Copium::ScriptingEngine::loadMonoClass(name);
	//instance = Copium::ScriptingEngine::newScriptInstance(monoClass);
	csAwake = mono_class_get_method_from_name(monoClass, "Awake", 0);
	csStart = mono_class_get_method_from_name(monoClass, "Start", 0);
	csUpdate = mono_class_get_method_from_name(monoClass, "Update", 0);
	csLateUpdate = mono_class_get_method_from_name(monoClass, "LateUpdate", 0);
	csOnCollisionEnter = mono_class_get_method_from_name(monoClass, "OnCollisionEnter", 0);
	mono_runtime_object_init(instance);
}

const std::string& ScriptComponent::Name()
{
	return name;
}

void ScriptComponent::Name(const std::string& _name)
{
	name = _name;
}

void ScriptComponent::Awake()
{
	mono_runtime_invoke(csAwake, instance, nullptr, nullptr);
}

void ScriptComponent::Start()
{
	mono_runtime_invoke(csStart, instance, nullptr, nullptr);
}

void ScriptComponent::Update()
{
	mono_runtime_invoke(csUpdate, instance, nullptr, nullptr);
}

void ScriptComponent::LateUpdate()
{
	mono_runtime_invoke(csLateUpdate, instance, nullptr, nullptr);
}

void ScriptComponent::OnCollisionEnter()
{
	mono_runtime_invoke(csOnCollisionEnter, instance, nullptr, nullptr);
}
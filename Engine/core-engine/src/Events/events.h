#pragma once
#include <Scripting/scriptable-object.h>
#include <config.h>
#include <GameObject/Components/script-component.h>

namespace Copium
{

	struct IEvent
	{
	protected:
		virtual ~IEvent() {};
	};

	struct SceneOpenedEvent : IEvent
	{
		SceneOpenedEvent(const char* _sceneName) :sceneName{ _sceneName } {};
		const char* sceneName;
	};
	
	struct ReflectScriptEvent : IEvent
	{
		ReflectScriptEvent(const char* _scriptName, ComponentID _scriptID, GameObjectID _gameObjectID) :
			scriptName{ _scriptName }, scriptID{ _scriptID }, gameObjectID{ _gameObjectID } {};
		const char* scriptName;
		ComponentID scriptID;
		GameObjectID gameObjectID;
	};

	struct InvokeScriptMethodEvent : IEvent
	{
		InvokeScriptMethodEvent(Script& _script,const std::string& _methodName, void** _params = 0, size_t _paramCount = 0) :
			script{ _script }, methodName{ _methodName }, params{ _params }, paramCount{ paramCount }{}
		Script& script;
		const std::string& methodName;
		void** params;
		size_t paramCount;
	};

	struct ScriptSetFieldEvent : IEvent
	{
		ScriptSetFieldEvent(Script& _script, const std::string& _fieldName, void* _data) :
			script{ _script }, fieldName{ _fieldName }, data{ _data }{}
		Script& script;
		const std::string& fieldName;
		void* data;
	};

	struct ScriptGetFieldEvent : IEvent
	{
		ScriptGetFieldEvent(const Script& _script, const std::string& _fieldName, void* _container) :
			script{ _script }, fieldName{ _fieldName }, container{ _container }{}
		const Script& script;
		const std::string& fieldName;
		void* container;
	};
}
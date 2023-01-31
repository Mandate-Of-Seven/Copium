#pragma once
#include <Scripting/scriptable-object.h>
#include <config.h>

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
}
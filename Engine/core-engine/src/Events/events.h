/*!***************************************************************************************
****
\file			events.h
\project		GAM200
\author			Zacharie Hong

\par			Course: GAM200
\par			Section: A
\date			

\brief
	This file contains the declarations of all the various events in the event system

All content ? 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/


#ifndef EVENTS_H
#define EVENTS_H

#include <Scripting/scriptable-object.h>
#include <config.h>
#include <GameObject/components.h>
#include <GameObject/game-object.h>

namespace Copium
{
	class Component;
	class Script;

	struct IEvent
	{
	protected:
		virtual ~IEvent() {};
	};

	struct StartPreviewEvent : IEvent
	{
		StartPreviewEvent(){}
	};

	struct StopPreviewEvent : IEvent
	{
		StopPreviewEvent(){}
	};

	struct SceneOpenedEvent : IEvent
	{
		SceneOpenedEvent(const char* _sceneName, GameObjectsArray& _gameObjectsArray, ComponentsArrays& _componentsArrays) :
			sceneName{ _sceneName }, gameObjectsArray{ _gameObjectsArray }, componentsArrays{_componentsArrays}{}
		const char* sceneName;
		GameObjectsArray& gameObjectsArray;
		ComponentsArrays& componentsArrays;
	};

	struct GameObjectInstantiateEvent : IEvent
	{
		GameObjectInstantiateEvent(GameObject*& _instanceContainer, GameObject* _pOriginal = nullptr) : 
			instanceContainer{ _instanceContainer }, pOriginal{_pOriginal}{}
		GameObject*& instanceContainer;
		GameObject* pOriginal;
	};

	struct GameObjectDestroyEvent : IEvent
	{
		GameObjectDestroyEvent(GameObject& _gameObject) : gameObject{ _gameObject } {}
		GameObject& gameObject;
	};
	
	struct ReflectComponentEvent : IEvent
	{
		ReflectComponentEvent(Component& _component): component{_component}{}
		Component& component;
	};

	struct ScriptInvokeMethodEvent : IEvent
	{
		ScriptInvokeMethodEvent(Script& _script, const std::string& _methodName, void** _returnVal = nullptr, void** _params = nullptr, size_t _paramCount = 0) :
			script{ _script }, methodName{ _methodName }, params{ _params }, paramCount{ _paramCount }, returnVal{ _returnVal } {}
		Script& script;
		const std::string& methodName;
		void** params;
		size_t paramCount;
		void** returnVal;
	};

	struct ScriptSetFieldEvent : IEvent
	{
		ScriptSetFieldEvent(Script& _script, const char* _fieldName, void* _data) :
			script{ _script }, fieldName{ _fieldName }, data{ _data }{}
		Script& script;
		const char* fieldName;
		void* data;
	};

	struct ScriptGetFieldEvent : IEvent
	{
		ScriptGetFieldEvent(Script& _script, const char* _fieldName, void* _container) :
			script{ _script }, fieldName{ _fieldName }, container{ _container }{}
		Script& script;
		const char* fieldName;
		void* container;
	};


	template <typename T>
	struct ScriptSetFieldReferenceEvent : public IEvent
	{
		ScriptSetFieldReferenceEvent(Script& _script, const char* _fieldName, T* _reference) :
			script{ _script }, fieldName{ _fieldName }, reference{ _reference }{}
		Script& script;
		const char* fieldName;
		T* reference;
	};

	struct ScriptGetMethodNamesEvent : public IEvent
	{
		ScriptGetMethodNamesEvent(Script& _script, const char**& _namesArray, size_t& _arraySize) :
			script{ _script }, namesArray{ _namesArray }, arraySize{ _arraySize }{}
		Script& script;
		const char**& namesArray;
		size_t& arraySize;
	};

	struct ScriptCreatedEvent : public IEvent
	{
		ScriptCreatedEvent(Script& _script) :script{ _script } {}
		Script& script;
	};

	struct ScriptDestroyedEvent : public IEvent
	{
		ScriptDestroyedEvent(Script& _script) :script{ _script } {}
		Script& script;
	};

}
#endif // !EVENTS_H

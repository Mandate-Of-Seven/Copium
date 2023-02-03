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
		SceneOpenedEvent(const char* _sceneName) :sceneName{ _sceneName } {};
		const char* sceneName;
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
		static_assert(ScriptReferenceables::contains<T>());

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
}
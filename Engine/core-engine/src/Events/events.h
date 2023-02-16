/*!***************************************************************************************
\file			events.h
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			15/02/2023

\brief
	This file contains the declarations of all event structures in the engine.

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/
#ifndef EVENTS_H
#define EVENTS_H

#include <Scripting/scriptable-object.h>
#include <config.h>
#include <GameObject/Components/script-component.h>

#include "Files/file.h"
#include "Files/file-directory.h"

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

	struct QuitEngineEvent : IEvent
	{
		QuitEngineEvent() {};
	};

	struct SceneOpenedEvent : IEvent
	{
		SceneOpenedEvent(const char* _sceneName) :sceneName{ _sceneName } {};
		const char* sceneName;
	};

	struct FileAssetEvent : IEvent
	{
		FileAssetEvent(File* _file, int _type) : file{ _file }, type{ _type } {};
		File* file;
		int type;
	};

	struct SetSelectedFileEvent : IEvent
	{
		SetSelectedFileEvent(File* _file) : file{ _file } {};
		File* file;
	};

	struct SetSelectedDirectoryEvent : IEvent
	{
		SetSelectedDirectoryEvent(Directory* _dir) : directory{ _dir } {};
		Directory* directory;
	};

	struct DeleteFromBrowserEvent : IEvent
	{
		DeleteFromBrowserEvent() {};
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
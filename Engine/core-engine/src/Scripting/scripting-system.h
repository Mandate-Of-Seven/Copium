/*!***************************************************************************************
\file			scripting-system.h
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			27/09/2022

\brief
	This file holds the declaration of functions for scripting-system.cpp

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#ifndef SCRIPTING_SYSTEM_H
#define SCRIPTING_SYSTEM_H
#pragma once

#include "CopiumCore\system-interface.h"
#include "Messaging\message-system.h"
#include "Files\file-system.h"

#include <string>
#include <unordered_map>

extern "C"
{
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoClassField MonoClassField;
}

namespace Copium
{
	enum class FieldType
	{
		Float, Double,
		Bool, Char, Byte, Short, Int, Long,
		UByte, UShort, UInt, ULong,
		Vector2, Vector3, None
	};

	struct Field
	{
		FieldType type;
		std::string name;
		MonoClassField* classField;
	};

	// ScriptField + data storage
	struct FieldInstance
	{
		Field Field;

		FieldInstance()
		{
			memset(m_Buffer, 0, sizeof(m_Buffer));
		}

		template<typename T>
		T GetValue()
		{
			static_assert(sizeof(T) <= 16, "Type too large!");
			return *(T*)m_Buffer;
		}

		template<typename T>
		void SetValue(T value)
		{
			static_assert(sizeof(T) <= 16, "Type too large!");
			memcpy(m_Buffer, &value, sizeof(T));
		}
	private:
		uint8_t m_Buffer[16];
	};

	using FieldMap = std::unordered_map<std::string, FieldInstance>;

	static std::unordered_map<std::string, FieldType> fieldTypeMap =
	{
		{ "System.Single", FieldType::Float },
		{ "System.Double", FieldType::Double },
		{ "System.Boolean", FieldType::Bool },
		{ "System.Char", FieldType::Char },
		{ "System.Int16", FieldType::Short },
		{ "System.Int32", FieldType::Int },
		{ "System.Int64", FieldType::Long },
		{ "System.Byte", FieldType::Byte },
		{ "System.UInt16", FieldType::UShort },
		{ "System.UInt32", FieldType::UInt },
		{ "System.UInt64", FieldType::ULong },

		{ "CopiumEngine.Vector2", FieldType::Vector2 },
		{ "CopiumEngine.Vector3", FieldType::Vector3 }
	};


	struct ScriptClass
	{
		ScriptClass() = delete;
		/**************************************************************************/
		/*!
			\brief
				Stores a monoClass and retrieves all the default functions
			\param _mClass
				Class to load functions from
		*/
		/**************************************************************************/
		ScriptClass(const std::string& _name, MonoClass* _mClass);
		const		std::string name;
		MonoClass*	mClass;
		MonoMethod* mAwake;
		MonoMethod* mStart;
		MonoMethod* mUpdate;
		MonoMethod* mLateUpdate;
		MonoMethod* mOnCollisionEnter;
		MonoMethod* mOnCreate;
		std::map<std::string, Field> mFields;
	};

	CLASS_SYSTEM(ScriptingSystem), IReceiver
	{
	public:
		/**************************************************************************/
		/*!
			\brief
				Default Constructor
		*/
		/**************************************************************************/
		ScriptingSystem();

		/**************************************************************************/
		/*!
			\brief
				Called at the start of engine runtime, starts a thread to 
				start compilation and track change of files
		*/
		/**************************************************************************/
		void init();

		/**************************************************************************/
		/*!
			\brief
				Called every frame
		*/
		/**************************************************************************/
		void update();

		/**************************************************************************/
		/*!
			\brief
				Called at the engine exit
		*/
		/**************************************************************************/
		void exit();

		/**************************************************************************/
		/*!
			\brief
				Gets the shared pointer of a ScriptClass from its name
			\param _name
				Name of ScriptClass to get
			\return
				Shared pointer to a ScriptClass
		*/
		/**************************************************************************/
		ScriptClass* getScriptClass(const std::string & _name);

		/**************************************************************************/
		/*!
			\brief
				Creates an instance of a monoClass
			\param mClass
				Class to create instance or clone from
			\return
				Pointer to the clone
		*/
		/**************************************************************************/
		MonoObject* instantiateClass(MonoClass * mClass);



		/**************************************************************************/
		/*!
			\brief
				Initializes mono by creating a root domain
		*/
		/**************************************************************************/
		void initMono();

		/**************************************************************************/
		/*!
			\brief
				Cleans up mono and its domains
		*/
		/**************************************************************************/
		void shutdownMono();

		void invoke(MonoObject * mObj, MonoMethod * mMethod, void** params = nullptr);

		void handleMessage(MESSAGE_TYPE mType);

		void reflectGameObject(uint64_t _ID);

		void loadAssemblyClasses();

		MonoObject* cloneInstance(MonoObject* _instance);

		const std::unordered_map<std::string, ScriptClass*>& getScriptClassMap();
	private:
		void updateScriptClasses();

		/**************************************************************************/
		/*!
			\brief
				Checks for addition or deletion of script files and updates a list
				of loaded script files
		*/
		/**************************************************************************/
		void updateScriptFiles();
		/**************************************************************************/
		/*!
			\brief
				Checks for any modified script files recompiles if there were
				any modified script files
		*/
		/**************************************************************************/
		void tryRecompileDll();
		/**************************************************************************/
		/*!
			\brief
				Creates an appDomain so that a new assembly can be loaded
		*/
		/**************************************************************************/
		void createAppDomain();
		/**************************************************************************/
		/*!
			\brief
				Deletes an appDomain and any assemblies loaded in it
		*/
		/**************************************************************************/
		void unloadAppDomain();
		/**************************************************************************/
		/*!
			\brief
				Reloads an assembly by creating a new domain
		*/
		/**************************************************************************/
		void swapDll();
		/**************************************************************************/
		/*!
			\brief
				Work for thread to check for script files and recompile if needed
		*/
		/**************************************************************************/
		void recompileThreadWork();
		/**************************************************************************/
		/*!
			\brief
				Checks whether a script is loaded as a File
			\param filePath
				Filepath to check whether the file is loaded
		*/
		/**************************************************************************/
		bool scriptIsLoaded(const std::filesystem::path& filePath);
		std::unordered_map<std::string, ScriptClass*> scriptClassMap;
		std::list<File>& scriptFiles;
	};
}
#endif // !SCRIPTING_SYSTEM_H
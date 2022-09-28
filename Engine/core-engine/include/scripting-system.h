/*!***************************************************************************************
\file			scripting-system.h
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			27/09/2022

\brief
	This file holds the declaration of functions for scripting-system.cpp

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#ifndef SCRIPTING_SYSTEM_H
#define SCRIPTING_SYSTEM_H
#pragma once

#include <mono/metadata/assembly.h>
#include <mono/metadata/attrdefs.h>
#include <mono/jit/jit.h>
#include <thread>
#include <filesystem>
#include "system-interface.h"
#include "message-system.h"
#include "file-system.h"

namespace Copium::Scripting
{
	enum class Accessibility : unsigned char
	{
		None = 0,
		Private = 1,
		Internal = (1 << 1),
		Protected = (1 << 2),
		Public = (1 << 3)
	};

	enum class FieldType
	{
		None,
		Vector2,
		Vector3,
		Vector4,
		Float = MONO_TYPE_R4,
		Integer = MONO_TYPE_I4,
		UnsignedInteger = MONO_TYPE_U4,
		String = MONO_TYPE_STRING
	};

	/**************************************************************************/
	/*!
		\brief
			Retrieves the type of field from monoType
		\param mType
			MonoType of field
	*/
	/**************************************************************************/
	static FieldType GetType(MonoType* mType)
	{
		int typeCode = mono_type_get_type(mType); //MonoTypeEnum
		switch (typeCode)
		{
		case MONO_TYPE_VALUETYPE:
		{
			char* typeName = mono_type_get_name(mType);
			if (strcmp(typeName, "Vector2") == 0) { return FieldType::Vector2; }
			if (strcmp(typeName, "Vector3") == 0) { return FieldType::Vector3; }
			if (strcmp(typeName, "Vector4") == 0) { return FieldType::Vector4; }
		}
		case MONO_TYPE_CLASS:
			break;
		default:
			return FieldType(typeCode);
			break;
		}
		PRINT("No type found");
		return FieldType::None;
	}

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
		ScriptClass(MonoClass* _mClass);
		MonoClass* mClass;
		MonoMethod* mAwake;
		MonoMethod* mStart;
		MonoMethod* mUpdate;
		MonoMethod* mLateUpdate;
		MonoMethod* mOnCollisionEnter;
	};

	CLASS_SYSTEM(ScriptingSystem)
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
		std::shared_ptr<ScriptClass> getScriptClass(const char* _name);

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
		MonoObject* createMonoObject(MonoClass * mClass);

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
	private:
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
		std::vector<std::shared_ptr<ScriptClass>> scriptClasses;
		MonoDomain* mRootDomain{ nullptr };		//JIT RUNTIME DOMAIN
		MonoDomain* mAppDomain{ nullptr };		//APP DOMAIN
		MonoAssembly* mCoreAssembly{ nullptr };	//ASSEMBLY OF SCRIPTS.DLL
		MonoImage* mAssemblyImage{ nullptr };	//LOADED IMAGE OF SCRIPTS.DLL
		std::list<Files::File>& scriptFiles;
	};

	/**************************************************************************/
	/*!
		\brief
			Gets the accessibility of a monoField
		\param field
			Monofield to check
		\return
			Enum of MonoAccessibility
			
	*/
	/**************************************************************************/
	unsigned char GetFieldAccessibility(MonoClassField* field);
}
#endif // !SCRIPTING_SYSTEM_H
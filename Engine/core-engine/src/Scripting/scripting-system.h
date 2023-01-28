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
#include <Scripting/scriptable-object.h>

#include <string>
#include <unordered_map>

extern "C"
{
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoObject MonoObject;
	typedef struct _MonoClassField MonoClassField;
	typedef struct _MonoType MonoType;
	typedef struct _MonoString MonoString;
}

namespace Copium
{
	using FieldFlag = uint8_t;
	#define FieldFlagList	0b00000001

	enum class FieldType
	{
		Float, Double,
		Bool, Char, Short, Int, Long,
		UShort, UInt, ULong,
		Vector2, Vector3, GameObject, Component ,None
	};

	struct Field
	{
		FieldType type{};
		std::string typeName;
		MonoClassField* classField{nullptr};
		FieldFlag flags{0};
	};

	static std::unordered_map<std::string, FieldType> fieldTypeMap =
	{
		{ "System.Single",				FieldType::Float		},
		{ "System.Double",				FieldType::Double		},
		{ "System.Boolean",				FieldType::Bool			},
		{ "System.Char",				FieldType::Char			},
		{ "System.Int16",				FieldType::Short		},
		{ "System.Int32",				FieldType::Int			},
		{ "System.Int64",				FieldType::Long			},
		{ "System.UInt16",				FieldType::UShort		},
		{ "System.UInt32",				FieldType::UInt			},
		{ "System.UInt64",				FieldType::ULong		},
		{ "CopiumEngine.Vector2",		FieldType::Vector2		},
		{ "CopiumEngine.Vector3",		FieldType::Vector3		},
		{ "CopiumEngine.GameObject",	FieldType::GameObject	},
	};

	struct FieldData
	{
		/***************************************************************************/
		/*!
		\brief
			Stores data of a given buffer to prevent out of scope destruction.
			Aka assigns memory from the heap

		\param _size
			Size of buffer

		\param _data
			Data to store and copy from
		*/
		/**************************************************************************/
		FieldData(size_t _size = 0, void* _data = nullptr)
		{
			size = _size;
			if (size)
				data = new char[size];
			else
				data = nullptr;
			if (_data)
				memcpy(data, _data, size);
		}

		/***************************************************************************/
		/*!
		\brief
			Copy constructor

		\param rhs
			FieldData to store and copy from
		*/
		/**************************************************************************/
		FieldData(const FieldData& rhs)
		{
			size = rhs.size;
			data = new char[size];
			memcpy(data, rhs.data, size);
		}

		/***************************************************************************/
		/*!
		\brief
			Destructor that frees memory
		*/
		/**************************************************************************/
		~FieldData()
		{
			if (data)
				delete[] data;
		}
		char* data;
		size_t size;
	};

	enum class CompilingState
	{
		Compiling,
		SwapAssembly,
		Deserializing,
		Wait,
		Previewing,
	};

	struct ScriptClass
	{
		ScriptClass() = default;
		/**************************************************************************/
		/*!
			\brief
				Stores a monoClass and retrieves all the default functions
			\param _mClass
				Class to load functions from
		*/
		/**************************************************************************/
		ScriptClass(const std::string& _name, MonoClass* _mClass);
		MonoClass* mClass{};
		std::unordered_map<std::string, MonoMethod*> mMethods;
		std::unordered_map<std::string, Field> mFields;

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

		const std::list<Copium::File>& getScriptFiles();
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

		/**************************************************************************/
		/*!
			\brief
				Cleans up mono and its domains
			\param mObj
				Instance to invoke from
			\param mMethod
				Method to invoke
			\param params
				Parameters to pass into mono function
		*/
		/**************************************************************************/
		void invoke(MonoObject * mObj, MonoMethod * mMethod, void** params = nullptr);

		/**************************************************************************/
		/*!
			\brief
				Listens to MT_REFLECT_CS_GAMEOBJECT to know when to tell scripts
				to create their version of a gameObject
			\param mType
				Message type, used if this listens to two or more messages
		*/
		/**************************************************************************/
		void handleMessage(MESSAGE_TYPE mType);



		/**************************************************************************/
		/*!
			\brief
				Reflects a gameObject of ID into C#
			\param _ID
				ID of gameObject to be reflected
		*/
		/**************************************************************************/
		MonoObject* cloneInstance(MonoObject* _instance);

		/*******************************************************************************
		/*!
		\brief
			Creates an instance of a given class
		\param _mClass
			Class to make object from
		\return
			MonoObject of a class
		*/
		/*******************************************************************************/
		MonoObject* createInstance(MonoClass* _mClass);

		/**************************************************************************/
		/*!
			\brief
				Creates a new file
			\param _name
				Name of the new script
		*/
		/**************************************************************************/
		void addEmptyScript(const std::string& _name);


		/**************************************************************************/
		/*!
			\brief
				Gets the map of names to ScriptClasses
			\return 
				Map of names to ScriptClasses
		*/
		/**************************************************************************/
		const std::unordered_map<std::string, ScriptClass>& getScriptClassMap();


		/**************************************************************************/
		/*!
			\brief
				Gets the map of names to ScriptClasses
			\return
				Map of names to ScriptClasses
		*/
		/**************************************************************************/
		const std::unordered_map<std::string, ScriptClass>& getScriptableObjectClassMap();

		/*******************************************************************************
		/*!
		\brief
			Finds a type in C# mono using a string and returns the monoType
		\param name
			Name of type to find
		\return
			MonoType found
		*/
		/*******************************************************************************/
		MonoType* getMonoTypeFromName(std::string& name);

		/*******************************************************************************
		/*!
		\brief
			Converts a C string into a mono string
		\param str
			Str to convert into mono string
		\return
			Mono version of given string
		*/
		/*******************************************************************************/
		MonoString* createMonoString(const char* str);

		/**************************************************************************/
		/*!
			\brief
				Creates a collision data for scripts
			\param collided
				Rhs gameobject
			\param collidee
				Lhs gameObject
		*/
		/**************************************************************************/
		MonoObject* getFieldMonoObject(MonoClassField* mField, MonoObject* mObject);

		/**************************************************************************/
		/*!
			\brief
				Creates a collision data for scripts
			\param collided
				Rhs gameobject
			\param collidee
				Lhs gameObject
		*/
		/**************************************************************************/
		void instantiateCollision2D(GameObject& collided, GameObject& collidee);

		bool isScriptableObject(const std::string& name);


		bool isScript(const std::string& name);
	private:

		/**************************************************************************/
		/*!
			\brief
				Reloads and updates script classes
		*/
		/**************************************************************************/
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
		std::unordered_map<std::string, ScriptClass> scriptClassMap;
		std::unordered_map<std::string, ScriptClass> scriptableObjectClassMap;
		std::list<File>& scriptFiles;
		CompilingState compilingState{ CompilingState::Wait };
		std::map<std::string, std::map<std::string,ScriptableObject>> scriptableObjects;
	};
}
#endif // !SCRIPTING_SYSTEM_H
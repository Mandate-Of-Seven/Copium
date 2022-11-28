/*!***************************************************************************************
\file			script-component.h
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			27/09/2022

\brief
	This file holds the declaration of functions for scripting.cpp

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef SCRIPTING_H
#define SCRIPTING_H

#include "GameObject/Components/component.h"
#include "Messaging/message-system.h"
#include "Scripting/scripting-system.h"
#include <limits>
#include <Math/math-library.h>

extern "C"
{
	typedef struct _MonoClass MonoClass;
	typedef struct _MonoMethod MonoMethod;
	typedef struct _MonoObject MonoObject;
}

namespace Copium
{
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
				memcpy(data,_data,size);
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

    class Script final : public Component, public IReceiver
    {
    public:
		/**************************************************************************/
		/*!
			\brief
				Constructs a Script

			\param gameObj
				Owner of this
		*/
		/**************************************************************************/
		Script(GameObject& gameObj);
		/**************************************************************************/
		/*!
			\brief
				Deconstructor, removes itself from a list of instances
		*/
		/**************************************************************************/
		~Script();
		/**************************************************************************/
		/*!
			\brief
				IReceiver overload to handle a the MT_SCRIPTING_UPDATED message
			\param mType
				Type of message
		*/
		/**************************************************************************/
		void handleMessage(MESSAGE_TYPE mType);
		/***************************************************************************/
		/*!
		\brief
			Name function for scripts to return their scriptname as their
			name
		\return
			Const reference to the name of this component
		*/
		/**************************************************************************/
		const std::string& Name() const;
		/***************************************************************************/
		/*!
		\brief
			Name function for scripts to set a new name, instantiate a different
			kind of class in Mono
		\param _name
			Name to change to
		*/
		/**************************************************************************/
		void Name(const std::string& _name);
		/**************************************************************************/
		/*!
			\brief
				Function called at the very start of game object.
		*/
		/**************************************************************************/
		void invoke(const std::string& methodName);

		/*******************************************************************************
		/*!
		*
		\brief
			Displays the inspector view with its fields

		*/
		/*******************************************************************************/
		void inspector_view();

		
		/*******************************************************************************
		/*!
		*
		\brief
			Gets a field from a C# field using its name
		\param name
			Name of the field
		\param buffer
			Buffer to store the values, needs to be type casted
		\return
			False if operation failed, true if it was successful
		*/
		/*******************************************************************************/
		bool getFieldValue(const std::string& name, char* buffer) const;

		/*******************************************************************************
		/*!
		*
		\brief
			Sets a field from a C# field using its name
		\param name
			Name of the field
		\param value
			Value to write into C# memory space
		\return
			False if operation failed, true if it was successful
		*/
		/*******************************************************************************/
		bool setFieldValue(const std::string& name, const char* value);

		/***************************************************************************/
		/*!
		\brief
			Clone function for preview mode and editor mode
		\param _gameObj
			GameObject to clone from
		\return
			Reference to the cloned component in current scene
		*/
		/**************************************************************************/
		Component* clone(GameObject& _gameObj) const;

		/***************************************************************************/
		/*!
		\brief
			Deserializes this component's data from a rapidjson value.

		\param _value
			reference to the rapidjson value that contains the data of the component

		\return
			void
		*/
		/**************************************************************************/
		void deserialize(rapidjson::Value& _value);

		/***************************************************************************/
		/*!
		\brief
			Links references to other gameObjects or components on deserialization
		\param _value
			Json value to get ID of references
		*/
		/**************************************************************************/
		void deserializeLink(rapidjson::Value& _value);

		/***************************************************************************/
		/*!
		\brief
			Links references to other gameObjects or components on serialization
		\param rhs
			Reference component to know which gameObject to assign itself to
		*/
		/**************************************************************************/
		void previewLink(Component* rhs);

		/***************************************************************************/
		/*!
		\brief
			Serializes this component's data to a rapidjson value.

		\param _value
			reference to the rapidjson value that will contain the data of the component

		\return
			void
		*/
		/**************************************************************************/
		void serialize(rapidjson::Value& _value, rapidjson::Document& _doc);

		/***************************************************************************/
		/*!
		\brief
			Gets the function names in this script
		\return
			Vector of strings of functions in this script
		*/
		/**************************************************************************/
		const std::vector<std::string>& getFunctionNames();

		ScriptClass* pScriptClass;

		MonoObject* mObject;

		friend class ScriptingSystem;
	private:
		void instantiate();
		static char buffer[128];
		std::string name;
		const Script* reference{ nullptr };
		std::unordered_map<std::string, GameObject*> fieldGameObjReferences;
		std::unordered_map<std::string, Component*> fieldComponentReferences;
		std::unordered_map<std::string, FieldData> fieldDataReferences;
		static ScriptingSystem& sS;
		bool isAddingGameObjectReference;
    };
}

#endif // !SCRIPTING_H
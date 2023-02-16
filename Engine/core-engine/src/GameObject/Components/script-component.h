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
#include <limits>
#include <Math/math-library.h>
#include <config.h>

enum class FieldType
{
	Float, Double,
	Bool, Char, Short, Int, Long,
	UShort, UInt, ULong, String,
	Vector2, Vector3, GameObject, Component, None
};

namespace Copium
{
	struct Field
	{
		Field() = default;
		char* data{nullptr};
		FieldType fType{};
		std::string typeName;
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
		Field(FieldType _fType, size_t _size = 0, void* _data = nullptr) :
			fType{ _fType }
		{
			size = _size;
			if (size)
				data = new char[size];
			else
				data = nullptr;
			if (_data)
				memcpy(data, _data, size);
		}

		template<typename T>
		void operator=(const T& val)
		{
			COPIUM_ASSERT(sizeof(T) > size, "FIELD DOES NOT HAVE ENOUGH SPACE TO STORE TYPE");
			memcpy(data, &val, sizeof(T));
		}

		template<typename T>
		void operator=(const T* val)
		{
			memcpy(data, val, size);
		}

		template<typename T>
		T& Get()
		{
			COPIUM_ASSERT(sizeof(T) > size, "FIELD DOES NOT HAVE ENOUGH SPACE TO STORE TYPE");
			return *reinterpret_cast<T*>(data);
		}

		void Resize(size_t _size)
		{
			if (data)
				delete data;
			size = _size;
			data = new char[size];
		}

		/***************************************************************************/
		/*!
		\brief
			Copy constructor

		\param rhs
			Field to store and copy from
		*/
		/**************************************************************************/
		Field(const Field& rhs)
		{
			size = rhs.size;
			data = new char[size];
			fType = rhs.fType;
			typeName = rhs.typeName;
			memcpy(data, rhs.data, size);
		}

		Field(Field&& rhs)
		{
			size = rhs.size;
			data = rhs.data;
			fType = rhs.fType;
			typeName = std::move(rhs.typeName);
			rhs.data = nullptr;
		}

		Field& operator=(Field&& rhs)
		{
			size = rhs.size;
			data = rhs.data;
			fType = rhs.fType;
			typeName = std::move(rhs.typeName);
			rhs.data = nullptr;
			return *this;
		}

		/***************************************************************************/
		/*!
		\brief
			Destructor that frees memory
		*/
		/**************************************************************************/
		~Field()
		{
			if (data)
				delete[] data;
		}

		size_t GetSize() const
		{
			return size;
		}

	private:
		size_t size{ 0 };
	};

	using ScriptReferenceables = TemplatePack<GameObject, Component>;


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
		void GetFieldValue(const std::string& name, char* buffer);

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
		void SetFieldValue(const std::string& name, const char* value);

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
		Component* clone(GameObject& _gameObj, ComponentID* newID = nullptr) const;

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

		std::unordered_map<std::string, Component*>& GetFieldComponentReferences() { return fieldComponentReferences; }
		std::unordered_map<std::string, GameObject*>& GetFieldGameObjReferences() { return fieldGameObjReferences; }
		std::unordered_map<std::string, Field>& GetFieldData() { return fieldDataReferences; }
		static char* GetBuffer() { return buffer; }


		friend class ScriptingSystem;
	private:
		void instantiate();
		static char buffer[128];
		std::string name;
		std::unordered_map<std::string, GameObject*> fieldGameObjReferences;
		std::unordered_map<std::string, Component*> fieldComponentReferences;
		std::unordered_map<std::string, Field> fieldDataReferences;
		static std::pair<const std::string,Field>* editedField;
		static bool isAddingReference;
    };
}

#endif // !SCRIPTING_H
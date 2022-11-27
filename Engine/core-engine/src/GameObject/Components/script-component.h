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
		template <typename T>
		FieldData(const T& _data)
		{
			data = new char[sizeof(T)];
			memcpy(data,&_data, sizeof(T));
		}
		char* data;
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

		const std::string& Name() const;

		void Name(const std::string& _name);
		/**************************************************************************/
		/*!
			\brief
				Function called at the very start of game object.
		*/
		/**************************************************************************/
		void invoke(const std::string& methodName);
		/**************************************************************************/
		/*!
			\brief
				Function called right after Awake().
		*/
		/**************************************************************************/
		void Start();
		/**************************************************************************/
		/*!
			\brief
				Function called every frame.
		*/
		/**************************************************************************/
		void Update();
		/**************************************************************************/
		/*!
			\brief
				Function called every frame after Update().
		*/
		/**************************************************************************/
		void FixedUpdate();
		void LateUpdate();
		/**************************************************************************/
		/*!
			\brief
				Function called when object collides with something.
		*/
		/**************************************************************************/
		void OnCollisionEnter();

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
		bool getFieldValue(const std::string& name, char* buffer);

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

		Component* clone(GameObject& _gameObj) const;

		void deserialize(rapidjson::Value& _value);

		void deserializeLink(rapidjson::Value& _value);

		void previewLink(Component* rhs);

		void serialize(rapidjson::Value& _value, rapidjson::Document& _doc);

		const std::vector<std::string>& getFunctionNames();

		friend class ScriptingSystem;
	private:
		void instantiate();
		static char buffer[128];
		ScriptClass* pScriptClass;
		MonoObject* mObject;
		std::string name;
		const Script* reference{ nullptr };
		std::unordered_map<std::string, GameObject*> fieldGameObjReferences;
		std::unordered_map<std::string, Component*> fieldComponentReferences;
		std::unordered_map<std::string, FieldData*> fieldDataReferences;
		static ScriptingSystem& sS;
		bool isAddingGameObjectReference;
    };
}

#endif // !SCRIPTING_H
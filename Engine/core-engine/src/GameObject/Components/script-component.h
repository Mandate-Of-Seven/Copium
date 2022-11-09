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
    class ScriptComponent final : public Component, public IReceiver
    {
    public:
		/**************************************************************************/
		/*!
			\brief
				Constructs a ScriptComponent

			\param gameObj
				Owner of this
		*/
		/**************************************************************************/
		ScriptComponent(GameObject& gameObj);
		/**************************************************************************/
		/*!
			\brief
				Deconstructor, removes itself from a list of instances
		*/
		/**************************************************************************/
		~ScriptComponent();
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
		void Awake();
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
		bool getFieldValue(const std::string& name, void* buffer);

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
		bool setFieldValue(const std::string& name, const void* value);

		/*******************************************************************************
		/*!
		*
		\brief
			Deep copies a ScriptComponent into another
		\param rhs
			Reference to another ScriptComponent
		\return
			Reference to this ScriptComponent
		*/
		/*******************************************************************************/
		ScriptComponent& operator=(const ScriptComponent& rhs);

		void deserialize(rapidjson::Value& _value);

		void serialize(rapidjson::Value& _value, rapidjson::Document& _doc);

	private:
		void instantiate();
		char buffer[32];
		ScriptClass* pScriptClass;
		MonoObject* mObject;
		MonoObject* mGameObject;
		std::string name;
		const ScriptComponent* reference;
		static ScriptingSystem& sS;
    };
}

#endif // !SCRIPTING_H
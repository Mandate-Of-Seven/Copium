/*!***************************************************************************************
\file			scripting.h
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

#include "GameObject/component.h"
#include "Messaging/message-system.h"
#include "Scripting/scripting-system.h"

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
				Constructs a ScriptComponent based on the scripts name

			\param _name
				Name of script to make an instance of
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

		void inspector_view();

		bool getFieldValue(const std::string& name, void* buffer);
		bool setFieldValue(const std::string& name, const void* value);

	private:
		void instantiate();

		ScriptClass* pScriptClass;
		MonoObject* mObject;
		MonoObject* mGameObject;
		std::string name;
		static ScriptingSystem& sS;
    };
}

#endif // !SCRIPTING_H
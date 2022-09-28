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
#include <mono/metadata/assembly.h>

namespace Copium
{
    class ScriptComponent final : public Component, public Message::IReceiver
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
		ScriptComponent(const char* _name);
		/**************************************************************************/
		/*!
			\brief
				Constructs a ScriptComponent based on a monoclass

			\param _mClass
				Monoclass of script to make an instance of
		*/
		/**************************************************************************/
		ScriptComponent(MonoClass* _mClass);
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
		void handleMessage(Message::MESSAGE_TYPE mType);
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
	private:
		std::shared_ptr<Scripting::ScriptClass> spScriptClass;
		MonoObject* mObject;
		const std::string name;
		static std::vector<ScriptComponent*> instances;
		static Scripting::ScriptingSystem& sS;
    };
}

#endif // !SCRIPTING_H
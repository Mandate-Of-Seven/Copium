/*!***************************************************************************************
\file			renderer-component.cpp
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			21/09/2022

\brief
	This file holds the declaration of the Renderer component class.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"
#include "GameObject/renderer-component.h"

namespace Copium
{
	RendererComponent::RendererComponent(GameObject& _gameObj) :Component(_gameObj,Type::Renderer)
	{
		PRINT("\tRenderer Component constructed");
	}

	void RendererComponent::deserialize(rapidjson::Value& _value)
	{
		spriteRenderer.deserialize(_value);
	}
}
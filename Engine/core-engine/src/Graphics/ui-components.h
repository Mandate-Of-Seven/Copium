#pragma once

#include "GameObject/component.h"

namespace Copium
{
	//Runs after InputSystem
	class UIComponent : public Component
	{

	};

	class UIButtonComponent : public UIComponent
	{
		//A screen space collider
	};

	class UITextComponent : public UIComponent
	{
		//Display a text
	};

	class UIImageComponent : public UIComponent
	{
		//Display an image
	};
}
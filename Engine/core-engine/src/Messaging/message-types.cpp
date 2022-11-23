/*!***************************************************************************************
\file			message-types.cpp
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			30/10/2022

\brief
	This file declares the static vars for the Message Containers

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#include "Messaging/message-types.h"

namespace Copium::MESSAGE_CONTAINER
{
	REFLECT_CS_GAMEOBJECT reflectCsGameObject;
	ADD_OR_DELETE_GAMEOBJECT addOrDeleteGameObject;
	ADD_OR_DELETE_COMPONENT addOrDeleteComponent;
}
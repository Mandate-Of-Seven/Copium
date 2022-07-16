/*!***************************************************************************************
****
\file			game-object.h		
\project		
\author			Zacharie Hong
\co-authors		// DELETE IF NOT APPLICABLE

\par			Course: GAM200
\par			Section: 
\date			16/07/2022

\brief
	// BRIEF DESCRIPTION OF FILE PURPOSE

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************
****/

#pragma once

#ifndef HEADER_GAME_OBJECT

#define HEADER_GAME_OBJECT

//INCLUDES

#include <list>
#include <string>

#include "component.h"
#include "transform.h"

//USING

using GameObjectID = unsigned long;

class GameObject
{
private:
    GameObjectID id;
    std::list<Component*> components;
    std::string name;
    Transform trans;
public:
    GameObject();
    GameObject(Vector2 _position, Vector2 _rotation, Vector2 _scale);

    void addComponent(Component* component);

    void deleteComponent(Component* component);

    void Trans(Transform _trans);

    Transform const Trans();

    ~GameObject();
};



#endif // !HEADER_GAME_OBJECT

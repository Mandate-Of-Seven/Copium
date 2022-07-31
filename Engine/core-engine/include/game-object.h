/*!***************************************************************************************
\file			game-object.h
\project        
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			17/07/2022

\brief
    Declares GameObject Class and member functions

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#pragma once

#ifndef GAME_OBJECT_H

#define GAME_OBJECT_H

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
    GameObjectID id;                    //Global ID for gameObjects
    std::list<Component*> components;   //Components for gameObject
    std::string name;                   //Name of gameObject
    Transform trans;                    //Transform of gameObject
public:
    GameObject& operator=(GameObject&) = delete;

    /***************************************************************************/
    /*!
    \brief
        Default constructor, initializes Transform, name, id
    */
    /**************************************************************************/
    GameObject();

    /***************************************************************************/
    /*!
    \brief
        Constructor that initializes Transform with given values
    \param _position
        Position of transform to initialize with
    \param _rotation
        Rotation of transform to initialize with
    \param _scale
        Scale of transform to initialize with
    */
    /**************************************************************************/
    GameObject(Vector3 _position, Vector3 _rotation, Vector3 _scale);

    /***************************************************************************/
    /*!
    \brief
        Appends a new component to components list
    \param component
        Pointer to component to append to components list
    */
    /**************************************************************************/
    void addComponent(Component* component);

    /***************************************************************************/
    /*!
    \brief
        Appends a new component by taking in component type to components list
    \param componentType
        Type of component to append to components list
    */
    /**************************************************************************/
    void addComponent(Component::Type componentType);

    /***************************************************************************/
    /*!
    \brief
        Deletes a component from components list
    \param component
        Pointer to component to delete from components list
    */
    /**************************************************************************/
    void deleteComponent(Component* component);

    std::list<Component*>& Components();

    /***************************************************************************/
    /*!
    \brief
        Setter for gameObject Transform
    \param _trans
        Transform to copy values from into gameObject
    */
    /**************************************************************************/
    void Trans(Transform _trans);

    /***************************************************************************/
    /*!
    \brief
        Getter for gameObject Transform
    \return
        Return a copy transform of gameObject
    */
    /**************************************************************************/
    Transform const Trans();

    /***************************************************************************/
    /*!
    \brief
        Destructor, does nothing
    */
    /**************************************************************************/
    ~GameObject();
};



#endif // !GAME_OBJECT_H

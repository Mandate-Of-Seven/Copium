/*!***************************************************************************************
\file			game-object.h
\project        
\author			Zacharie Hong
\co-authors     Matthew Lau

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

#include "GameObject/component.h"
#include "Math/transform.h"
#include <rapidjson/document.h>

//USING

using GameObjectID = unsigned long;

class GameObject
{
private:
    GameObjectID id;                    //Global ID for gameObjects
    GameObjectID parentid;
    std::list<Component*> components;   //Components for gameObject
    std::string name;                   //Name of gameObject
    Transform trans;                    //Transform of gameObject
    GameObject* parent;                 //Pointer to this gameObject's parent
    std::list<GameObject*> children;    //List of pointers to this gameObject's children

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
    GameObject(Copium::Math::Vec3 _position, Copium::Math::Vec3 _rotation, Copium::Math::Vec3 _scale);

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

    /***************************************************************************/
    /*!
    \brief
        Gets a reference to the components list of this game object

    \return 
        reference to this gameobject's component list

    */
    /**************************************************************************/
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
    Transform Trans() const;

    /*******************************************************************************
    /*!
    *
    \brief
        Sets the name of the GameObject

    \param	_name
        string containing the GameObject's new name

    \return
        void
    */
    /*******************************************************************************/
    void set_name(const std::string& _name);
    /*******************************************************************************
    /*!
    *
    \brief
        Gets the GameObject's name

    \return
        reference to a string that contains the name of the GameObject
    */
    /*******************************************************************************/
    std::string get_name() const;

    /*******************************************************************************
    /*!
    *
    \brief
        Sets the ID of the GameObject

    \param	_id
        the new ID to be set

    \return
        void
    */
    /*******************************************************************************/
    void set_id(GameObjectID& _id);
    /*******************************************************************************
    /*!
    *
    \brief
        Gets the ID of the GameObject

    \return
        the ID of the GameObject
    */
    /*******************************************************************************/
    GameObjectID get_id() const;

    /*******************************************************************************
    /*!
    *
    \brief
        Check if the game object is a parent

    \return
        if gameobject is a parent, return true
        if gameobject is not a parent, return false
    */
    /*******************************************************************************/
    bool is_parent() const;

    /*******************************************************************************
    /*!
    *
    \brief
        Check if the game object has a parent

    \return
        if gameobject has a parent, return true
        if gameobject does not have a parent, return false
    */
    /*******************************************************************************/
    bool has_parent() const;

    /*******************************************************************************
    /*!
    *
    \brief
        Accessor for this game object's parent (if any)

    \return
        pointer to the parent game object
        if this game object does not have a parent, returns nullptr
    */
    /*******************************************************************************/
    GameObject* get_parent();
    /*******************************************************************************
    /*!
    *
    \brief
        Sets the parent of this gameobject

    \param _parent
         ptr to the gameobject which will become the parent of this game object

    \return
        void
    */
    /*******************************************************************************/
    void set_parent(GameObject* _parent);
    /*******************************************************************************
    /*!
    *
    \brief
        Returns a list that contains pointers to this game object's children (read-only)

    \return
        reference to the list containing the pointers to this game object's children
    */
    /*******************************************************************************/
    const std::list<GameObject*>& childList() const;

    /*******************************************************************************
    /*!
    *
    \brief
        Returns a list that contains pointers to this game object's children

    \return
        reference to the list containing the pointers to this game object's children
    */
    /*******************************************************************************/
    std::list<GameObject*>& mchildList();

    /*******************************************************************************
    /*!
    *
    \brief
        Attach a child to this game object

    \return
        if specified child game object is valid, return true
        if specified child game object is not valid, return false
    */
    /*******************************************************************************/
    bool attach_child(GameObject* _child);

    /*******************************************************************************
    /*!
    *
    \brief
        Detach a child from this game object. Removes specified child from this game object's childlist.
        Note: if specified game object is not a child of this game object, returns false

    \return
        if specified child game object is successfully detached, return true
        if specified child game object is not a child of this game object, return false
    */
    /*******************************************************************************/
    bool deattach_child(GameObject* _child);

    /*******************************************************************************
    /*!
    *
    \brief
        Set the parentid of this gameobject

    \param _id
        reference to a GameObjectID whose value is to be set to this gameobject's parentid

    \return
        void
    */
    /*******************************************************************************/
    void set_ppid(GameObjectID& _id);
    /*******************************************************************************
    /*!
    *
    \brief
        Get the parentid of this gameobject

    \return
        parent id of this gameobject
    */
    /*******************************************************************************/
    GameObjectID get_ppid() const;

    /*******************************************************************************
    /*!
    *
    \brief
        Deserialize GameObject data from rapidJson value and populate this GameObject with the data

    \return
        if deserialization was successful, return true
        if deserialization was unsuccessful, return false
    */
    /*******************************************************************************/
    bool deserialize(rapidjson::Value& _value);


    /***************************************************************************/
    /*!
    \brief
        Destructor, does nothing
    */
    /**************************************************************************/
    ~GameObject();
};



#endif // !GAME_OBJECT_H

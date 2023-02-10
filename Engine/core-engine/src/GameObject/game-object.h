/*!***************************************************************************************
\file			game-object.h
\project        
\author			Zacharie Hong (50%)
\co-authors     Matthew Lau (50%)

\par			Course: GAM200
\par			Section:
\date			17/07/2022

\brief
    Declares GameObject Class and member functions

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

//INCLUDES

#include <list>
#include <string>

#include "Messaging/message-system.h"
#include <rapidjson/document.h>
#include "Math/math-library.h"
#include "GameObject/Components/transform-component.h"
#include <GameObject/components.h>

//USING

namespace Copium
{

using GameObjectID = uint64_t;

class GameObject final : public IReceiver
{
private:
    friend class GameObjectFactory;
    std::string name;                   //Name of gameObject
    bool active;
    ComponentsPtrArrays componentPtrArrays;

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
    GameObject(GameObjectID _id,
        Math::Vec3 _position = { 0,0,0 },
        Math::Vec3 _rotation = { 0,0,0 },
        Math::Vec3 _scale = { 1,1,1 });

    /*******************************************************************************
    /*!
    *
    \brief
        Copy Constructs using a deep copy of another GameObject
    \param rhs
        Reference to another GameObject

    */
    /*******************************************************************************/
    GameObject(const GameObject& rhs);

    /*******************************************************************************
    /*!
    *
    \brief
        Copy assignment operator, only usable by GOF
    \param _src
        GameObject to copy from
    \return
        Ref to this gameObject after copy assignment

    */
    /*******************************************************************************/
    GameObject& operator=(const GameObject& _src);

public:           
    //Global ID for gameObjects
    const GameObjectID id;
    Transform transform;
    ComponentID assign_id();

    /*******************************************************************************
    /*!
    *
    \brief
        Checks whether a gameobject is active depending on their parent
    \return
        Whether a gameObject is active based on its parent's activeness
    */
    /*******************************************************************************/
    bool isActive();

    /*******************************************************************************
    /*!
    *
    \brief
        Sets a gameeobject's activeness
    \param _active
        Bool to set activeness of gameObject
    */
    /*******************************************************************************/
    void setActive(bool _active);

    /*******************************************************************************
    /*!
    *
    \brief
        Adds a component using a template
    \param T
        Any derived classes of component to be added
    \return
        Added component
    */
    /*******************************************************************************/
    template <typename T>
    T& AddComponent()
    {
        ComponentsPtrArray<T>& components{ componentPtrArrays.GetArray<T>() };
        T* component = new T(*this);
        components.push_back(component);
        component->id = assign_id();
        return *component;
    }

    /*******************************************************************************
    /*!
        \brief
            Gets all components of type from components list
        \param T
            Type of component to get
        \return
            Vector of components gotten
    */
    /*******************************************************************************/
    template <typename T>
    const ComponentsPtrArray<T>& GetComponents()
    {
        return componentPtrArrays.GetArray<T>();
    }

    /*******************************************************************************
    /*!
    *
    \brief
        Template function to add a component to components list
    \param T
        Type of component to add
    \return
        Pointer to component added
    */
    /*******************************************************************************/
    template <typename T>
    T& AddComponent(const T& component)
    {
        ComponentsPtrArray<T>& components{ componentPtrArrays.GetArray<T>() };
        T* tmp = new T(this);
        components.push_back(tmp);

        MESSAGE_CONTAINER::addOrDeleteComponent.gameObjID = id;
        MESSAGE_CONTAINER::addOrDeleteComponent.componentID = component->id;
        MessageSystem::Instance()->dispatch(MESSAGE_TYPE::MT_ADD_COMPONENT);
        return *tmp;
    }

    /*******************************************************************************
    /*!
    *
    \brief
        Template function to get a component from components list
    \param T
        Type of component to get
    \return
        Pointer to component gotten
    */
    /*******************************************************************************/
    template <typename T>
    T* GetComponent()
    {
        ComponentsPtrArray<T>& components{componentPtrArrays.GetArray<T>()};
        if (!components.isEmpty())
            return components[0];
       /* std::cout << tName << "returns a nullptr" << std::endl;*/
        return nullptr;
    }

    /*******************************************************************************
    /*!
    *
    \brief
        Deletes a component from components list
    \param componentID
        ID of component to match in order to delete it from the components list
    */
    /*******************************************************************************/
    void removeComponent(ComponentID componentID);

    /*******************************************************************************
    /*!
    *
    \brief
        Template function to delete a component from components list
    \param T
        Type of component to delete
    */
    /*******************************************************************************/
    template <typename T>
    void removeComponent()
    {
        static_assert(std::is_base_of<Component, T>::value);
        std::string tName = typeid(T).name() + std::string("class Copium::").length();
        ComponentType componentType = Component::nameToType(tName); 
        removeComponent(componentType);
    }

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
        Deserialize GameObject data from rapidJson value and populate this GameObject with the data

    \param  _value
        reference to the rapidjson::Value that contains the GameObject data

    \return
        if deserialization was successful, return true
        if deserialization was unsuccessful, return false
    */
    /*******************************************************************************/
    bool deserialize(rapidjson::Value& _value);

    /*******************************************************************************
    /*!
    *
    \brief
        Serialize this GameObject's data into a rapidjson::Value

    \param  _value
        reference to the rapidjson::Value that will contain the GameObject data

    \param _doc
        reference to the rapidjson::Document which is associated with the save file

    \return
        if deserialization was successful, return true
        if deserialization was unsuccessful, return false
    */
    /*******************************************************************************/
    bool serialize(rapidjson::Value& _value, rapidjson::Document& _doc);

    /*******************************************************************************
    /*!
    *
    \brief
        Displays the inspector view with its transform and all its components

    */
    /*******************************************************************************/
    void inspectorView();


    /***************************************************************************/
    /*!
    \brief
        Destructor, does nothing
    */
    /**************************************************************************/
    ~GameObject();

    /*******************************************************************************
    /*!
    *
    \brief
        Mainly waits for scripting to be updated so that it can tell scripting
           system to remake a copy of this gameObject
    \param mType
        Type of message gotten

    */
    /*******************************************************************************/
    void handleMessage(MESSAGE_TYPE mType);
};

}


#endif // !GAME_OBJECT_H

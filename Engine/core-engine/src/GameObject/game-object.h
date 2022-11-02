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
#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

//INCLUDES

#include <list>
#include <string>

#include "Messaging/message-system.h"
#include <rapidjson/document.h>
#include "Math/math-library.h"
#include "GameObject/transform-component.h"

//USING

namespace Copium
{

using GameObjectID = uint64_t;

class GameObject final : public IReceiver
{
private:
    GameObjectID parentid;
    static GameObjectID count;
    std::list<Component*> components;   //Components for gameObject
    std::string name;                   //Name of gameObject
    TransformComponent transform;
    GameObject* parent;                 //Pointer to this gameObject's parent
    std::list<GameObject*> children;    //List of pointers to this gameObject's children

public:
    const GameObjectID id;                    //Global ID for gameObjects

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
    GameObject(
        Math::Vec3 _position = { 0,0,0 },
        Math::Vec3 _rotation = { 0,0,0 },
        Math::Vec3 _scale = {1,1,1});

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
        Adds a component using a template
    \param T
        Any derived classes of component to be added
    \return
        Added component
    */
    /*******************************************************************************/
    template <typename T>
    T& addComponent()
    {
        static_assert(std::is_base_of<Component, T>::value);
        T* component = new T(*this);
        components.push_back(component);
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
    const std::vector<T*>& getComponents()
    {
        static_assert(std::is_base_of<Component, T>::value);
        static std::vector<T*> typedComponents;
        typedComponents.clear();

        std::string tName = typeid(T).name() + std::string("class Copium::").length();
        ComponentType componentType = Component::nameToType(tName);
        for (Component* pComponent : components)
        {
            if (pComponent->componentType == componentType)
            {
                typedComponents.push_back(reinterpret_cast<T*>(pComponent));
            }
        }
        return typedComponents;
    }

    /*******************************************************************************
    /*!
        \brief
            Gets a component of type from components list
        \param componentType
            ComponentType of component to get
        \return
            Pointer to component gotten
    */
    /*******************************************************************************/
    Component* getComponent(ComponentType componentType);

    /***************************************************************************/
    /*!
    \brief
        Appends a new component by taking in component type to components list
    \param componentType
        Type of component to append to components list
    */
    /**************************************************************************/
    Component* addComponent(ComponentType componentType);

    /*******************************************************************************
    /*!
    *
    \brief
        Checks if components list has a component of given type
    \param componentType
        Type of component to find
    \return
        True if component of type exists in components list
    */
    /*******************************************************************************/
    bool hasComponent(ComponentType componentType) const;


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
    T* addComponent(const T& component)
    {
        static_assert(std::is_base_of<Component, T>::value);
        T* tmp = addComponent(component.componentType);
        *tmp = component;
        return tmp;
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
    T* getComponent()
    {

        static_assert(std::is_base_of<Component, T>::value);
        std::string tName = typeid(T).name() + std::string("class Copium::").length();
        ComponentType componentType = Component::nameToType(tName);
        for (Component* pComponent : components)
        {
            if (pComponent->componentType == componentType)
            {
                return reinterpret_cast<T*>(pComponent);
            }
        }
       /* std::cout << tName << "returns a nullptr" << std::endl;*/
        return nullptr;
    }

    /***************************************************************************/
    /*!
    \brief
        Deletes a component from components list
    \param component
        Pointer to component to delete from components list
    */
    /**************************************************************************/
    void removeComponent(ComponentType componentType);

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

    /***************************************************************************/
    /*!
    \brief
        Getter for gameObject Transform
    \return
        Return a copy transform of gameObject
    */
    /**************************************************************************/
    TransformComponent& Transform();

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

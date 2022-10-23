/*!***************************************************************************************
\file			component.h
\project        
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			17/07/2022

\brief
    Declares Component Class to be added to GameObjects and its member functions

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef COMPONENT_H
#define COMPONENT_H

//INCLUDES
#include <string>
#include <map>
#include "Math/transform.h"

class GameObject;

    //USING

using ComponentID = unsigned char;

class Component
{
    public:
        enum class Type       // Types of Components
        {
            Animator,
            Collider,
            Renderer,
            Script,
            Transform
        };

        static std::map<Type, const std::string> componentMap; // Declared map to link Component::Type and its name

        /***************************************************************************/
        /*!
        \brief
            Function called when component is removed or deleted from GameObject
        */
        /**************************************************************************/
        virtual void destroy();
        /***************************************************************************/
        /*!
        \brief
            Getter for component id
        \return
            Id of this component
        */
        /**************************************************************************/
        ComponentID const ID();

        Component::Type get_type();

        const std::string& Name();

        /***************************************************************************/
        /*!
        \brief
            Pure virtual function that deserializes this component's data from a rapidjson value.
            Note: each component must define how to deserialize itself as each component has its own data structure

        \param _value
            reference to the rapidjson value that contains the data of the component
            
        \return
            void
        */
        /**************************************************************************/
        virtual void deserialize(rapidjson::Value& _value);

        virtual ~Component()
        {
            //std::cout << "default component dtor\n";
        }

        bool Enabled() const noexcept;

        void Enabled(bool) noexcept;
    protected:

        /***************************************************************************/
        /*!
        \brief
            Hidden base class default constructor to prevent creation and force
            derived classes from inheriting
        */
        /**************************************************************************/
        Component() = delete;

        GameObject& gameObj;
        Component(GameObject& _gameObj, Component::Type _componentType);
    private:
        ComponentID id;                     //Id of component, local to gameObject
        Type componentType;                 //Type of component
        const bool allowMultiple = false;   //Can gameObjects only have one of this Component?
        bool enabled;
};

    class ColliderComponent : public Component
    {
    public:
        /***************************************************************************/
        /*!
        \brief
            Default constructor for collider Components
        */
        /**************************************************************************/
        ColliderComponent(GameObject& _gameObj);

        /***************************************************************************/
        /*!
        \brief
            Deserialize this component's data from specified rapidjson value
        */
        /**************************************************************************/
        //void deserialize(rapidjson::Value& _value);
    protected:
    };

    class AnimatorComponent : public Component
    {
    public:
        /***************************************************************************/
        /*!
        \brief
            Default constructor for animator Components
        */
        /**************************************************************************/
        AnimatorComponent(GameObject& _gameObj);

        /***************************************************************************/
        /*!
        \brief
            Deserialize this component's data from specified rapidjson value
        */
        /**************************************************************************/
        //void deserialize(rapidjson::Value& _value);
    protected:

    };

    class TransformComponent : public Component
    {
    public:
        /***************************************************************************/
        /*!
        \brief
            Default constructor for transform Components

        \return
            void
        */
        /**************************************************************************/
        TransformComponent(GameObject& _gameObj);
        /***************************************************************************/
        /*!
        \brief
            Destructor for transform Components

        \return
            void
        */
        /**************************************************************************/
        ~TransformComponent()
        {
            //std::cout << "transform component dtor\n";
        }

        /***************************************************************************/
        /*!
        \brief
            Deserialize this component's data from specified rapidjson value

        \return 
            void
        */
        /**************************************************************************/
        void deserialize(rapidjson::Value& _value);

        /***************************************************************************/
        /*!
        \brief
            Get read-only reference to the transform object in this component

        \return
            read-only reference to the transform object in this component
        */
        /**************************************************************************/
        const Transform& get_transform() const
        {
            return t;
        }
        
        /***************************************************************************/
        /*!
        \brief
            Set the transform object to the specified transform object.
            Performs deep copy of the specified transform object into the transform object in this component.

        \param _src
            read-only reference to the transform object that is to be copied

        \return
            void
        */
        /**************************************************************************/
        void set_transform(const Transform& _src)
        {
            t = _src;
        }
    
    private:
        Transform t;
        
    };

    // Creators - 1x for each component
    class ComponentCreator 
    {
    public:

        /*******************************************************************************
        /*!
        *
        \brief
            Pure virtual function that each component must define. Handles the creation of the component tied to this creator
            E.g TransformCreator should be able to create a TransformComponent using this function

        \return
            if Component was successfully created, return pointer to the newly created Component
            if there were any errors in the process, return nullptr
        */
        /*******************************************************************************/
        virtual Component* create(GameObject& _gameObj) = 0;
        /*******************************************************************************
        /*!
        *
        \brief
            Virtual destructor for Component classes and all derived classes

        \return
            void
        */
        /*******************************************************************************/
        virtual ~ComponentCreator(){}
    };
    class AnimatorCreator : public ComponentCreator 
    {
    public:
        /*******************************************************************************
        /*!
        *
        \brief
            Default contructor for AnimatorCreator

        \return
            void
        */
        /*******************************************************************************/
        AnimatorCreator() 
        {
            std::cout << "Animator registered\n";
        }
        /***************************************************************************/
        /*!
        \brief
            Creates an Animator Component

        \return
            if successful in creating an Animator Component, return ptr to it
            if there were errors in the process, return nullptr
        */
        /**************************************************************************/
	    virtual Component* create(GameObject& _gameObj) {
		    return new AnimatorComponent(_gameObj);
	    }
        /*******************************************************************************
        /*!
        *
        \brief
            Destructor for AnimatorCreator

        \return
            void
        */
        /*******************************************************************************/
        ~AnimatorCreator()
        {
            //std::cout << "Animator Creator destructed\n";
        }
    };

    class ColliderCreator : public ComponentCreator 
    {
    public:
        /*******************************************************************************
        /*!
        *
        \brief
            Default contructor for Collider Creator

        \return
            void
        */
        /*******************************************************************************/
        ColliderCreator()
        {
            std::cout << "Collider registered\n";
        }
        /***************************************************************************/
        /*!
        \brief
            Creates an Collider Component

        \return
            if successful in creating an Collider Component, return ptr to it
            if there were errors in the process, return nullptr
        */
        /**************************************************************************/
        virtual Component* create(GameObject& _gameObj)
        {
            return new ColliderComponent(_gameObj);
        }
        /*******************************************************************************
        /*!
        *
        \brief
            Destructor for Collider Creator

        \return
            void
        */
        /*******************************************************************************/
        ~ColliderCreator()
        {
            //std::cout << "Collider Creator destructed\n";
        }
    };
    class TransformCreator : public ComponentCreator
    {
    public:
        /*******************************************************************************
        /*!
        *
        \brief
            Default contructor for Transform Creator

        \return
            void
        */
        /*******************************************************************************/
        TransformCreator()
        {
            std::cout << "Transform registered\n";
        }
        /***************************************************************************/
        /*!
        \brief
            Creates an Transform Component

        \return
            if successful in creating an Transform Component, return ptr to it
            if there were errors in the process, return nullptr
        */
        /**************************************************************************/
        virtual Component* create(GameObject& _gameObj)
        {
            return new TransformComponent(_gameObj);
        }
        /*******************************************************************************
        /*!
        *
        \brief
            Destructor for Transform Creator

        \return
            void
        */
        /*******************************************************************************/
        ~TransformCreator()
        {
            //std::cout << "Transform Creator destructed\n";
        }
    };

#endif // !COMPONENT_H

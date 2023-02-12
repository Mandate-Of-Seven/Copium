/*!***************************************************************************************
\file			component.h
\project        
\author			Zacharie Hong (50%)
\co-author      Matthew Lau (50%)

\par			Course: GAM200
\par			Section:
\date			17/07/2022

\brief
    Declares Component Class to be added to GameObjects and its member functions

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef COMPONENT_H
#define COMPONENT_H

//INCLUDES
#include <string>
#include <map>
#include <rapidjson/document.h>
#include "CopiumCore/uuid.h"


namespace Copium
{

class GameObject;

    //USING

using ComponentID = uint64_t;


enum class ComponentType : int      // Types of Components
{
    Animator,
    AudioSource,
    BoxCollider2D,
    Button,
    Camera,
    Image,
    Rigidbody2D,
    SpriteRenderer,
    Script,
    Transform,
    Text,
    SortingGroup,
    None
};

static std::map<ComponentType, std::string> MAP_COMPONENT_TYPE_NAME
{
    {ComponentType::Animator,"Animator"},
    {ComponentType::AudioSource,"AudioSource"},
    {ComponentType::BoxCollider2D,"BoxCollider2D"},
    {ComponentType::Button,"Button"},
    {ComponentType::Camera,"Camera"},
    {ComponentType::Image,"ImageComponent"},
    {ComponentType::Rigidbody2D,"Rigidbody2D"},
    {ComponentType::SpriteRenderer,"SpriteRenderer"},
    {ComponentType::Script,"Script"},
    {ComponentType::Button,"Button"},
    {ComponentType::Text,"Text"},
    {ComponentType::SortingGroup,"SortingGroup"},
};


class Component
{
    public:
        Component(const Component&) = delete;

        const ComponentType componentType;      //Type of component
        ComponentID id;                   //Id of component
        UUID uuid;


        uint64_t GetUID() const { return uuid; }
        /*******************************************************************************
        /*!
        *
        \brief
            Converts a name to ComponentType
        \param _name
            Name to look for in the map
        \return
            ComponentType
        */
        /*******************************************************************************/
        static ComponentType nameToType(const std::string& _name)
        {
            int i {0};
            int end{ (int)ComponentType::None };
            while (i != end)
            {
                if (MAP_COMPONENT_TYPE_NAME[(ComponentType)i] == _name)
                    return (ComponentType)i;
                i+=1;
            }
            return ComponentType::None;
        }

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
            Overridable name function for scripts to return their scriptname as their
            name
        \return
            Const reference to the name of this component
        */
        /**************************************************************************/
        virtual const std::string& Name() const;

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

        /***************************************************************************/
        /*!
        \brief
            Pure virtual function that serializes this component's data to a rapidjson value.
            Note: each component must define how to serialize itself as each component has its own data structure

        \param _value
            reference to the rapidjson value that will contain the data of the component

        \return
            void
        */
        /**************************************************************************/
        virtual void serialize(rapidjson::Value& _value, rapidjson::Document& _doc);

        /***************************************************************************/
        /*!
        \brief
            Clone function for preview mode and editor mode
        \param _gameObj
            GameObject to clone from
        \return
            Reference to the cloned component in current scene
        */
        /**************************************************************************/
        virtual Component* clone(GameObject& _gameObj, ComponentID* newID = nullptr) const = 0;

        /***************************************************************************/
        /*!
        \brief
            Links references to other gameObjects or components on deserialization
        \param _value
            Json value to get ID of references
        */
        /**************************************************************************/
        virtual void deserializeLink(rapidjson::Value& _value) { (void)_value; };

        /***************************************************************************/
        /*!
        \brief
            Links references to other gameObjects or components on serialization
        \param rhs
            Reference component to know which gameObject to assign itself to
        */
        /**************************************************************************/
        virtual void previewLink(Component* rhs) { (void)rhs; };

        /*******************************************************************************
        /*!
        *
        \brief
            Displays the inspector view with its fields

        */
        /*******************************************************************************/
        virtual void inspector_view();

        /*******************************************************************************
        /*!
        *
        \brief
            Default constructor that does nothing

        */
        /*******************************************************************************/
        virtual ~Component()
        {
            //std::cout << "default component dtor\n";
        }



        /*******************************************************************************
        /*!
        *
        \brief
            Deep copies a Component into another by making it type safe in the sense
            that they must be the same component types
        \param rhs
            Reference to another Component
        \return
            Reference to this Component
        */
        /*******************************************************************************/
        template<typename T>
        T& operator=(const T& rhs)
        {
            static_assert(std::is_base_of<Component, T>::value); 
            COPIUM_ASSERT(componentType != rhs.componentType, "TRYING TO COPY ASSIGN TWO DIFFERENT COMPONENT TYPES!");
            enabled = rhs.enabled;
            T* self = reinterpret_cast<T*>(this);
            *self = rhs;
            return *self;
        }


        /***************************************************************************/
        /*!
        \brief
            Clone function for preview mode and editor mode
        \param _gameObj
            GameObject to clone from
        \return
            Reference to the cloned component in current scene
        */
        /**************************************************************************/
        bool Enabled() const noexcept;


        /***************************************************************************/
        /*!
        \brief
            Clone function for preview mode and editor mode
        \param _gameObj
            GameObject to clone from
        \return
            Reference to the cloned component in current scene
        */
        /**************************************************************************/
        void Enabled(bool) noexcept;

        GameObject& gameObj;
    protected:

        /***************************************************************************/
        /*!
        \brief
            Hidden base class default constructor to prevent creation and force
            derived classes from inheriting
        */
        /**************************************************************************/
        Component() = delete;

        /***************************************************************************/
        /*!
        \brief
            Hidden base class constructor for derived classes to implement
            determine the componentType
        \param _gameObj
            Owner of this component
        \param _componentType
            Type of component to be defined by derived classes
        */
        /**************************************************************************/
        Component(GameObject& _gameObj, ComponentType _componentType);
    private:
        const bool allowMultiple = false;   //Can gameObjects only have one of this Component?
        bool enabled;
};

}
#endif // !COMPONENT_H

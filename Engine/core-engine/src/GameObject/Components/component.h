/*!***************************************************************************************
\file			component.h
\project        
\author			Zacharie Hong

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

namespace Copium
{

class GameObject;

    //USING

using ComponentID = uint64_t;


enum class ComponentType : int      // Types of Components
{
    Animator,
    BoxCollider2D,
    Rigidbody2D,
    SpriteRenderer,
    Script,
    Transform,
    Button,
    Text,
    Image,
    AudioSource,
    None
};

static std::map<ComponentType, std::string> MAP_COMPONENT_TYPE_NAME
{
    {ComponentType::Animator,"AnimatorComponent"},
    {ComponentType::BoxCollider2D,"BoxCollider2D"},
    {ComponentType::Rigidbody2D,"Rigidbody2D"},
    {ComponentType::SpriteRenderer,"SpriteRenderer"},
    {ComponentType::Script,"ScriptComponent"},
    {ComponentType::Button,"ButtonComponent"},
    {ComponentType::Text,"TextComponent"},
    {ComponentType::Image,"ImageComponent"},
    {ComponentType::AudioSource,"AudioSourceComponent"}
};

class Component
{
    public:
        Component(const Component&) = delete;

        const ComponentType componentType;      //Type of component
        const ComponentID id;                   //Id of component

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




        /*******************************************************************************
        /*!
        *
        \brief
            Displays the inspector view with its fields

        */
        /*******************************************************************************/
        virtual void inspector_view() = 0;

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
            Deep copies a Component into another
        \param rhs
            Reference to another Component
        \return
            Reference to this Component
        */
        /*******************************************************************************/
        Component& operator=(const Component& rhs)
        {
            enabled = rhs.enabled;
            return *this;
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

        bool Enabled() const noexcept;

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
        static ComponentID count;
        bool enabled;
};

    

    class AnimatorComponent : public Component
    {
    public:
        /***************************************************************************/
        /*!
        \brief
            Constructor for animator Components
        \param _gameObj
            Owner of this component
        */
        /**************************************************************************/
        AnimatorComponent(GameObject& _gameObj);


        /*******************************************************************************
        /*!
        *
        \brief
            Displays the inspector view with its fields

        */
        /*******************************************************************************/
        void inspector_view(){};


        /*******************************************************************************
        /*!
        *
        \brief
            Deep copies a AnimatorComponent into another
        \param rhs
            Reference to another AnimatorComponent
        \return
            Reference to this AnimatorComponent
        */
        /*******************************************************************************/
        AnimatorComponent& operator=(const AnimatorComponent& rhs) { (void)rhs; return *this; }
        /***************************************************************************/
        /*!
        \brief
            Deserialize this component's data from specified rapidjson value
        */
        /**************************************************************************/
        //void deserialize(rapidjson::Value& _value);
    protected:

    };

}
#endif // !COMPONENT_H

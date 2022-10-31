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
#include <rapidjson/document.h>

class GameObject;

    //USING

using ComponentID = unsigned char;


enum class ComponentType : int      // Types of Components
{
    Animator,
    Collider,
    Renderer,
    Script,
    Transform,
    UIButton,
    UIText,
    UIImage,
    None
};

static std::map<ComponentType, const std::string> MAP_COMPONENT_TYPE_NAME
{
    {ComponentType::Animator,"AnimatorComponent"},
    {ComponentType::Collider,"ColliderComponent"},
    {ComponentType::Renderer,"RendererComponent"},
    {ComponentType::Script,"ScriptComponent"},
    {ComponentType::UIButton,"UIButtonComponent"},
    {ComponentType::UIText,"UITextComponent"},
    {ComponentType::UIImage,"UIImageComponent"}
};

class Component
{
    public:
        const ComponentType componentType;           //Type of component

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
            Getter for component id
        \return
            Id of this component
        */
        /**************************************************************************/
        ComponentID const ID();

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

        virtual void inspector_view() = 0;

        virtual ~Component()
        {
            //std::cout << "default component dtor\n";
        }

        Component& operator=(const Component& rhs);

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
        Component(GameObject& _gameObj, ComponentType _componentType);

        GameObject& gameObj;
    private:
        ComponentID id;                     //Id of component, local to gameObject
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

        void inspector_view(){};

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

        void inspector_view(){};

        /***************************************************************************/
        /*!
        \brief
            Deserialize this component's data from specified rapidjson value
        */
        /**************************************************************************/
        //void deserialize(rapidjson::Value& _value);
    protected:

    };

#endif // !COMPONENT_H

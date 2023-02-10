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
#include <config.h>


namespace Copium
{

class GameObject;

class Component
{
    public:
        Component(const Component&) = delete;

        ComponentID id;                   //Id of component



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
        Component(GameObject& _gameObj);
    private:
        const bool allowMultiple = false;   //Can gameObjects only have one of this Component?
        bool enabled;
};

}
#endif // !COMPONENT_H

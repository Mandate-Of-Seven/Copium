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

#pragma once

#ifndef COMPONENT_H

#define COMPONENT_H
//INCLUDES
#include <string>
#include <map>

//USING
using ComponentID = unsigned char;

class Component
{
public:
    enum Type       // Types of Components
    {
        Animator,
        Collider,
        Renderer,
        Script
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

    bool Enabled() const noexcept;

    void Enabled(bool) noexcept;

    const std::string& Name();
protected:

    /***************************************************************************/
    /*!
    \brief
        Hidden base class default constructor to prevent creation and force
        derived classes from inheriting
    */
    /**************************************************************************/
    Component();

    /***************************************************************************/
    /*!
    \brief
        Hidden constructor that is called by derived classes to determine name
        and type
    */
    /**************************************************************************/
    Component(Component::Type _componentType);



private:
    ComponentID id;                     //Id of component, local to gameObject
    Type componentType;                 //Type of component
    const bool allowMultiple = false;   //Can gameObjects only have one of this Component?
    bool enabled;
};

class ColliderComponent: public Component
{
public:
    /***************************************************************************/
    /*!
    \brief
        Default constructor for collider Components
    */
    /**************************************************************************/
    ColliderComponent();
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
    AnimatorComponent();
protected:
};

class RendererComponent : public Component
{
public:
    /***************************************************************************/
    /*!
    \brief
        Default constructor for renderer Components
    */
    /**************************************************************************/
    RendererComponent();
protected:
};




#endif // !COMPONENT_H

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
#include <glm/vec2.hpp> //Vec2
#include <string>

//USING
using ComponentID = unsigned char;


class Component
{
private:
    std::string name = "Component";
    ComponentID id;
public:
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

    std::string Name();
protected:
    /***************************************************************************/
    /*!
    \brief
        Hidden base class default constructor to prevent creation and force
        derived classes from inheriting
    */
    /**************************************************************************/
    Component();
};

#endif // !COMPONENT_H

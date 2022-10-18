/*!***************************************************************************************
\file			transform.h
\project
\author			Zacharie Hong
\co-authors     Matthew Lau

\par			Course: GAM200
\par			Section:
\date			17/07/2022

\brief
    Declares Transform Class to be added to GameObjects and its member functions

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#pragma once

#ifndef TRANSFORM_H

#define TRANSFORM_H

//INCLUDES
#include "pch.h"
#include <rapidjson/document.h>
#include <Math/math-library.h>

class Transform;

class Transform
{
private:
    Copium::Math::Vec3 position;
    Copium::Math::Vec3 rotation;
    Copium::Math::Vec3 scale;
    Transform* parent;
    std::list<Transform*> childList;

public:
    void* operator new(size_t count) = delete;
    void* operator new[](size_t count) = delete;
    void operator delete(void*) = delete;
    void operator delete(void*, size_t) = delete;

    /***************************************************************************/
    /*!
    \brief
        Default constructor, position and rotations to 0, scale to 1, and parent
        to nullptr
    */
    /**************************************************************************/
    Transform();

    /***************************************************************************/
    /*!
    \brief
        Constructor that takes in position, rotations,scale to 1, and initializes
        parent to nullptr
    */
    /**************************************************************************/
    Transform(Copium::Math::Vec3 _position, Copium::Math::Vec3 _rotation = {0,0,0}, Copium::Math::Vec3 _scale = {1,1,1});

    /***************************************************************************/
    /*!
    \brief
        Getter for Position
    \return
        Position of transform
    */
    /**************************************************************************/
    const Copium::Math::Vec3& get_position();
    /***************************************************************************/
    /*!
    \brief
        Gets Position vector and converts it to glm::dvec3
    \return
        Position of transform as a glm::dvec3
    */
    /**************************************************************************/
    glm::dvec3 glmPosition() const;

    /***************************************************************************/
    /*!
    \brief
        Setter for Position
    \param _position
        Position to set for transform
    */
    /**************************************************************************/
    void set_position(const Copium::Math::Vec3& _position);


    /***************************************************************************/
    /*!
    \brief
        Getter for Rotation
    \return
        Rotation of transform
    */
    /**************************************************************************/
    const Copium::Math::Vec3& get_rotation();
    /***************************************************************************/
    /*!
    \brief
        Gets Rotation vector and converts it to glm::dvec3
    \return
        Rotation of transform as a glm::dvec3
    */
    /**************************************************************************/
    glm::dvec3 glmRotation() const;

    /***************************************************************************/
    /*!
    \brief
        Setter for Rotation
    \param _rotation
        Rotation to set for transform
    */
    /**************************************************************************/
    void set_rotation(const Copium::Math::Vec3& _rotation);

    /***************************************************************************/
    /*!
    \brief
        Getter for Scale
    \return
        Scale of transform
    */
    /**************************************************************************/
    const Copium::Math::Vec3& get_scale();
    /***************************************************************************/
    /*!
    \brief
        Gets Scale vector and converts it to glm::dvec3
    \return
        Scale of transform as a glm::dvec3
    */
    /**************************************************************************/
    glm::dvec3 glmScale() const;

    /***************************************************************************/
    /*!
    \brief
        Setter for Scale
    \param _rotation
        Scale to set for transform
    */
    /**************************************************************************/
    void set_scale(const Copium::Math::Vec3& _scale);

    /***************************************************************************/
    /*!
    \brief
        Deserializes transform data from a rapidjson::Value
    \return
        on success, return true
        on failure, return false
    */
    /**************************************************************************/
    bool deserialize(rapidjson::Value& _value);

    RTTR_ENABLE();
};



#endif // !TRANSFORM_H

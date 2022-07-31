/*!***************************************************************************************
\file			transform.h
\project
\author			Zacharie Hong

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
#include <glm/vec3.hpp> //Vec3
#include <list>

//USING
using Vector3 = glm::dvec3;

class Transform;

class Transform final
{
private:
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
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
    Transform(Vector3 _position, Vector3 _rotation = {0,0,0}, Vector3 _scale = {1,1,1});

    /***************************************************************************/
    /*!
    \brief
        Getter for Position
    \return
        Position of transform
    */
    /**************************************************************************/
    Vector3 const Position();

    /***************************************************************************/
    /*!
    \brief
        Setter for Position
    \param _position
        Position to set for transform
    */
    /**************************************************************************/
    void Position(Vector3 _position);


    /***************************************************************************/
    /*!
    \brief
        Getter for Rotation
    \return
        Rotation of transform
    */
    /**************************************************************************/
    Vector3 const Rotation();


    /***************************************************************************/
    /*!
    \brief
        Setter for Rotation
    \param _rotation
        Rotation to set for transform
    */
    /**************************************************************************/
    void Rotation(Vector3 _rotation);

    /***************************************************************************/
    /*!
    \brief
        Getter for Scale
    \return
        Scale of transform
    */
    /**************************************************************************/
    Vector3 const Scale();

    /***************************************************************************/
    /*!
    \brief
        Setter for Scale
    \param _rotation
        Scale to set for transform
    */
    /**************************************************************************/
    void Scale(Vector3 _scale);
};

#endif // !TRANSFORM_H

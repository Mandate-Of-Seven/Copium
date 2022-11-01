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
#ifndef TRANSFORM_H
#define TRANSFORM_H

//INCLUDES
#include "pch.h"
#include "Math/math-library.h"
#include "GameObject/component.h"

namespace Copium
{

class TransformComponent : public Component
{
private:
    TransformComponent* parent;
    std::list<TransformComponent*> childList;

public:
    Math::Vec3 position;
    Math::Vec3 rotation;
    Math::Vec3 scale;

    /***************************************************************************/
    /*!
    \brief
        Constructor that takes in position, rotations,scale to 1, and initializes
        parent to nullptr
    */
    /**************************************************************************/
    TransformComponent(
        GameObject& _gameObj,
        Math::Vec3 _position = Math::Vec3(),
        Math::Vec3 _rotation = Math::Vec3(),
        Math::Vec3 _scale = {1,1,1});

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
    void set_position(const Math::Vec3& _position);


    /***************************************************************************/
    /*!
    \brief
        Getter for Rotation
    \return
        Rotation of transform
    */
    /**************************************************************************/
    const Math::Vec3& get_rotation();
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
    void set_rotation(const Math::Vec3& _rotation);

    /***************************************************************************/
    /*!
    \brief
        Getter for Scale
    \return
        Scale of transform
    */
    /**************************************************************************/
    const Math::Vec3& get_scale();
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
    void set_scale(const Math::Vec3& _scale);

    /***************************************************************************/
    /*!
    \brief
        Deserializes transform data from a rapidjson::Value
    
    \param  _value
        reference to the rapidjson::Value that the transform component data is to be loaded from

    \return
        on success, return true
        on failure, return false
    */
    /**************************************************************************/
    void deserialize(rapidjson::Value& _value);

    /***************************************************************************/
    /*!
    \brief
        Serializes transform data to a rapidjson::Value

    \param  _value
        reference to a rapidjson::Value which will store the transform component data. It will then be saved to a json file

    \param _doc
        reference to a rapidjson::Document that is associated with the save file

    \return
        on success, return true
        on failure, return false
    */
    /**************************************************************************/
    void serialize(rapidjson::Value& _value, rapidjson::Document& _doc);

    void inspector_view();

    TransformComponent& operator=(const TransformComponent& rhs);

    RTTR_ENABLE();
    
};

}



#endif // !TRANSFORM_H

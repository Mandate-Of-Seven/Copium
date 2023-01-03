/*!***************************************************************************************
\file			transform.h
\project
\author			Zacharie Hong
\co-authors     Matthew Lau
                Shawn Tanary

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
#include "GameObject/Components/component.h"

namespace Copium
{

class Transform : public Component
{
public:
    Transform() = default;

    Transform* parent = nullptr;
    std::list<Transform*> children;
    Math::Vec3 position;
    Math::Vec3 rotation;
    Math::Vec3 scale;

    bool hasParent() const
    {
        return parent;
    }

    void setParent(Transform* _parent)
    {
        //Previously had a parent
        if (parent)
            parent->children.remove(this);
        parent = _parent;
        //_parent might be nullptr
        if (_parent)
            _parent->children.push_back(this);
    }

    /***************************************************************************/
    /*!
    \brief
        Constructor that takes in position, rotations,scale to 1, and initializes
        parent to nullptr
    */
    /**************************************************************************/
    Transform(
        ComponentID _entityID,
        Math::Vec3 _position = Math::Vec3(),
        Math::Vec3 _rotation = Math::Vec3(),
        Math::Vec3 _scale = {1,1,1});

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

    void deserializeLink(rapidjson::Value& _value);

    void previewLink(Component* rhs);

    /*******************************************************************************
    /*!
    *
    \brief
        Serialize this transform component's data to the specified rapidjson Value

    \param _value
        reference to the rapidjson Value which the transform component's data is to serialize its data to

    \param _doc
        reference to the rapidjson Document which is associated to the save file which the data is being saved to

    \return
        void

    */
    /*******************************************************************************/
    void serialize(rapidjson::Value& _value, rapidjson::Document& _doc);

    /*******************************************************************************
    /*!
    *
    \brief
        Displays the inspector view with its fields

    */
    /*******************************************************************************/
    void inspector_view();

    Component* clone(ComponentID _entityID) const
    {
        Transform* component = new Transform(_entityID);
        component->position = position;
        component->rotation = rotation;
        component->scale = scale;
        return component;
    }
};

}



#endif // !TRANSFORM_H

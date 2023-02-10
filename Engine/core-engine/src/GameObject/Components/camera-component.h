/*!***************************************************************************************
\file			camera-component.h
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			16/11/2022

\brief
	This file holds the declaration of functions for camera-component.h.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "GameObject/Components/component.h"

#include "Graphics/base-camera.h"

namespace Copium
{
    // Forward declaration of the transform class for the gameobject
    class Transform;

	class Camera : public Component, public BaseCamera
	{
    public:
        /***************************************************************************/
        /*!
        \brief
            Constructor for camera component
        \param _gameObj
            Owner of this component
        */
        /**************************************************************************/
        Camera(GameObject& _gameObj);

        /***************************************************************************/
        /*!
        \brief
            Destructs the camera, exits the base camera and remove it from the 
            graphics render pipeline
        */
        /**************************************************************************/
        ~Camera();

        /*******************************************************************************
        /*!
        \brief
            Deserialize this renderer component's data from the specified rapidjson Value
        \param _value
            reference to the rapidjson Value which the renderer component's data 
            deserializes its data 
        */
        /*******************************************************************************/
        void deserialize(rapidjson::Value& _value);

        /*******************************************************************************
        /*!
        \brief
            Serialize this renderer component's data to the specified rapidjson Value
        \param _value
            reference to the rapidjson Value which the renderer component's data is to 
            serialize its data to
        \param _doc
            reference to the rapidjson Document which is associated to the save file
            which the data is being saved to
        */
        /*******************************************************************************/
        void serialize(rapidjson::Value& _value, rapidjson::Document& _doc);

        /*******************************************************************************
        /*!
        \brief
            Displays the inspector view with its fields
        */
        /*******************************************************************************/
        void inspector_view();

        virtual Camera* clone(GameObject& _gameObj, ComponentID* newID = nullptr) const
        {
            (void)newID;
            Camera* component = new Camera(_gameObj);
            return component;
        }

    protected:
        GameObject* gameObject; // Reference to the gameobject
	};
}
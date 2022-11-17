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


        /*******************************************************************************
        /*!
        *
        \brief
            Displays the inspector view with its fields

        */
        /*******************************************************************************/
        void inspector_view();


        virtual Camera* clone(GameObject& _gameObj) const
        {
            Camera* component = new Camera(_gameObj);
            return component;
        }

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
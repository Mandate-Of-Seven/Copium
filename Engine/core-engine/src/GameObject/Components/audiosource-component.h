/*!***************************************************************************************
\file			audiosource-component.h
\project
\author			Shawn Tanary

\par			Course: GAM200
\par			Section:
\date			25/11/2022

\brief
    This file holds the declaration of functions for audiosource-component.h.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#pragma once
#include "pch.h"
#include "GameObject/Components/component.h"
#include "SceneManager/scene-manager.h"

namespace Copium
{
	class AudioSource : public Component
	{
		protected:
            std::string alias;
            bool overLap = false;
            bool loop = false;
            int loopCount;
		public:

            /***************************************************************************/
            /*!
            \brief
                Constructor for audio source

            \param  _gameObj
                reference to game object that this belongs to
            */
            /**************************************************************************/
            AudioSource(GameObject& _gameObj);

            /***************************************************************************/
            /*!
            \brief
                assignment operator overload

            \param  rhs
                reference to audio source whose values need to be assigned
            */
            /**************************************************************************/
            AudioSource& operator=(const AudioSource& rhs);

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

            /***************************************************************************/
            /*!
            \brief
                Plays the audio file inside the audio source
            */
            /**************************************************************************/
            void play_sound();

            /***************************************************************************/
            /*!
            \brief
                Stops the audio file inside the audio source if its playing
            */
            /**************************************************************************/
            void stop_sound();

            void stop_all_sound();

            /***************************************************************************/
            /*!
            \brief
                Deserializes transform data from a rapidjson::Value

            \param  s
                string to set for the alias
            */
            /**************************************************************************/
            void set_alias(std::string s) { alias = s; }

            /***************************************************************************/
            /*!
            \brief
                Clones the component and returns a pointer to that clone

            \param  _gameObj
                reference to the game object to be cloned

            \return
                return a pointer to the clone
            */
            /**************************************************************************/
            Component* clone(GameObject& _gameObj, ComponentID* newID = nullptr) const;
	};

}

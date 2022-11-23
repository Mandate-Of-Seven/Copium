#pragma once
#include "pch.h"
#include "GameObject/Components/component.h"

namespace Copium
{
	class AudioSource : public Component
	{
		protected:
            std::string alias;
		public:

            AudioSource(GameObject& _gameObj);
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

            void set_alias(std::string s) { alias = s; }

            Component* clone(GameObject& _gameObj) const;
	};

}

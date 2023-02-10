/*!***************************************************************************************
\file			animation-system.h
\project
\author			Sean Ngo
\co-authors     Matthew Lau
                Shawn Tanary

\par			Course: GAM200
\par			Section:
\date			05/01/2023

\brief
	This file holds the declaration of the Animation system.
    The Animation system is responsible for the following:
    1. Keeping track of all animators present in the current scene's game objects
    2. Updating animators every frame

    Animator Component declaration done below as well. It's responsible for the following:
    1. Keeping track and storing all animations attached to itself
    2. Updating the relevant offsets for the current frame of the current animation
        that should be drawn on the screen
    3. Keeping track of the current animator status
    4. Playing, pausing and stoppage of the current animation

All content � 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

/*!***************************************************************************************
\file			animation-struct.h
\project
\author			Sean Ngo
\co-author      Matthew Lau

\par			Course: GAM200
\par			Section:
\date			05/01/2023

\brief
    This file holds the declaration of the Animator and the Animation System

All content � 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include <vector>
#include "CopiumCore/system-interface.h"
#include "GameObject/Components/component.h"
#include "Animation/animation-struct.h"

namespace Copium
{
    class Animator : public Component
    {
    public:

        // For handling playing, pausing
        enum class AnimatorStatus : char
        {
            idle = 0, 
            playing,
            paused
        };

        /***************************************************************************/
        /*!
        \brief
            Constructor for animator components

        \param _gameObj
            Owner of this component
        */
        /**************************************************************************/
        Animator(GameObject& _gameObj);
        
        /*******************************************************************************
        /*!
        *
        \brief
            Displays the inspector view with its relevant fields
        */
        /*******************************************************************************/
        void inspector_view();

        /***************************************************************************/
        /*!
        \brief
            Clone function that is mainly used for transitioning between preview mode and editor mode

        \param _gameObj
            GameObject to clone from

        \return
            Reference to the cloned component in current scene
        */
        /**************************************************************************/
        virtual Animator* clone(GameObject& _gameObj, ComponentID* newID = nullptr) const
        {
            std::cout << "animator clone\n";
            Animator* component = new Animator(_gameObj);
            component->animationCount = animationCount;
            component->animations = animations;

            for (Animation& anim : component->animations)
            {
            	anim.currentFrameIndex = 0;
            	anim.timer = 0;
            	
            }

            return component;
        }

        /***************************************************************************/
        /*!
        \brief
            Deserialize this component's data from specified rapidjson value

        \param _value
            the rapidjson Value that should contain this component's data

        \return
            void
        */
        /**************************************************************************/
        void deserialize(rapidjson::Value& _value);
        /***************************************************************************/
        /*!
        \brief
            Serialize this component's data to a rapidjson value

        \param _value
            the rapidjson Value where this component's data will be written to

        \param _doc
            reference to the rapidjson Document that is being written to

        \return
            void
        */
        /**************************************************************************/
        void serialize(rapidjson::Value& _value, rapidjson::Document& _doc);

        /***************************************************************************/
        /*!
        \brief
            Updates the timer which determines whether to move to the next frame or not

        \param _dt
            delta time

        \return
            void
        */
        /**************************************************************************/
        void Update(double _dt);

        /***************************************************************************/
        /*!
        \brief
            Get the vector of animations attached to this animator

        \return
            reference to this animator's vector of animations
        */
        /**************************************************************************/
        std::vector<Animation>& get_animation_vector() { return animations; }

        /***************************************************************************/
        /*!
        \brief
            Checks if there are any animations in this animator

        \return
            true if there are animations
            false if there are no animations
        */
        /**************************************************************************/
        bool IsEmpty() const { return animations.empty(); }

        void AddAnimation();

        /***************************************************************************/
       /*!
       \brief
           Plays all animations
       */
       /**************************************************************************/
        void PlayAnimation();

        /***************************************************************************/
       /*!
       \brief
           Pauses all animations
       /**************************************************************************/
        void PauseAnimation();
        /***************************************************************************/
        /*!
        \brief
            Add an animation to this animator

        \return
            void
        */
        /**************************************************************************/
        void AddAnimation();
        /***************************************************************************/
        /*!
        \brief
            Play the current animation

        \return
            void
        */
        /**************************************************************************/
        void PlayAnimation();
        /***************************************************************************/
        /*!
        \brief
            Pause the current animation

        \return
            void
        */
        /**************************************************************************/
        void PauseAnimation();
        /***************************************************************************/
        /*!
        \brief
            Get the current animation

        \return
            pointer to the current animation
        */
        /**************************************************************************/
        Animation* GetCurrentAnimation();

        /***************************************************************************/
        /*!
        \brief
            Gets the AnimatorStatus
        \return
            AnimatorStatus
        */
        /**************************************************************************/
        AnimatorStatus GetStatus() { return status; }

        /***************************************************************************/
        /*!
        \brief
            Set the AnimatorStatus to the specified param

        \param _status
            the new AnimatorStatus of this animator

        \return
            void
        */
        /**************************************************************************/
        void SetStatus(AnimatorStatus _status) { status = _status; }

    protected:
        std::vector<Animation> animations;    // The indices of the animations inside the assets-system
        int currentAnimationIndex;      // Current playing animation
        int startingAnimationIndex;     // The first animation that is playing
        unsigned int animationCount;
        bool loop, reverse;
        AnimatorStatus status;
    };

	CLASS_SYSTEM(AnimationSystem)
	{
	public:
        /***************************************************************************/
        /*!
        \brief
            Initialise system flags

        \return
            void
        */
        /**************************************************************************/
        void init();

        /***************************************************************************/
        /*!
        \brief
            Call the update functions of all animators in the current scene
        */
        /**************************************************************************/
        void update();
        
        /***************************************************************************/
        /*!
        \brief
            Does nothing as of now
        */
        /**************************************************************************/
        void exit();

        /***************************************************************************/
        /*!
        \brief
            Pauses all the animators
        */
        /**************************************************************************/
        void PauseAllAnimation();

        /***************************************************************************/
        /*!
        \brief
            Starts all the animators
        */
        /**************************************************************************/
        void PlayAllAnimation();

	private:
        std::vector<Animator*> animators;
	};
}

#endif // !ANIMATION_SYSTEM_H

/*!***************************************************************************************
\file			animation-system.h
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			05/01/2023

\brief
	This file holds the declaration of the Animation system. The Animation system animates
	the spritesheets in the engine, where the user has to create the animation in the 
	animator in the editor.

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

#include <vector>

#include "CopiumCore/system-interface.h"
#include "GameObject/Components/component.h"
#include "Animation/animation-struct.h"

namespace Copium
{
    class Animator : public Component
    {
    public:


        enum class AnimatorStatus : char
        {
            idle = 0, 
            playing
        };
        /***************************************************************************/
        /*!
        \brief
            Constructor for animator Components
        \param _gameObj
            Owner of this component
        */
        /**************************************************************************/
        Animator(GameObject& _gameObj);


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
            Clone function for preview mode and editor mode
        \param _gameObj
            GameObject to clone from
        \return
            Reference to the cloned component in current scene
        */
        /**************************************************************************/
        virtual Animator* clone(GameObject& _gameObj) const
        {
            Animator* component = new Animator(_gameObj);
            return component;
        }

        /***************************************************************************/
        /*!
        \brief
            Deserialize this component's data from specified rapidjson value
        */
        /**************************************************************************/
        //void deserialize(rapidjson::Value& _value);

        void update(float _dt);

        std::vector<Animation>& get_animation_vector() { return animations; }

        bool IsEmpty() const { return animations.empty(); }

        void AddAnimation();
        void PlayAnimation();

        Animation* GetCurrentAnimation() 
        { 
            if (IsEmpty())
                return nullptr;
            return &animations[currentAnimationIndex]; 
        }

    protected:
        std::vector<Animation> animations;    // The indices of the animations inside the assets-system
        int currentAnimationIndex;      // Current playing animation
        int startingAnimationIndex;     // The first animation that is playing
        unsigned int animationCount;
        bool loop;
        AnimatorStatus status;
    };

    /*
        The animation system will:
        1. Loop through all the animation components
        2. For each animation component:
            - Plays the current animation with its current frame
            - Checks if the current frame's timeDelay is achieved to switch to the next frame
            - May loop an animation
            - May pause an animation
    
    */
	CLASS_SYSTEM(AnimationSystem)
	{
	public:
        /***************************************************************************/
        /*!
        \brief
            Inits all the different animation's data and or status
        */
        /**************************************************************************/
        void init();

        /***************************************************************************/
        /*!
        \brief
            Manages all the different animation's data and or status
        */
        /**************************************************************************/
        void update();
        
        /***************************************************************************/
        /*!
        \brief
            Frees up any memory from all animators
        */
        /**************************************************************************/
        void exit();

	private:
        std::vector<Animator*> animators;
	};
}

#endif // !ANIMATION_SYSTEM_H

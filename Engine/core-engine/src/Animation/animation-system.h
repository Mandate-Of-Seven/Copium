/*!***************************************************************************************
\file			animation-system.h
\project
\author			Sean Ngo
\co-authors     Matthew Lau
                Shawn Tanary

\par			Course: GAM250
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

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

#include <vector>
#include "CopiumCore/system-interface.h"
#include "Events/events-system.h"


namespace Copium
{
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
	};
}

#endif // !ANIMATION_SYSTEM_H

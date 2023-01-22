#pragma once

#include <CopiumCore/system-interface.h>

namespace Copium
{
    CLASS_SYSTEM(AnimationSystem)
    {
    public:
        /***************************************************************************/
        /*!
        \brief
            Inits all the different animation's data and or status
        */
        /**************************************************************************/
        void Init();

        /***************************************************************************/
        /*!
        \brief
            Manages all the different animation's data and or status
        */
        /**************************************************************************/
        void Update();

        /***************************************************************************/
        /*!
        \brief
            Frees up any memory from all animators
        */
        /**************************************************************************/
        void Exit();
    };
}
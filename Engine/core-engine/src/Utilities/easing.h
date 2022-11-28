/*!***************************************************************************************
\file			easing.h
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			28/11/2022

\brief
    This file holds the define of functions to interpolate float and struct of floats

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#ifndef EASING_H

#define EASING_H

#include <stdlib.h>
#include <math.h>
#define NATURAL_LOG_OF_2    0.693147181f
#define PI                3.14159265358979323846f  /* pi */

namespace Copium
{
    /***************************************************************************/
    /*!
    \brief
        Linear interpolation for struct of floats and floats
    \param start
        Starting value
    \param end
        Max value
    \param value
        Time value of interpolation between 0 & 1
    \return
        New container
    */
    /**************************************************************************/
    template <typename T,size_t SZ=sizeof(T)/sizeof(float)>
    static T Linear(T start, T end, float value)
    {
        float* fStart = reinterpret_cast<float*>(&start);
        float* fEnd = reinterpret_cast<float*>(&end);
        T container{};
        float* fContainer = reinterpret_cast<float*>(&container);
        for (size_t i = 0; i < SZ; ++i)
        {
            *fContainer = (1.f - value) * (*fStart) + value * (*fEnd);
            ++fContainer;
            ++fStart;
            ++fEnd;
        }
        
        return container;
    }
}
#endif // !EASING_H
#pragma once

#include <stdlib.h>
#include <math.h>
#define NATURAL_LOG_OF_2    0.693147181f
#define PI                3.14159265358979323846f  /* pi */

namespace Copium
{
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
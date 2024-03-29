/*!***************************************************************************************
****
\file			pch.h
\project
\author			Zacharie Hong
\co-authors

\par			Course: GAM250
\par			Section:
\date			18/09/2022

\brief
    Contains precompiled include to reduce build times

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/

#pragma warning( disable : 4390 )

#ifndef PCH_H
#define PCH_H

//DATA STRUCTURES
#include <string>
#include <map>
#include <list>
#include <vector>
#include <array>
#include <queue>

//IMGUI
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

//STREAMS
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>

#include <memory>
#include <algorithm>

//DEBUGGING
#include "Debugging/logging-system.h"
#include "Debugging/frame-rate-controller.h"

void print(){}

template <typename T, typename... Ts>
void print(T val, Ts... others)
{
    std::cout << val;
    print(others);
}



#if defined(DEBUG) | defined(_DEBUG)
    #define PRINT(str) std::cout << str << std::endl
#else
    #define PRINT(str)
#endif

#endif // !PCH_H
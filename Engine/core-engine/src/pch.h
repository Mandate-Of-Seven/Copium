/*!***************************************************************************************
****
\file			pch.h
\project
\author			Zacharie Hong
\co-authors

\par			Course: GAM200
\par			Section:
\date			18/09/2022

\brief
    Contains precompiled include to reduce build times

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/

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

//STREAMS
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>

#include <memory>
#include <algorithm>

//DEBUGGING
#include "Debugging/logging.h"

#define PRINT(str) std::cout << str << std::endl

#endif // !PCH_H
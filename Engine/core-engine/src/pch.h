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

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/

#ifndef PCH_H
#define PCH_H

//DATA STRUCTURES
#include <string>
#include <map>
#include <list>
#include <vector>
#include <array>

//IMGUI
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

//STREAMS
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>

#include <regex>
#include <algorithm>
#include "Audio/sound-system.h"

//DEBUGGING
#include "Debugging/logging.h"

#define PRINT(str) std::cout << str << std::endl

/**************************************************************************/
/*!
    \brief
        Reads bytes from a file
    \param filepath
        File path to read bytes from
    \param outSize
        Amount of bytes read
        
    \return
        String of bytes read
*/
/**************************************************************************/
static char* ReadBytes(const std::string& filepath, uint32_t* outSize)
{
    std::ifstream stream(filepath, std::ios::binary | std::ios::ate);

    if (!stream) return nullptr;
    std::streampos end = stream.tellg();
    stream.seekg(0, std::ios::beg);
    uint32_t size = end - stream.tellg();
    if (size == 0) return nullptr;
    char* buffer = new char[size];
    stream.read((char*)buffer, size);
    stream.close();
    *outSize = size;
    return buffer;
}

#endif // !PCH_H
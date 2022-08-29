#pragma once

//DATA STRUCTURES
#include <string>
#include <map>
#include <list>
#include <vector>
#include <array>
#include <glm/vec3.hpp>

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

#ifndef NDEBUG
#   define M_Assert(Expr, Msg) \
    __M_Assert(#Expr, Expr, __FILE__, __LINE__, Msg)
#else
#   define M_Assert(Expr, Msg) ;
#endif

static void __M_Assert(std::string expr_str, bool expr, std::string file, int line, std::string msg)
{
    if (!expr)
    {
        std::cerr << "Assert failed:\t" << msg << "\n"
            << "Expected:\t" << expr_str << "\n"
            << "Source:\t\t" << file << ", line " << line << "\n";
        abort();
    }
}


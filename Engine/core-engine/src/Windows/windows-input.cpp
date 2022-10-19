/*!***************************************************************************************
\file			windows-input.cpp
\project
\author			Shawn Tanary

\par			Course: GAM200
\par			Section:
\date			16/09/2022

\brief
    This file contins callbacks for keyboard and mouse input detection. 
    Also contains functions to get the key press states or mouse information.

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Windows/input.h"
#include "Windows/windows-input.h"
#include "Windows/windows-system.h"

#define COPIUM_MAX_KEYS 400
#define COPIUM_MAX_MOUSEBUTTONS 5
using namespace Copium;

// Bean: Temporary for window systems declaration
namespace
{
    Windows::WindowsSystem* windowsSystem = Windows::WindowsSystem::Instance();
}

void WindowsInput::init()
{
    get_input_instance()->keys = new short[COPIUM_MAX_KEYS];
    get_input_instance()->mouseButtons = new short[COPIUM_MAX_MOUSEBUTTONS];
    for (int i = 0; i < 400; i++)
    {
        get_input_instance()->keys[i] = 0;
    }
    COPIUM_ASSERT(get_input_instance()->keys == nullptr, "keys was not created properly");
    COPIUM_ASSERT(get_input_instance()->mouseButtons == nullptr, "mouse keys was not created properly");
    
    glfwSetKeyCallback(windowsSystem->get_window(), Input::key_callback);
    glfwSetMouseButtonCallback(windowsSystem->get_window(), Input::mousebutton_callback);
    glfwSetScrollCallback(windowsSystem->get_window(), Input::mousescroll_callback);
    glfwSetCursorPosCallback(windowsSystem->get_window(), Input::mousepos_callback);
    std::cout << "Input init was called" << std::endl;
}

bool WindowsInput::is_key_pressed_impl(int keycode)
{
    COPIUM_ASSERT((keycode > COPIUM_MAX_KEYS), "Keycode entered is out of range");
    if (get_input_instance()->keys[keycode]== GLFW_PRESS)
    {
        std::cout << get_input_instance()->keys[keycode] << "  " << std::endl;
        get_input_instance()->keys[keycode] = 0;
        return true;
    }
    return false;
}

bool WindowsInput::is_key_held_impl(int keycode)
{
    COPIUM_ASSERT((keycode > COPIUM_MAX_KEYS), "Keycode entered is out of range");
    if (get_input_instance()->keys[keycode] == GLFW_REPEAT || get_input_instance()->keys[keycode] == GLFW_PRESS)
    {
        //get_input_instance()->keys[keycode] = 0;
        return true;
    }
    return false;
}

bool WindowsInput::is_mousebutton_pressed_impl(int button)
{
    COPIUM_ASSERT((button > COPIUM_MAX_MOUSEBUTTONS), "Mouse button entered is out of range");
    
    if (get_input_instance()->mouseButtons[button])
    {
        get_input_instance()->mouseButtons[button] = 0;
        return true;
    }

    return false;
}

std::pair<float, float> WindowsInput::get_mouseposition_impl()
{
    auto& window = *windowsSystem->get_window();
    double xPos, yPos;
    glfwGetCursorPos(&window, &xPos, &yPos);

    if (xPos < 0)
    {
        xPos = 0;
    }
    else if (xPos > windowsSystem->get_window_width())
    {
        xPos = windowsSystem->get_window_width();
    }

    if (yPos < 0)
    {
        yPos = 0;
    }
    else if (yPos > windowsSystem->get_window_height())
    {
        yPos = windowsSystem->get_window_height();
    }

    return { (float)xPos , (float)yPos };
}

float WindowsInput::get_mouseX_impl()
{
    auto [xPos, yPos] = get_mouseposition_impl();
    return xPos;
}

float WindowsInput::get_mouseY_impl()
{
    auto [xPos, yPos] = get_mouseposition_impl();
    return yPos;
}

double WindowsInput::get_mousescroll_impl()
{
    double temp = get_input_instance()->mouseScrollOffset;
    get_input_instance()->mouseScrollOffset = 0;
    return temp;
}

void Input::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    (void) mods, scancode, window; // Bean: to prevent warning, remove later
    if (action == GLFW_PRESS)
    {
        get_input_instance()->keys[key] = GLFW_PRESS;
        //std::cout<<key << " some Key pressed" << std::endl;
    }
    else if (action == GLFW_REPEAT)
    {
        get_input_instance()->keys[key] = GLFW_REPEAT;
        //std::cout << key<< " some Key repeatedly pressed" << std::endl;
    }
    else if (action == GLFW_RELEASE)
    {
        get_input_instance()->keys[key] = GLFW_RELEASE;
        //std::cout<< key << " some Key released" << std::endl;
    }
}

void Input::mousebutton_callback(GLFWwindow* window, int button, int action, int mods)
{
    (void) mods, window; // Bean: to prevent warning, remove later
    int target = 0;
    switch (button) 
    {
        case GLFW_MOUSE_BUTTON_LEFT:
            target = GLFW_MOUSE_BUTTON_LEFT;
        #ifdef _DEBUG               
            //std::cout << "Left mouse button ";
        #endif
        break;

        case GLFW_MOUSE_BUTTON_RIGHT:
            target = GLFW_MOUSE_BUTTON_RIGHT;
        #ifdef _DEBUG           
            //std::cout << "Right mouse button ";
        #endif
        break;

        case GLFW_MOUSE_BUTTON_3:
            target = GLFW_MOUSE_BUTTON_3;
        #ifdef _DEBUG           
            //std::cout << "Middle mouse button ";
        #endif
            break;

        case GLFW_MOUSE_BUTTON_4:
            target = GLFW_MOUSE_BUTTON_4;
        #ifdef _DEBUG           
             //std::cout << "Mouse button 4 ";
        #endif
            break;

        case GLFW_MOUSE_BUTTON_5:
            target = GLFW_MOUSE_BUTTON_5;
        #ifdef _DEBUG           
            //std::cout << "Mouse button 5 ";
        #endif
            break;
    }
    switch (action) 
    {
        case GLFW_PRESS:
            get_input_instance()->mouseButtons[target] = 1;
        #ifdef _DEBUG
            //std::cout << "pressed!!!" << std::endl;
        #endif
        break;

        case GLFW_RELEASE:
            get_input_instance()->mouseButtons[target] = 0;
        #ifdef _DEBUG
            //std::cout << "released!!!" << std::endl;
        #endif
        break;
    }
}

void Input::mousescroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
    (void) window;
    get_input_instance()->mouseScrollOffset = yOffset;

    if (yOffset<0)
    {
        //std::cout << "scrolling down" << std::endl;
    }else if (yOffset > 0)
    {
        //std::cout << "scrolling up" << std::endl;
    }
    else if(yOffset == 0 && !xOffset)
    {
        //std::cout << "not scrolling" << std::endl;
    }
    #ifdef _DEBUG
        //std::cout << "Mouse scroll wheel offset: (" << xOffset << ", " << yOffset << ")" << std::endl;
    #endif
}

void Input::mousepos_callback(GLFWwindow* window, double xPos, double yPos)
{
    (void) window, xPos, yPos;
    #ifdef _DEBUG
        //std::cout << "Mouse cursor position: (" << xPos << ", " << yPos << ")" << std::endl;
    #endif
}
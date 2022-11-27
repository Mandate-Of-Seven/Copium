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

#include "Math/math-library.h"
#include "Windows/windows-input.h"
#include "Windows/windows-system.h"

#define COPIUM_MAX_MOUSEBUTTONS 5

// Bean: Temporary for window systems declaration
namespace
{
    Copium::WindowsSystem * windowsSystem = Copium::WindowsSystem::Instance();
    short mouseButtons[COPIUM_MAX_MOUSEBUTTONS];
    double mouseScrollOffset;

}

namespace Copium
{
    short InputSystem::keys[COPIUM_MAX_KEYS];

void InputSystem::init()
{
    systemFlags |= FLAG_RUN_ON_EDITOR | FLAG_RUN_ON_PLAY;
    for (int i = 0; i < 400; i++)
    {
        keys[i] = 0;
    }
    glfwSetKeyCallback(windowsSystem->get_window(), key_callback);
    glfwSetMouseButtonCallback(windowsSystem->get_window(), mousebutton_callback);
    glfwSetScrollCallback(windowsSystem->get_window(), mousescroll_callback);
    glfwSetCursorPosCallback(windowsSystem->get_window(), mousepos_callback);
    std::cout << "Input init was called" << std::endl;
}

void InputSystem::update() 
{

}

void InputSystem::exit() {}

bool InputSystem::is_key_pressed(int keycode)
{
    COPIUM_ASSERT((keycode > COPIUM_MAX_KEYS), "Keycode entered is out of range");
    if (keys[keycode]== GLFW_PRESS)
    {
        //std::cout << keys[keycode] << "  " << std::endl;
        keys[keycode] = 0;
        return true;
    }
    return false;
}

bool InputSystem::is_key_held(int keycode)
{
    COPIUM_ASSERT((keycode > COPIUM_MAX_KEYS), "Keycode entered is out of range");
    if (keys[keycode] == GLFW_REPEAT || keys[keycode] == GLFW_PRESS)
    {
        //keys[keycode] = 0;
        return true;
    }
    return false;
}

bool InputSystem::is_mousebutton_pressed(int button)
{
    COPIUM_ASSERT((button > COPIUM_MAX_MOUSEBUTTONS), "Mouse button entered is out of range");
    
    if (mouseButtons[button])
    {
        return true;
    }

    return false;
}

Copium::Math::Vec2 InputSystem::get_mouseposition()
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

float InputSystem::get_mouseX()
{
    return get_mouseposition().x;
}

float InputSystem::get_mouseY()
{
    return get_mouseposition().y;
}

double InputSystem::get_mousescroll()
{
    double temp = mouseScrollOffset;
    mouseScrollOffset = 0;
    return temp;
}

void InputSystem::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    (void)mods, scancode, window; // Bean: to prevent warning, remove later
    if (action == GLFW_PRESS)
    {
        keys[key] = GLFW_PRESS;
        //std::cout<<key << " some Key pressed" << std::endl;
    }
    else if (action == GLFW_REPEAT)
    {
        keys[key] = GLFW_REPEAT;
        //std::cout << key<< " some Key repeatedly pressed" << std::endl;
    }
    else if (action == GLFW_RELEASE)
    {
        keys[key] = GLFW_RELEASE;
        //std::cout<< key << " some Key released" << std::endl;
    }
}

void InputSystem::mousebutton_callback(GLFWwindow* window, int button, int action, int mods)
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
            mouseButtons[target] = 1;
        #ifdef _DEBUG
            //std::cout << "pressed!!!" << std::endl;
        #endif
        break;

        case GLFW_RELEASE:
            mouseButtons[target] = 0;
        #ifdef _DEBUG
            //std::cout << "released!!!" << std::endl;
        #endif
        break;
    }
}

void InputSystem::mousescroll_callback(GLFWwindow* window, double xOffset, double yOffset)
{
    (void) window;
    mouseScrollOffset = yOffset;

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

void InputSystem::mousepos_callback(GLFWwindow* window, double xPos, double yPos)
{
    (void) window, xPos, yPos;
    #ifdef _DEBUG
        //std::cout << "Mouse cursor position: (" << xPos << ", " << yPos << ")" << std::endl;
    #endif
}



}



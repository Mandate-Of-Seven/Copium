
#include "pch.h"
#include "windows-input.h"
#include <glhelper.h>
#include <GLFW/glfw3.h>  
#include <input.h>
#include <iostream>


void WindowsInput::Init()
{
    getInputInstance()->keys = new short[400];
    for (int i = 0; i < 400; i++)
    {
        getInputInstance()->keys[i] = 0;
    }
    std::cout << "Init was called" << std::endl;
}

bool WindowsInput::isKeyPressedImpl(int keycode)
{
    auto& window = GLHelper::ptr_window;
    auto state = glfwGetKey(window,keycode);
    if (getInputInstance()->keys[keycode]== GLFW_PRESS)
    {
        std::cout << getInputInstance()->keys[keycode] << "  " << std::endl;
        getInputInstance()->keys[keycode] = 0;
        return true;
    }
    return false;
}

bool WindowsInput::isKeyHeldImpl(int keycode)
{
    auto& window = GLHelper::ptr_window;
    auto state = glfwGetKey(window, keycode);
    if (getInputInstance()->keys[keycode] == GLFW_REPEAT || getInputInstance()->keys[keycode] == GLFW_PRESS)
    {
        std::cout << getInputInstance()->keys[keycode] << "  " << std::endl;
        getInputInstance()->keys[keycode] = 0;
        return true;
    }
    return false;
}

bool WindowsInput::isMouseButtonPressedImpl(int button)
{
    auto& window = GLHelper::ptr_window;
    auto state = glfwGetMouseButton(window, button);
    return state == GLFW_PRESS;
}

std::pair<float, float> WindowsInput::getMousePositionImpl()
{
    auto& window = GLHelper::ptr_window;
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);

    if (xPos < 0)
    {
        xPos = 0;
    }
    else if (xPos > GLHelper::width)
    {
        xPos = GLHelper::width;
    }

    if (yPos < 0)
    {
        yPos = 0;
    }
    else if (yPos > GLHelper::height)
    {
        yPos = GLHelper::height;
    }

    return { (float)xPos , (float)yPos };
}

float WindowsInput::getMouseXImpl()
{
    auto [xPos, yPos] = getMousePositionImpl();
    return xPos;
}

float WindowsInput::getMouseYImpl()
{
    auto [xPos, yPos] = getMousePositionImpl();
    return yPos;
}

void Input::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS)
    {
        getInputInstance()->keys[key] = GLFW_PRESS;
        std::cout<<key << " some Key pressed" << std::endl;
    }
    else if (action == GLFW_REPEAT)
    {
        getInputInstance()->keys[key] = GLFW_REPEAT;
        std::cout << key<< " some Key repeatedly pressed" << std::endl;
    }
    else if (action == GLFW_RELEASE)
    {
        getInputInstance()->keys[key] = GLFW_RELEASE;
        std::cout<< key << " some Key released" << std::endl;
    }
}

void Input::mousebuttonCallback(GLFWwindow* window, int button, int action, int mods)
{
    switch (button) 
    {
        case GLFW_MOUSE_BUTTON_LEFT:
        #ifdef _DEBUG
                std::cout << "Left mouse button ";
        #endif
        break;

        case GLFW_MOUSE_BUTTON_RIGHT:
        #ifdef _DEBUG
                std::cout << "Right mouse button ";
        #endif
        break;
    }
    switch (action) 
    {
        case GLFW_PRESS:
        #ifdef _DEBUG
                std::cout << "pressed!!!" << std::endl;
        #endif
        break;

        case GLFW_RELEASE:
        #ifdef _DEBUG
                std::cout << "released!!!" << std::endl;
        #endif
        break;
    }
}

void Input::mousescrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
    #ifdef _DEBUG
        //std::cout << "Mouse scroll wheel offset: (" << xOffset << ", " << yOffset << ")" << std::endl;
    #endif
}

void Input::mouseposCallback(GLFWwindow* window, double xPos, double yPos)
{
    #ifdef _DEBUG
        //std::cout << "Mouse cursor position: (" << xPos << ", " << yPos << ")" << std::endl;
    #endif
}
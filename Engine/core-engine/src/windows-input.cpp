#include "windows-input.h"
#include <glhelper.h>
#include <GLFW/glfw3.h>  
#include <input.h>
#include <iostream>


bool WindowsInput::isKeyPressedImpl(int keycode)
{
    auto& window = GLHelper::ptr_window;
    auto state = glfwGetKey(window,keycode);
    return state == GLFW_PRESS;
}

//currently doesnt work
bool WindowsInput::isKeyHeldImpl(int keycode)
{
    auto& window = GLHelper::ptr_window;
    auto state = glfwGetKey(window, keycode);
    return state == GLFW_REPEAT;
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
    if (key == GLFW_KEY_Q && action == GLFW_PRESS)
    {
        std::cout << "Q Key pressed" << std::endl;
    }
    else if (action == GLFW_REPEAT)
    {
        std::cout << "some Key repeatedly pressed" << std::endl;
    }
    else if (action == GLFW_RELEASE)
    {
        std::cout << "some Key released" << std::endl;
    }
}
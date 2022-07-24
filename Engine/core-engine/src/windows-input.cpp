#include "windows-input.h"
#include <glhelper.h>
#include <GLFW/glfw3.h>  

Input* Input::inputInstance = new WindowsInput();


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

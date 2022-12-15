/*!***************************************************************************************
\file			windows-system.cpp
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			16/09/2022

\brief
    This file implements functionality useful and necessary to build OpenGL
    applications including use of external APIs such as GLFW to create a
    window and start up an OpenGL context and use GLEW to extract function 
    pointers to OpenGL implementations.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#include "pch.h"

#include "Windows/windows-system.h"
#include "Files/file-system.h"
#include <Debugging/frame-rate-controller.h>
#include "Windows/windows-input.h"

namespace Copium
{
    int WindowsSystem::windowWidth;
    int WindowsSystem::windowHeight;
    bool WindowsSystem::windowFocused;
    bool WindowsSystem::windowFullscreen;

    void WindowsSystem::init()
    {
        systemFlags |= FLAG_RUN_ON_EDITOR | FLAG_RUN_ON_PLAY;
        init_system(1600, 900, "Copium");
    }

    void WindowsSystem::init_system(int _width, int _height, std::string _title)
    {
        windowWidth = _width;
        windowHeight = _height;
        windowFocused = true;
        windowFullscreen = false;
        title = _title;
        
        std::string config("Data\\config.json");
        load_config(config, windowWidth, windowHeight);

        if (!glfwInit())
        {
            PRINT("GLFW init has failed - abort program!!!");
            abort();
        }

        // In case a GLFW function fails, an error is reported to callback function
        glfwSetErrorCallback(error_callback);

        // Before asking GLFW to create an OpenGL context, we specify the minimum constraints
        // in that context:
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
        glfwWindowHint(GLFW_RED_BITS, 8); glfwWindowHint(GLFW_GREEN_BITS, 8);
        glfwWindowHint(GLFW_BLUE_BITS, 8); glfwWindowHint(GLFW_ALPHA_BITS, 8);
        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE); // window dimensions are NOT static

        //GLFWwindow * ptr_window = glfwCreateWindow(_width, _height, _title.c_str(), NULL, NULL);
        window = glfwCreateWindow(_width, _height, _title.c_str(), NULL, NULL);

        if (!window)
        {
            std::cerr << "GLFW unable to create OpenGL context - abort program\n";
            glfwTerminate();
            abort();
        }

        glfwMakeContextCurrent(window);

        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetWindowFocusCallback(window, window_focus_callback);
        glfwSetDropCallback(window, window_drop_callback);

        // this is the default setting ...
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

        // Initialize entry points to OpenGL functions and extensions
        GLenum err = glewInit();
        if (GLEW_OK != err)
        {
            std::cerr << "Unable to initialize GLEW - error: "
                << glewGetErrorString(err) << " abort program" << std::endl;
            abort();
        }
        if (GLEW_VERSION_4_5)
        {
            PRINT("Using glew version: " << glewGetString(GLEW_VERSION));
            PRINT("Driver supports OpenGL 4.5\n");
        }
        else
        {
            std::cerr << "Driver doesn't support OpenGL 4.5 - abort program" << std::endl;
            abort();
        }

        // Initialise Viewport
        glViewport(0, 0, windowWidth, windowHeight);
    }

    void WindowsSystem::update()
    {
        static bool fullscreen = false;
        if (InputSystem::Instance()->is_key_pressed(GLFW_KEY_F))
        {
            fullscreen = !fullscreen;
            Fullscreen(fullscreen, 1600, 900);
        }

        if (!glfwWindowShouldClose(window))
            glfwPollEvents();
    }

    void WindowsSystem::draw()
    {
        // Printing to Windows Title Bar
        std::stringstream sstr;
        sstr << std::fixed << std::setprecision(2) << title << " | FPS: " << MyFrameRateController.getFPS()
            << " | Resolution: " << windowWidth << " by " << windowHeight;
        glfwSetWindowTitle(window, sstr.str().c_str());

        glfwSwapBuffers(window);
    }

    void WindowsSystem::exit()
    {
        //glfwTerminate();
    }

    void WindowsSystem::Fullscreen(bool _enable, int _width, int _height)
    {
        // Dont update if it is already in the selected window mode
        if (_enable == windowFullscreen)
            return;

        if (_enable != windowFullscreen)
        {
            windowFullscreen = _enable;

            // If enabled, set window to fullscreen
            if (windowFullscreen)
            {
                GLFWmonitor* currentMonitor = glfwGetPrimaryMonitor();
                int width, height;
                glfwGetMonitorWorkarea(currentMonitor, NULL, NULL, &width, &height);
                if (currentMonitor != nullptr)
                {
                    glfwSetWindowMonitor(window, currentMonitor, 0, 0, width, height, 0);
                }
            }
            // Set to maximized
            else
            {
                glfwSetWindowMonitor(window, NULL, _width * 0.1f, _height * 0.1f, _width, _height, 0);
                glfwMaximizeWindow(window);
            }
        }
    }

    void WindowsSystem::error_callback(int _error, char const* _description)
    {
#ifdef _DEBUG
        std::cerr << "GLFW error: " << _description << std::endl;
#endif
        (void) _error, _description;
    }

    void WindowsSystem::framebuffer_size_callback(GLFWwindow* _window, int _width, int _height)
    {
#ifdef _DEBUG
        PRINT("Framebuffer size getting called!!!");
#endif
        // Bean: This would be changed once we implement IMGUI
        // 
        // use the entire framebuffer as drawing region
        windowWidth = _width;
        windowHeight = _height;
        glViewport(0, 0, _width, _height);

        // later, if working in 3D, we'll have to set the projection matrix here ...
        (void) _window;
    }

    void WindowsSystem::window_focus_callback(GLFWwindow* _window, int _focused)
    {
#ifdef _DEBUG
        PRINT("Window focus getting called!!!");
#endif
        windowFocused = (bool) _focused;
        (void) _window;
    }

    void WindowsSystem::window_drop_callback(GLFWwindow* _window, int _pathCount, const char* _paths[])
    {
#ifdef _DEBUG
        PRINT("Window drop getting called!!!");
#endif        
        FileSystem::Instance()->accept_dropped_files(_pathCount, _paths);

        (void) _window;
    }

    bool WindowsSystem::load_config(std::string& _filename, GLint& _w, GLint& _h)
    {
        std::ifstream ifs(_filename);
        if (!ifs)
        {
            std::cout << "Error opening config json file!\n";
            return false;
        }
        rapidjson::IStreamWrapper isw(ifs);
        rapidjson::Document doc;
        doc.ParseStream(isw);
        if (doc.HasMember("Width"))
        {
            _w = doc["Width"].GetInt();
        }
        else
        {
            return false;
        }
        if (doc.HasMember("Height"))
        {
            _h = doc["Height"].GetInt();
        }
        else
        {
            return false;
        }
        std::cout << "Loading from config...\n" << "Window Width:" << _w << '\n'
            << "Window Height:" << _h << std::endl;
        return true;
    }
}



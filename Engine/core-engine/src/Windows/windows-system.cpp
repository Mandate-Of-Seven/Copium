/*!***************************************************************************************
\file			Windows/windows-system.h
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

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"

#include "Windows/windows-system.h"

namespace Copium::Windows
{

    void WindowsSystem::init()
    {
        init_system(1600, 900, "Copium");
    }

    void WindowsSystem::init_system(int _width, int _height, std::string _title)
    {
        windowWidth = _width;
        windowHeight = _height;
        title = _title;

        std::string config("Data\\config.json");
        load_config(config, _width, _height);

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
        if(!glfwWindowShouldClose(window))
            glfwPollEvents();
    }

    void WindowsSystem::draw()
    {
        // Printing to Windows Title Bar
        std::stringstream sstr;
        sstr << std::fixed << std::setprecision(2) << title << " | FPS: " << fps 
            << " | Resolution: " << windowWidth << " by " << windowHeight;
        glfwSetWindowTitle(window, sstr.str().c_str());

        glfwSwapBuffers(window);
    }

    void WindowsSystem::exit()
    {
        //glfwTerminate();
    }

    // Updates the system time
    void WindowsSystem::update_time(double _fpsInterval)
    {
        // get elapsed time (in seconds) between previous and current frames
        static double prev_time = glfwGetTime();
        double curr_time = glfwGetTime();
        delta_time = curr_time - prev_time;
        prev_time = curr_time;

        // fps calculations
        static double count = 0.0; // number of game loop iterations
        static double start_time = glfwGetTime();
        // get elapsed time since very beginning (in seconds) ...
        double elapsed_time = curr_time - start_time;

        ++count;

        // update fps at least every 10 seconds ...
        _fpsInterval = (_fpsInterval < 0.0) ? 0.0 : _fpsInterval;
        _fpsInterval = (_fpsInterval > 10.0) ? 10.0 : _fpsInterval;
        if (elapsed_time > _fpsInterval)
        {
            fps = count / elapsed_time;
            start_time = curr_time;
            count = 0.0;
            //std::cout << "FPS:" << fps << std::endl;
        }
    }

    void WindowsSystem::error_callback(int _error, char const * _description)
    {
#ifdef _DEBUG
        std::cerr << "GLFW error: " << _description << std::endl;
#endif
        (void) _error, _description;
    }

    void WindowsSystem::framebuffer_size_callback(GLFWwindow * _window, int _width, int _height)
    {
#ifdef _DEBUG
        PRINT("Framebuffer size getting called!!!");
#endif
        // Bean: This would be changed once we implement IMGUI
        // 
        // use the entire framebuffer as drawing region
        glViewport(0, 0, _width, _height);
        // later, if working in 3D, we'll have to set the projection matrix here ...
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



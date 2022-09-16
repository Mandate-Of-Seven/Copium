/*!***************************************************************************************
\file			windows-system.h
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
#include <windows-system.h>

namespace Copium
{
    // Bean: some reason the data is not stored in the memory...
    int WindowsSystem::screenWidth;
    int WindowsSystem::screenHeight;
    GLFWwindow * WindowsSystem::window;

    void WindowsSystem::init(int _width, int _height, std::string _title)
    {
        screenWidth = _width;
        screenHeight = _height;
        title = _title;

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
    }

    void WindowsSystem::update()
    {
        if(!glfwWindowShouldClose(window))
            glfwPollEvents();
    }

    void WindowsSystem::draw()
    {
        glfwSwapBuffers(window);
    }

    void WindowsSystem::exit()
    {
        glfwTerminate();
    }

    void WindowsSystem::error_callback(int _error, char const * _description)
    {
#ifdef _DEBUG
        std::cerr << "GLFW error: " << _description << std::endl;
#endif
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
    }

    /*********************************** DEPRECIATED ***********************************/

//    // static data members declared in GLHelper
    GLdouble GLHelper::fps;
    GLdouble GLHelper::delta_time;
//    /*  _________________________________________________________________________ */
//    /*! error_cb
//
//    @param int
//    GLFW error code
//
//    @parm char const*
//    Human-readable description of the code
//
//    @return none
//
//    The error callback receives a human-readable description of the error and
//    (when possible) its cause.
//    */
//    void GLHelper::error_cb(int error, char const * description)
//    {
//#ifdef _DEBUG
//        std::cerr << "GLFW error: " << description << std::endl;
//#endif
//    }
//
//    /*  _________________________________________________________________________ */
//    /*! fbsize_cb
//
//    @param GLFWwindow*
//    Handle to window that is being resized
//
//    @parm int
//    Width in pixels of new window size
//
//    @parm int
//    Height in pixels of new window size
//
//    @return none
//
//    This function is called when the window is resized - it receives the new size
//    of the window in pixels.
//    */
//    void GLHelper::fbsize_cb(GLFWwindow * ptr_win, int width, int height)
//    {
//#ifdef _DEBUG
//        std::cout << "fbsize_cb getting called!!!" << std::endl;
//#endif
//        // use the entire framebuffer as drawing region
//        glViewport(0, 0, width, height);
//        // later, if working in 3D, we'll have to set the projection matrix here ...
//    }

    /*  _________________________________________________________________________*/
    /*! update_time

    @param double
    fps_calc_interval: the interval (in seconds) at which fps is to be
    calculated

    This function must be called once per game loop. It uses GLFW's time functions
    to compute:
    1. the interval in seconds between each frame
    2. the frames per second every "fps_calc_interval" seconds
    */
    void GLHelper::update_time(double fps_calc_interval)
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
        fps_calc_interval = (fps_calc_interval < 0.0) ? 0.0 : fps_calc_interval;
        fps_calc_interval = (fps_calc_interval > 10.0) ? 10.0 : fps_calc_interval;
        if (elapsed_time > fps_calc_interval)
        {
            GLHelper::fps = count / elapsed_time;
            start_time = curr_time;
            count = 0.0;
            std::cout << "FPS:" << GLHelper::fps << std::endl;
        }
    }
}



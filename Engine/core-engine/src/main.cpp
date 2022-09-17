/*!***************************************************************************************
\file			main.cpp
\project
\author			Everyone

\par			Course: GAM200
\par			Section:
\date			16/09/2022

\brief
    This file contains beginning of the core-engine.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

// Extension loader library's header must be included before GLFW's header!!!
#include "pch.h"

//PRECOMPILED HEADERS(Commonly used external libraries)
#include "windows-system.h"
#include "windows-input.h"
#include "editor-layer.h"
#include "scripting-system.h"
#include "scripting.h"
#include "logging.h"

//State Manager
#include "state-manager.h"
#include "scene-manager.h"

//Systems
#include "message-system.h"
#include "SAMPLE_RECEIVER.h"
#include "serializer.h"
#include "frameratecontroller.h"
#include "graphics.h"

// Bean:: Remove after including namespace
using namespace Copium;

namespace
{
    // Our state
    float recompileTimer = 0;
    Copium::Message::MessageSystem messageSystem;
    //Copium::Scripting::ScriptingSystem scriptingSystem;
}

Input * Input::inputInstance = new WindowsInput();
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

// Function declarations
static void draw();
static void update();
static void init();
static void cleanup();

void quitKeyCallback(GLFWwindow*, int, int, int, int);

/***************************************************************************/
/*!
\brief
    Indicates how the program existed. Normal exit is signaled by a return 
    value of 0. Abnormal termination is signaled by a non-zero return value.
    Note that the C++ compiler will insert a return 0 statement if one is 
    missing.
*/
/**************************************************************************/
int main() 
{
    init();

    init_statemanager(esActive);

    glfwSetKeyCallback(windowsSystem.get_window(), quitKeyCallback);
    //glfwSetKeyCallback(GLHelper::ptr_window, Input::keyCallback);
    //glfwSetMouseButtonCallback(GLHelper::ptr_window, Input::mousebuttonCallback);
    //glfwSetScrollCallback(GLHelper::ptr_window, Input::mousescrollCallback);
    //glfwSetCursorPosCallback(GLHelper::ptr_window, Input::mouseposCallback);

    // Enable run-time memory check for debug purposes 
    #if defined(DEBUG) | defined(_DEBUG)
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    #endif

    Copium::Message::DUMMY_RECEIVER dummy12;
    Copium::Message::DUMMY_RECEIVER dummy122;
    messageSystem.init();
    messageSystem.dispatch(Copium::Message::MESSAGE_TYPE::MT_MOUSE_CLICKED);
    SceneManager SM;
    FrameRateController frc(100.0);
    std::string str = "blah";
    SceneSandbox* sandboxScene = new SceneSandbox(str);

    //scriptingSystem.init();
    //ScriptComponent *yolo;
    //yolo = new ScriptComponent("PlayerMovement");
    //delete yolo;

    // Engine Loop
    while (!glfwWindowShouldClose(windowsSystem.get_window()) && esCurrent != esQuit) 
    {
        SM.add_scene(sandboxScene);
        //std::cout << "Number of scenes: " << SM.get_scenecount() << std::endl;
        SM.change_scene(0);

        if (esCurrent == esActive) 
        {
            std::cout << "scene active" << std::endl;
            while (SM.current != gsQuit) 
            {
                //If game state is not set to restart, update the state manager and load in the next game state
                if (SM.current == gsRestart) 
                {
                    SM.current = SM.previous;
                    SM.next = SM.current;
                }
                else 
                {
                    //checks for change in scene
                    SM.load_scene();             //LOAD STATE
                }

                SM.init_scene();                 //INIT STATE

                while (SM.current == SM.next) 
                {
                    frc.start();

                    SM.update_scene();         //UPDATE STATE         
                    SM.draw_scene();           //DRAW STATE

                    update();

                    if (esCurrent == esQuit)
                        SM.change_scene(gsQuit);

                    draw();
                    frc.end();
                }

                SM.free_scene();                 //FREE STATE

                if (SM.next != gsRestart)
                    SM.unload_scene();           //UNLOAD STATE

                SM.previous = SM.current;
                SM.current = SM.next;
            }
        }
    }

    cleanup();
    //delete sandboxScene;
    std::cout << "Engine Closing...\n";
}

/***************************************************************************/
/*!
\brief
    The OpenGL context initialization stuff is abstracted away in 
    windows-system. 
*/
/**************************************************************************/
static void init()
{
    // Bean: This should be handles by ISystem (with regards to the "System" itself)
    windowsSystem.init(1600, 900, "Copium");

    // Bean: This initialises the imgui, which i think should also be handled by ISystem
    Editor::editor.init();

    Log::init();
    Console_Critical("Test 1");
    Console_Error("Test 2");
    Console_Warn("What happens");
    Console_Info("Hello");
    Console_Trace("Goodbye");

    //spdlog::info("File test");
    //File_Warn("Hello{}",3);
}

/***************************************************************************/
/*!
\brief
    Uses GLHelper::GLFWWindow* to get handle to OpenGL context.
    For now, there are no objects to animate nor keyboard, mouse button 
    click, mouse movement, and mouse scroller events to be processed.
*/
/**************************************************************************/
static void update()
{
    // Bean: This should be handles by ISystem
    windowsSystem.update();

    // Bean: This should be handles by ISystem
    Editor::editor.update();

    //testing
    //auto [x, y] = Input::getMousePosition();
    //std::cout << "Mouse Pos:" << x << "," << y << std::endl;
    //std::cout<< "Is Shift Button Held:" << Input::isMouseButtonPressed(GLFW_KEY_LEFT_SHIFT) << std::endl;
}

/***************************************************************************/
/*!
\brief
    Call application to draw and then swap front and back frame buffers ...
    Uses GLHelper::GLFWWindow* to get handle to OpenGL context.
*/
/**************************************************************************/
static void draw() 
{
    // Bean: This should be handles by ISystem
    Editor::editor.draw();
    
    // Bean: This should be handles by ISystem
    windowsSystem.draw();
}

/***************************************************************************/
/*!
\brief
    Return allocated resources for window and OpenGL context thro GLFW back
    to system.
*/
/**************************************************************************/
void cleanup() 
{
    // Bean: This should be handles by ISystem
    Editor::editor.exit();

    Input::destroy();
}

void quitKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) 
    {

        change_enginestate(esQuit);
        std::cout << "Q was pressed" << std::endl;
    }
}


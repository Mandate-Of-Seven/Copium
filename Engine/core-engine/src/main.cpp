/*!***************************************************************************************
\file			main.cpp
\project
\author			Everyone

\par			Course: GAM200
\par			Section:
\date			16/09/2022

\brief
    This file contains beginning of the core-engine.

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#include "pch.h"

//Systems
#include "CopiumCore/copium-core.h"
#include "Windows/windows-input.h"
#include "Editor/editor-system.h"
#include <Windows/windows-system.h>
#include <Debugging/frame-rate-controller.h>

//State Manager
#include "SceneManager/state-manager.h"

namespace
{
    // Our state
    //float recompileTimer = 0;
    Copium::CopiumCore& copiumCore{ *Copium::CopiumCore::Instance()};
}

ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

// Function declarations
static void draw();
static void update();
static void init();
static void cleanup();

void quitEngine(GLFWwindow* window);
bool load_config(std::string& _filename, GLint& _w, GLint& _h);

/*                                                      function definitions
----------------------------------------------------------------------------- */
/*  _________________________________________________________________________ */
/*! main

@param none

@return int

Indicates how the program existed. Normal exit is signaled by a return value of
0. Abnormal termination is signaled by a non-zero return value.
Note that the C++ compiler will insert a return 0 statement if one is missing.
*/
/**************************************************************************/
int main() 
{
    init_statemanager(esActive);
    // Enable run-time memory check for debug purposes 
    #if defined(DEBUG) | defined(_DEBUG)
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    #endif

    init();
    copiumCore.init();
    MyFrameRateController.init(60);
    glfwSetWindowCloseCallback(MyWindowSystem.get_window(), quitEngine);



    // Engine Loop
    while (esCurrent != esQuit)
    {

        if (esCurrent == esActive) 
        {
            MyFrameRateController.start();
            copiumCore.update();
            update();

            draw();
            MyFrameRateController.end();
        }
    }
    copiumCore.exit();
    cleanup();
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

}

/***************************************************************************/
/*!
\brief
    Engines update loop
*/
/**************************************************************************/
static void update()
{   
    if (MyInputSystem.is_key_pressed(GLFW_KEY_Z))//undo
    {
        if (!MyEditorSystem.get_commandmanager()->undoStack.empty())
        {
            Copium::UndoRedo::Command* temp = MyEditorSystem.get_commandmanager()->undoStack.top();
            //temp->printCommand();
            MyEditorSystem.get_commandmanager()->undoStack.top()->Undo(&MyEditorSystem.get_commandmanager()->redoStack);
            MyEditorSystem.get_commandmanager()->undoStack.pop();
            delete temp;
        }
        else
        {
            PRINT("No undo commands left");
            return;
        }
    }

    if (MyInputSystem.is_key_pressed(GLFW_KEY_X) )//redo
    {
        
        if (!MyEditorSystem.get_commandmanager()->redoStack.empty())
        {
            Copium::UndoRedo::Command* temp = MyEditorSystem.get_commandmanager()->redoStack.top();
            MyEditorSystem.get_commandmanager()->redoStack.top()->Redo(&MyEditorSystem.get_commandmanager()->undoStack);
            MyEditorSystem.get_commandmanager()->redoStack.pop();
            delete temp;
        }
        else
        {
            PRINT("No redo commands left");
            return;
        }
    }

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
    MyEditorSystem.draw();
    MyWindowSystem.draw();
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
    glfwTerminate();
}

/***************************************************************************/
/*!
\brief
    Sets the state of the engine to quit
\param window
    The window reference
*/
/**************************************************************************/
void quitEngine(GLFWwindow* window)
{
    change_enginestate(esQuit);
}

/***************************************************************************/
/*!
\brief
    Load the config to set width and height to

\param _filename
    Filename of config files

\param _w
    Width of window

\param _h
    Height of window
*/
/**************************************************************************/
bool load_config(std::string& _filename, GLint& _w, GLint& _h)
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
    else {
        return false;
    }
    if (doc.HasMember("Height"))
    {
        _h = doc["Height"].GetInt();
    }
    else {
        return false;
    }
    std::cout << "Loading from config...\n" << "Window Width:" << _w << '\n'
        << "Window Height:" << _h << std::endl;

    return true;
}



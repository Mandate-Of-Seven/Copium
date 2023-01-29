/*!***************************************************************************************
\file			main.cpp
\project
\author			Everyone

\par			Course: GAM200
\par			Section:
\date			16/09/2022

\brief
    This file contains beginning of the core-engine.

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#include "pch.h"

//Systems
#include "Windows/windows-system.h"
#include "Windows/windows-input.h"
#include "Editor/editor-system.h"
#include "Scripting/scripting-system.h"
#include "Scripting/logic-system.h"
#include "Editor/editor-consolelog.h"
#include "GameObject/Components/script-component.h"
#include "Debugging/logging-system.h"
#include "Audio/sound-system.h"
#include "SceneManager/scene-manager.h"
#include "GameObject/Components/component.h"
#include "GameObject/Components/renderer-component.h"
#include "Editor/editor-undoredo.h"
#include "GameObject/Components/ui-components.h"

//State Manager
#include "SceneManager/state-manager.h"

#include "Editor/inspector.h"
#include "CopiumCore/copium-core.h"
#include "Debugging/frame-rate-controller.h"
namespace
{
    // Our state
    float recompileTimer = 0;
    Copium::CopiumCore& copiumCore{ *Copium::CopiumCore::Instance()};
    Copium::SoundSystem& soundSystem{ *Copium::SoundSystem::Instance()};
    Copium::InputSystem& inputSystem { *Copium::InputSystem::Instance()};
    Copium::WindowsSystem* windowsSystem = Copium::WindowsSystem::Instance();
    Copium::SceneManager* sceneManager = Copium::SceneManager::Instance();
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
    glfwSetWindowCloseCallback(windowsSystem->get_window(), quitEngine);



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
    Uses GLHelper::GLFWWindow* to get handle to OpenGL context.
    For now, there are no objects to animate nor keyboard, mouse button 
    click, mouse movement, and mouse scroller events to be processed.
*/
/**************************************************************************/
static void update()
{   
    if (inputSystem.is_key_pressed(GLFW_KEY_Z))//undo
    {
        if (!Copium::EditorSystem::Instance()->get_commandmanager()->undoStack.empty())
        {
            Copium::UndoRedo::Command* temp = Copium::EditorSystem::Instance()->get_commandmanager()->undoStack.top();
            //temp->printCommand();
            Copium::EditorSystem::Instance()->get_commandmanager()->undoStack.top()->Undo(&Copium::EditorSystem::Instance()->get_commandmanager()->redoStack);
            Copium::EditorSystem::Instance()->get_commandmanager()->undoStack.pop();
            delete temp;
        }
        else
        {
            PRINT("No undo commands left");
            return;
        }
    }

    if (inputSystem.is_key_pressed(GLFW_KEY_X) )//redo
    {
        
        if (!Copium::EditorSystem::Instance()->get_commandmanager()->redoStack.empty())
        {
            Copium::UndoRedo::Command* temp = Copium::EditorSystem::Instance()->get_commandmanager()->redoStack.top();
            Copium::EditorSystem::Instance()->get_commandmanager()->redoStack.top()->Redo(&Copium::EditorSystem::Instance()->get_commandmanager()->undoStack);
            Copium::EditorSystem::Instance()->get_commandmanager()->redoStack.pop();
            delete temp;
        }
        else
        {
            PRINT("No redo commands left");
            return;
        }
    }

    if (inputSystem.is_doubleclicked())
    {
        std::cout << "DOUBLE CLICK \n\n\n\n";
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
    Copium::EditorSystem::Instance()->draw();
    Copium::WindowsSystem::Instance()->draw();
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



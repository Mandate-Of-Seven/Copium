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
#include "SceneManager/sm.h"
#include "GameObject/Components/component.h"
#include "GameObject/Components/renderer-component.h"
#include "Editor/editor-undoredo.h"
#include "GameObject/Components/ui-components.h"

//State Manager
#include "SceneManager/state-manager.h"
#include "SceneManager/scene-manager.h"
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
    Copium::NewSceneManager* sceneManager = Copium::NewSceneManager::Instance();
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

    Copium::SceneManager SM;
    MyFrameRateController.init(60);
    std::string str = "blah";
    SceneSandbox* sandboxScene = new SceneSandbox(str);
    glfwSetWindowCloseCallback(windowsSystem->get_window(), quitEngine);

    // Engine Loop
    while (esCurrent != esQuit)
    {
        SM.add_scene(sandboxScene);
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
                    MyFrameRateController.start();

                    SM.update_scene();         //UPDATE STATE
                    SM.draw_scene();           //DRAW STATE
                    copiumCore.update();
                    update();
                    //button.update();
                    if (esCurrent == esQuit)
                        SM.change_scene(gsQuit);

                    draw();
                    MyFrameRateController.end();
                }

                SM.free_scene();                 //FREE STATE

                if (SM.next != gsRestart)
                    SM.unload_scene();           //UNLOAD STATE

                SM.previous = SM.current;
                SM.current = SM.next;
            }
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



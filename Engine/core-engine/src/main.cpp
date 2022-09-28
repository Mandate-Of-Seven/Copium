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

// Extension loader library's header must be included before GLFW's header!!!
#include "pch.h"

//PRECOMPILED HEADERS(Commonly used external libraries)
#include "Editor/editor-system.h"
#include "Scripting/scripting-system.h"
#include "Scripting/scripting.h"
#include "Windows/windows-system.h"
#include "Windows/windows-input.h"
#include "Debugging/logging.h"
#include "Editor/ConsoleLog.h"

//State Manager
#include "SceneManager/state-manager.h"
#include "SceneManager/scene-manager.h"

//Systems
#include "CopiumCore/copium-core.h"
#include "Messaging/SAMPLE_RECEIVER.h"
#include "Debugging/frame-rate-controller.h"
namespace
{
    // Our state
    float recompileTimer = 0;
    Copium::CopiumCore& copiumCore{ *Copium::CopiumCore::Instance()};
    Copium::Message::MessageSystem& messageSystem{ *Copium::Message::MessageSystem::Instance() };
}

Input * Input::inputInstance = new WindowsInput();
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

// Function declarations
static void draw();
static void update();
static void init();
static void cleanup();

void quitEngine();
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
    

    Copium::Windows::WindowsSystem* windowsSystem = Copium::Windows::WindowsSystem::Instance();

    glfwSetKeyCallback(windowsSystem->get_window(), Input::key_callback);

    SceneManager SM;
    FrameRateController frc(100.0);
    std::string str = "blah";
    SceneSandbox* sandboxScene = new SceneSandbox(str);
    //ScriptComponent *yolo;
    //yolo = new ScriptComponent("PlayerMovement");
    //delete yolo;

    // Engine Loop
    while (!glfwWindowShouldClose(windowsSystem->get_window()) && esCurrent != esQuit)
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
                    copiumCore.update();
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

    copiumCore.exit();
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

    Input::get_input_instance()->init();
    Copium::Log::init();

    SoundSystem::init();
    SoundSystem::CreateSound("./Assets/sounds/reeling.wav", SoundAlias::reeling);
    SoundSystem::SetVolume(reeling, 0.3f);
    SoundSystem::CreateSound("./Assets/sounds/zap.wav", SoundAlias::zap);
    SoundSystem::SetVolume(zap, 0.3f);

    

    //Uncomment to test asserts
    //COPIUM_ASSERT(1+1==2,"Asserts are working as intended");
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
    if (Input::is_key_pressed(GLFW_KEY_1))
    {
        SoundSystem::Play(zap, true, false);
        std::cout << "Zap sound is being played\n";
    }
    if (Input::is_key_pressed(GLFW_KEY_2))
    {
        SoundSystem::Play(reeling, true, false);
        std::cout << "Reeling sound is being played\n";
    }
    quitEngine();
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
    Copium::Editor::EditorSystem::Instance()->draw();
    Copium::Windows::WindowsSystem::Instance()->draw();
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
    glfwTerminate(); // Bean: should be in WindowSystem exit once the 
                     //         exit system is called in reverse order
    Input::destroy();
}

void quitEngine() 
{
    if (Input::is_key_pressed(GLFW_KEY_Q)) 
    {

        change_enginestate(esQuit);
        std::cout << "Q was pressed" << std::endl;
    }
}

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


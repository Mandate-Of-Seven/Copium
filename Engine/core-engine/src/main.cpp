/*!
@file    main.cpp
@author  pghali@digipen.edu
@date    10/11/2016

This file uses functionality defined in types GLHelper and GLApp to initialize 
an OpenGL context and implement a game loop.

*//*__________________________________________________________________________*/

/*                                                                   includes
----------------------------------------------------------------------------- */
// Extension loader library's header must be included before GLFW's header!!!
#include "pch.h"
#include <glhelper.h>
#include <glapp.h>

//PRECOMPILED HEADERS(Commonly used external libraries)

#include "inspector.h"
#include "windows-input.h"
#include "scriptingEngine.h"
#include "scripting.h"

//State Manager
#include "state-manager.h"
#include "scene-manager.h"
#include <thread>

//Systems
#include "message-system.h"
#include "SAMPLE_RECEIVER.h"
#include "serializer.h"
#include "frameratecontroller.h"

namespace
{
    // Our state
    bool show_demo_window = true;
    float recompileTimer = 0;
    Copium::Message::MessageSystem messageSystem;
}

Input* Input::inputInstance = new WindowsInput();


/*                                                   type declarations
----------------------------------------------------------------------------- */

/*                                                      function declarations
----------------------------------------------------------------------------- */
static void draw();
static void update();
static void init();
static void cleanup();

void quitKeyCallback(GLFWwindow*, int, int, int, int);


ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);



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
int main() {
    init();
    init_statemanager(esActive);
    glfwSetKeyCallback(GLHelper::ptr_window, quitKeyCallback);

    // Enable run-time memory check for debug purposes 
    #if defined(DEBUG) | defined(_DEBUG)
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    #endif
    Copium::Message::DUMMY_RECEIVER dummy12;
    Copium::Message::DUMMY_RECEIVER dummy122;
    messageSystem.init();
    messageSystem.dispatch(Copium::Message::MESSAGE_TYPE::MOUSE_CLICKED);
    SceneManager SM;
    FrameRateController frc(100.0);
    std::string str = "blah";
    SceneSandbox* sandboxScene = new SceneSandbox(str);

    Copium::ScriptingEngine::init();
    std::thread recompileThread(Copium::ScriptingEngine::tryRecompileDll);
    //ScriptComponent *yolo;
    //yolo = new ScriptComponent("PlayerMovement");
    //delete yolo;
    // Engine Loop
    while (!glfwWindowShouldClose(GLHelper::ptr_window) && esCurrent != esQuit) {

        SM.add_scene(sandboxScene);
        //std::cout << "Number of scenes: " << SM.get_scenecount() << std::endl;
        SM.change_scene(0);

        if (esCurrent == esActive) {
            std::cout << "scene active" << std::endl;
            while (SM.current != gsQuit) {
                //If game state is not set to restart, update the state manager and load in the next game state
                if (SM.current == gsRestart) {
                    SM.current = SM.previous;
                    SM.next = SM.current;
                }
                else 
                {
                    //checks for change in scene
                    SM.load_scene();             //LOAD STATE
                }


                SM.init_scene();                 //INIT STATE

                while (SM.current == SM.next) {

                    frc.start();

                    SM.update_scene();         //UPDATE STATE         
                    SM.draw_scene();           //DRAW STATE
                    update();
                    Copium::ScriptingEngine::trySwapDll(recompileThread);
                    //float gcHeapSize = (float)mono_gc_get_heap_size();
                    //float gcUsageSize = (float)mono_gc_get_used_size();
                    //PRINT("GC Heap Info (Used/Avaliable): " << (gcUsageSize / 1024.0f) << " " << gcHeapSize / 1024.0f);
                    //Check for engine close
                    if (esCurrent == esQuit) {
                        SM.change_scene(gsQuit);
                    }

                    draw();

                    frc.end();
                    
                }



                SM.free_scene();                 //FREE STATE

                if (SM.next != gsRestart) {

                    SM.unload_scene();           //UNLOAD STATE
                }
                SM.previous = SM.current;
                SM.current = SM.next;




            }
        }
    }
    #if _DEBUG
    recompileThread.detach();
    #else
    recompileThread.join();
    #endif

    // Part 3
    cleanup();
    std::cout << sandboxScene << std::endl;

    std::cout << "Engine Closing...\n";
}

/*  _________________________________________________________________________ */
/*! update
@param none
@return none

Uses GLHelper::GLFWWindow* to get handle to OpenGL context.
For now, there are no objects to animate nor keyboard, mouse button click,
mouse movement, and mouse scroller events to be processed.
*/
static void update() {
    // Part 1
    glfwPollEvents();

    ;

    //testing
    //auto [x, y] = Input::getMousePosition();
    //std::cout << "Mouse Pos:" << x << "," << y << std::endl;
    //std::cout<< "Is Shift Button Held:" << Input::isMouseButtonPressed(GLFW_KEY_LEFT_SHIFT) << std::endl;
  
    // Part 2
    //GLHelper::update_time(1.0);
  
    // Part 3
    //GLApp::update();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);


    Window::Inspector::update();
    ImGui::EndFrame();
}

/*  _________________________________________________________________________ */
/*! draw
@param none
@return none

Call application to draw and then swap front and back frame buffers ...
Uses GLHelper::GLFWWindow* to get handle to OpenGL context.
*/
static void draw() {
    // Part 1


    //GLApp::draw();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Part 2: swap buffers: front <-> back
    glfwSwapBuffers(GLHelper::ptr_window);
}

/*  _________________________________________________________________________ */
/*! init
@param none
@return none

The OpenGL context initialization stuff is abstracted away in GLHelper::init.
The specific initialization of OpenGL state and geometry data is
abstracted away in GLApp::init
*/
static void init() {
    if (!GLHelper::init(1600, 900, "Engine")) {
        std::cout << "Unable to create OpenGL context" << std::endl;
        std::exit(EXIT_FAILURE);
    }


    //imgui
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(GLHelper::ptr_window, true);
    ImGui_ImplOpenGL3_Init("#version 330");


    Window::Inspector::init();
    Window::Inspector::selectedGameObject = new GameObject();

    // Part 2
    GLHelper::print_specs();

    // Part 3
    //GLApp::init();
}

/*  _________________________________________________________________________ */
/*! cleanup
@param none
@return none

Return allocated resources for window and OpenGL context thro GLFW back
to system.
Return graphics memory claimed through 
*/
void cleanup() 
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    // Part 1
    GLApp::cleanup();
    Copium::ScriptingEngine::shutdown();
    // Part 2
    GLHelper::cleanup();
    delete Window::Inspector::selectedGameObject;
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


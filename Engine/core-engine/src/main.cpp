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
#include <glhelper.h>
#include <glapp.h>
#include <iostream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "inspector.h"
#include "input.h"

//State Manager
#include "statemanager.h"
#include "scenemanager.h"



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

  // Part 1
  init();
  init_statemanager(esActive);
  glfwSetKeyCallback(GLHelper::ptr_window, quitKeyCallback);
  
  //imgui
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(GLHelper::ptr_window, true);
  ImGui_ImplOpenGL3_Init("#version 330");


  Window::Inspector::init();
  Window::Inspector::selectedGameObject = new GameObject();

  Window::Inspector::selectedGameObject->addComponent(new Component());

  // Our state
  bool show_demo_window = true;
  bool show_another_window = false;

    // Enable run-time memory check for debug purposes 
    #if defined(DEBUG) | defined(_DEBUG)
        _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    #endif

    SceneManager SM;
    std::string str = "blah";
    SceneSandbox* sandboxScene = new SceneSandbox(str);
    SM.addScene(sandboxScene);
    std::cout << "Number of scenes: " << SM.getSceneCount() << std::endl;
    SM.changeScene(0);


  // Part 2
  // Engine Loop
  while (!glfwWindowShouldClose(GLHelper::ptr_window) && esCurrent != esQuit) {

    if (esCurrent == esActive) {
        std::cout << "scene active" << std::endl;

        while (SM.current != gsQuit) {
            //If game state is not set to restart, update the state manager and load in the next game state
            if (SM.current == gsRestart) {
                SM.current = SM.previous;
                SM.next = SM.current;
            }
            else {
                //checks for change in scene
                SM.loadScene();             //LOAD STATE
            }    
        
                                   
            SM.initScene();                 //INIT STATE

            while (SM.current == SM.next) {

                                 
                                 
                SM.updateScene();         //UPDATE STATE         
                SM.drawScene();           //DRAW STATE

                update();

                //Check for engine close
                if (esCurrent == esQuit) {
                    SM.changeScene(gsQuit);
                }


                // Start the Dear ImGui frame
                ImGui_ImplOpenGL3_NewFrame();
                ImGui_ImplGlfw_NewFrame();
                ImGui::NewFrame();

                // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
                if (show_demo_window)
                    ImGui::ShowDemoWindow(&show_demo_window);

                // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
                {
                    static float f = 0.0f;
                    static int counter = 0;

                    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

                    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
                    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
                    ImGui::Checkbox("Another Window", &show_another_window);

                    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

                    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                        counter++;
                    ImGui::SameLine();
                    ImGui::Text("counter = %d", counter);

                    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                    ImGui::End();
                }

                // 3. Show another simple window.
                if (show_another_window)
                {
                    ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
                    ImGui::Text("Hello from another window!");
                    if (ImGui::Button("Close Me"))
                        show_another_window = false;
                    ImGui::End();
                }

                // Part 2b
                draw();
            }

                                   
            SM.freeScene();                 //FREE STATE

            if (SM.next != gsRestart) {
                                
                SM.unloadScene();           //UNLOAD STATE
            }
            SM.previous = SM.current;
            SM.current = SM.next;
            
        }


    }

    
        update();
        

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        Window::Inspector::update();

        // Part 2b
        draw();
        
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  // Part 3
  cleanup();

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


  //testing
  //auto [x, y] = Input::getMousePosition();
  //std::cout << "Mouse Pos:" << x << "," << y << std::endl;
  //std::cout<< "Is Shift Button Held:" << Input::isMouseButtonPressed(GLFW_KEY_LEFT_SHIFT) << std::endl;
  
  // Part 2
  GLHelper::update_time(1.0);
  
  // Part 3
  //GLApp::update();
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
  // Part 1
  if (!GLHelper::init(1920, 1080, "Engine")) {
    std::cout << "Unable to create OpenGL context" << std::endl;
    std::exit(EXIT_FAILURE);
  }

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
void cleanup() {
  // Part 1
  GLApp::cleanup();

  // Part 2
  GLHelper::cleanup();
}

void quitKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {

        change_enginestate(esQuit);
        

        std::cout << "Q was pressed" << std::endl;
    }
}


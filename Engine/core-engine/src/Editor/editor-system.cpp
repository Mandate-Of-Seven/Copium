/*!***************************************************************************************
\file			editor-system.cpp
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			16/09/2022

\brief
	This file holds the definition of functions for the editor.

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"
#include "Windows/windows-system.h"
#include "Editor/editor-system.h"
#include "GameObject/game-object.h"
#include "Editor/inspector.h"
#include "Editor/editor-consolelog.h"
#include "Editor/editor-hierarchy-list.h"
#include "Windows/windows-utils.h"
#include "SceneManager/state-manager.h"

#include "CopiumCore/copium-core.h"

namespace Copium
{
	// Our state
	bool show_demo_window = false;
	CopiumCore& copiumCore{ *CopiumCore::Instance() };


	void EditorSystem::init()
	{
		systemFlags |= FLAG_RUN_ON_EDITOR | FLAG_RUN_ON_PLAY;
		//PRINT("FLAGS: " << systemFlags);
		WindowsSystem* windowsSystem = WindowsSystem::Instance();

		//imgui
		ImGui::CreateContext();
		ImGuiIO & io = ImGui::GetIO(); (void) io;

		// Only move window from title bar
		io.ConfigWindowsMoveFromTitleBarOnly = true;

		// Global Font Size
		io.FontGlobalScale = 0.6f;
		
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(windowsSystem->get_window(), true);
		ImGui_ImplOpenGL3_Init("#version 330");
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		ImGui::GetIO().ConfigDockingWithShift = true;
		Window::Inspector::init();
		Window::EditorConsole::init();
		Window::Hierarchy::init();

		sceneView.init();
		contentBrowser.init();
		
		// Initialize a new editor camera
		camera.init((float) sceneView.get_width(), (float) sceneView.get_height());
	}

	void EditorSystem::update()
	{
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		//Dockspace
		{
            static bool p_open = true;
            static bool opt_fullscreen = true;
            static bool opt_padding = false;
            static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

            // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
            // because it would be confusing to have two docking targets within each others.
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
            if (opt_fullscreen)
            {
                const ImGuiViewport* viewport = ImGui::GetMainViewport();
                ImGui::SetNextWindowPos(viewport->WorkPos);
                ImGui::SetNextWindowSize(viewport->WorkSize);
                ImGui::SetNextWindowViewport(viewport->ID);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
                window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
                window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
            }
            else
            {
                dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
            }

            // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
            // and handle the pass-thru hole, so we ask Begin() to not render a background.
            if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
                window_flags |= ImGuiWindowFlags_NoBackground;

            // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
            // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
            // all active windows docked into it will lose their parent and become undocked.
            // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
            // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
            if (!opt_padding)
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
            ImGui::Begin("DockSpace Demo", &p_open, window_flags);
            if (!opt_padding)
                ImGui::PopStyleVar();

            if (opt_fullscreen)
                ImGui::PopStyleVar(2);

            // Submit the DockSpace
            ImGuiIO& io = ImGui::GetIO();
			ImGuiID dockspace_id = 0;
            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
            {
                dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }

			//top menu bar
			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("File"))
				{
					// Disabling fullscreen would allow the window to be moved to the front of other windows, 
					// which we can't undo at the moment without finer window depth/z control.
					//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);1
					if (ImGui::MenuItem("New", "Ctrl+N"))
					{
						//create new scene
					}

					if (ImGui::MenuItem("Open...", "Ctrl+O"))
					{
						//open scene
						std::string filepath = FileDialogs::open_file("Copium Scene (*.json)\0*.json\0");
						if (!filepath.empty())
						{
							std::cout << filepath << std::endl;


							if (Copium::NewSceneManager::Instance()->get_current_scene() != nullptr)
							{
								std::cout << "change scene\n";
								Copium::NewSceneManager::Instance()->change_scene(filepath);
							}
							else {
								if (Copium::NewSceneManager::Instance()->load_scene(filepath))
									std::cout << "loading success\n";
								else
									std::cout << "loading fail\n";
							}

						}
						else
						{
							std::cout << "file failed to open\n";
						}

					}

					if (ImGui::MenuItem("Save", "Ctrl+S"))
					{
						//save scene
						NewSceneManager::Instance()->save_scene();
					}

					if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					{
						if (Copium::NewSceneManager::Instance()->get_current_scene())
						{
							//save scene as
							std::string filepath = FileDialogs::save_file("Copium Scene (*.json)\0.json\0");
							std::cout << filepath << std::endl;
							Copium::NewSceneManager::Instance()->save_scene(filepath);
						}
						else
						{
							PRINT("There is no scene to save...\n");
						}

					}

					if (ImGui::MenuItem("Exit"))
					{

						//exit game engine
						change_enginestate(EngineState::esQuit);
						std::cout << "Copium has been huffed, Engine shutting down" << std::endl;

					}

					ImGui::EndMenu();
				}

				if (ImGui::BeginMenu("Preview"))
				{
					if (ImGui::MenuItem("Play Scene"))
					{
						printf("Starting scene\n");
						NewSceneManager::Instance()->startPreview();
						copiumCore.toggle_inplaymode();
					}
					if (ImGui::MenuItem("Stop Scene"))
					{
						NewSceneManager::Instance()->endPreview();
						copiumCore.toggle_inplaymode();
					}

					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

            //Call all the editor layers updates here
            Window::Inspector::update();
            Window::EditorConsole::update();
			Window::Hierarchy::update();
            sceneView.update();
			contentBrowser.update();


			// demo update
			if (show_demo_window)
				ImGui::ShowDemoWindow(&show_demo_window);


            // Editor Camera
            camera.update();

            ImGui::End();
		}

		
		ImGui::EndFrame();
	}

	void EditorSystem::draw()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void EditorSystem::exit()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		Window::Inspector::exit();
		Window::Inspector::selectedGameObject = nullptr;
		sceneView.exit();
		contentBrowser.exit();
	}

	void EditorSystem::imguiConsoleAddLog(std::string value)
	{
		std::cout << value << "\n";
		Window::EditorConsole::editorLog.add_logEntry(value);
	}
}

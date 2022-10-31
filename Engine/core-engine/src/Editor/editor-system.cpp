/*!***************************************************************************************
\file			editor-layer.cpp
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
#include "Editor/editor-sceneview.h"
#include "GameObject/game-object.h"
#include "Editor/inspector.h"
#include "Editor/editor-consolelog.h"
#include "Editor/editor-hierarchy-list.h"

namespace Copium
{
	// Our state
	bool show_demo_window = true;

	void EditorSystem::init()
	{
		Copium::WindowsSystem* windowsSystem = Copium::WindowsSystem::Instance();

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
					}

					if (ImGui::MenuItem("Save", "Ctrl+S"))
					{
						//save scene
						Copium::NewSceneManager::Instance()->save_scene();
					}

					if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					{
						//save sceen as
					}

					if (ImGui::MenuItem("Exit"))
					{
						//exit game engine
					}

					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			//ImGui::DockBuilderRemoveNode(dockspace_id);
			//ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
			//ImGui::DockBuilderSetNodeSize(dockspace_id, { 1600,900 });

			//ImGuiID mainID = dockspace_id;
			//ImGuiID left = ImGui::DockBuilderSplitNode(mainID, ImGuiDir_Left, 0.2f, NULL, &mainID);
			//ImGuiID btm = ImGui::DockBuilderSplitNode(mainID, ImGuiDir_Down, 0.50f, NULL, &mainID);
			//ImGui::DockBuilderDockWindow("Console Log", btm);
			//ImGui::DockBuilderDockWindow("Inspector", left);
			//ImGui::DockBuilderFinish(mainID);

            //Call all the editor layers updates here
            Window::Inspector::update();
            Window::EditorConsole::update();
			Window::Hierarchy::update();
            sceneView.update();

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
	}
}

/*!***************************************************************************************
\file			editor-layer.cpp
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			16/09/2022

\brief
	This file holds the definition of functions for the editor.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"
#include "Windows/windows-system.h"
#include "Editor/editor-system.h"
#include "Editor/editor-sceneview.h"
#include "GameObject/game-object.h"
#include "Editor/inspector.h"
#include "Editor/ConsoleLog.h"
#include "Editor/editor-hierarchy-list.h"

namespace Copium::Editor
{
	// Our state
	bool show_demo_window = true;

	void EditorSystem::init()
	{
		Copium::Windows::WindowsSystem* windowsSystem = Copium::Windows::WindowsSystem::Instance();

		//imgui
		ImGui::CreateContext();
		ImGuiIO & io = ImGui::GetIO(); (void) io;

		// Only move window from title bar
		io.ConfigWindowsMoveFromTitleBarOnly = true;
		
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(windowsSystem->get_window(), true);
		ImGui_ImplOpenGL3_Init("#version 330");

		Window::Inspector::init();
		Window::EditorConsole::init();
		Window::Hierarchy::init();

		sceneView.init();
		
		// Initialize a new editor camera
		float aspectRatio = sceneView.get_width() / (float)sceneView.get_height();
		camera.init(aspectRatio);
	}

	void EditorSystem::update()
	{
		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		
		// All the editor layers
		Window::Inspector::update();
		Window::EditorConsole::update();
		Window::Hierarchy::update();
		sceneView.update();

		// Editor Camera
		camera.update();
		
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

		Window::Inspector::selectedGameObject = nullptr;
		sceneView.exit();
	}
}

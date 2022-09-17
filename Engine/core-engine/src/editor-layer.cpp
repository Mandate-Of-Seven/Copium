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
#include "windows-system.h"
#include "editor-layer.h"
#include "editor-sceneview.h"
#include "game-object.h"
#include "inspector.h"

namespace Copium::Editor
{
	// Our state
	bool show_demo_window = true;

	void Editor::init()
	{
		//imgui
		ImGui::CreateContext();
		ImGuiIO & io = ImGui::GetIO(); (void) io;
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(windowsSystem.get_window(), true);
		ImGui_ImplOpenGL3_Init("#version 330");

		Window::Inspector::init();
		Window::Inspector::selectedGameObject = new GameObject();

		SceneView::init();
	}

	void Editor::update()
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
		SceneView::update();

		ImGui::EndFrame();
	}

	void Editor::draw()
	{
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void Editor::exit()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();

		delete Window::Inspector::selectedGameObject;
		SceneView::exit();
	}
}

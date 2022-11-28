/*!***************************************************************************************
\file			editor-system.cpp
\project
\author			Sean Ngo
\co-author		Shawn Tanary

\par			Course: GAM200
\par			Section:
\date			16/09/2022

\brief
	This file holds the definition of functions for the editor system.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"
#include "Windows/windows-system.h"
#include "Windows/windows-input.h"
#include "Editor/editor-system.h"
#include "GameObject/game-object.h"
#include "Editor/inspector.h"
#include "Editor/editor-consolelog.h"
#include "Editor/editor-hierarchy-list.h"
#include "Editor/editor-colortheme.h"
#include "Windows/windows-utils.h"
#include "Utilities/thread-system.h"
#include "SceneManager/state-manager.h"
#include "Messaging/message-system.h"
#include "Graphics/graphics-system.h"
#include <ImGuizmo.h>


namespace Copium
{
	namespace
	{
		// Our state
		bool show_demo_window = false;
		ThreadSystem& threadSystem{ *ThreadSystem::Instance() };
		MessageSystem& messageSystem{ *MessageSystem::Instance() };
		InputSystem& inputSystem{ *InputSystem::Instance() };
		WindowsSystem& windowsSystem{ *WindowsSystem::Instance() };
		GraphicsSystem& graphicsSystem{ *GraphicsSystem::Instance() };
		bool tempMode = true;
	}

	void EditorSystem::init()
	{
		messageSystem.subscribe(MESSAGE_TYPE::MT_START_PREVIEW, this);
		messageSystem.subscribe(MESSAGE_TYPE::MT_STOP_PREVIEW, this);
		systemFlags |= FLAG_RUN_ON_EDITOR | FLAG_RUN_ON_PLAY;
		//PRINT("FLAGS: " << systemFlags);

		//imgui
		ImGui::CreateContext();
		ImGuiIO & io = ImGui::GetIO(); (void) io;
		io.Fonts->AddFontFromFileTTF("Data\\arial.ttf", 32.f);

		// Only move window from title bar
		io.ConfigWindowsMoveFromTitleBarOnly = true;

		// Global Font Size
		io.FontGlobalScale = 0.6f;
		
		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(windowsSystem.get_window(), true);
		ImGui_ImplOpenGL3_Init("#version 330");
		ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		ImGui::GetIO().ConfigDockingWithShift = true;
		Window::Inspector::init();
		Window::EditorConsole::init();
		Window::Hierarchy::init();
		Window::ColorTheme::init();

		sceneView.init();
		game.init();
		contentBrowser.init();
		
		// Initialize a new editor camera
		camera.init((float) sceneView.get_width(), (float) sceneView.get_height());
	}

	void EditorSystem::update()
	{
		if (game.is_window_focused())
		{
			if (inputSystem.is_key_held(GLFW_KEY_LEFT_SHIFT) && inputSystem.is_key_pressed(GLFW_KEY_E))
			{
				tempMode = !tempMode;
			}
		}

		if (tempMode != enableEditor)
		{
			enableEditor = tempMode;
			playMode(enableEditor);
		}

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();


		//Dockspace
		if(enableEditor)
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
						Copium::NewSceneManager::Instance()->create_scene();
					}

					if (ImGui::MenuItem("Open...", "Ctrl+O"))
					{
						//open scene
						while (!threadSystem.acquireMutex(MutexType::FileSystem));
						std::string filepath = FileDialogs::open_file("Copium Scene (*.scene)\0*.scene\0");
						threadSystem.returnMutex(MutexType::FileSystem);
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
						if (NewSceneManager::Instance()->get_scenefilepath().empty()) {
							//save sceen as
							while (!threadSystem.acquireMutex(MutexType::FileSystem));
							std::string filepath = FileDialogs::save_file("Copium Scene (*.scene)\0.scene\0");
							threadSystem.returnMutex(MutexType::FileSystem);
							std::cout << filepath << std::endl;
							Copium::NewSceneManager::Instance()->save_scene(filepath);
						}
						else 
						{
							NewSceneManager::Instance()->save_scene();
						}
					}

					if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
					{
						
						if (Copium::NewSceneManager::Instance()->get_current_scene())
						{
							//save sceen as
							while (!threadSystem.acquireMutex(MutexType::FileSystem));
							std::string filepath = FileDialogs::save_file("Copium Scene (*.scene)\0.scene\0");
							threadSystem.returnMutex(MutexType::FileSystem);
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
						Scene* s = NewSceneManager::Instance()->get_storage_scene();
						if (s)
						{
							NewSceneManager::Instance()->endPreview();
						}
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
						if (NewSceneManager::Instance()->startPreview())
						{
							messageSystem.dispatch(MESSAGE_TYPE::MT_START_PREVIEW);
						}
					}
					if (ImGui::MenuItem("Stop Scene"))
					{
						if (NewSceneManager::Instance()->endPreview())
						{
							messageSystem.dispatch(MESSAGE_TYPE::MT_STOP_PREVIEW);
						}
					}

					ImGui::EndMenu();
				}

				//if your IMGUI window can be closed,you should make the bool inline in the header and include it here to be able to reopen it
				if (ImGui::BeginMenu("Windows"))
				{
					if (ImGui::MenuItem("Hierarchy"))
					{
						Window::Hierarchy::isHierarchyOpen = true;
					}
					if (ImGui::MenuItem("Console Log"))
					{
						Window::EditorConsole::isConsoleLogOpen = true;
					}
					if (ImGui::MenuItem("Theme generator"))
					{
						Window::ColorTheme::isColorThemeOpen = true;
					}
					if (ImGui::MenuItem("Inspector"))
					{
						Window::Inspector::isInspectorOpen = true;
					}

					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}


            //Call all the editor layers updates here
			Window::ColorTheme::update();
			Window::Inspector::update();
			Window::EditorConsole::update();
			Window::Hierarchy::update();
			sceneView.update();
			game.update();
			contentBrowser.update();

			// demo update
			if (show_demo_window)
				ImGui::ShowDemoWindow(&show_demo_window);

			// Game Camera
			if (!graphicsSystem.get_cameras().empty())
			{
				(*graphicsSystem.get_cameras().begin())->update();
			}

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
		sceneView.exit();
		game.exit();
		contentBrowser.exit();


		std::cout << "Before deleting, Undo stack: " << commandManager.undoStack.size() << ", Redo stack:" << commandManager.redoStack.size()<<"\n";
		while (commandManager.undoStack.size() > 0)
		{
			UndoRedo::Command* temp = EditorSystem::Instance()->get_commandmanager()->undoStack.top();
			delete temp;
			EditorSystem::Instance()->get_commandmanager()->undoStack.pop();
			
		}

		while (commandManager.redoStack.size() > 0)
		{
			UndoRedo::Command* temp = EditorSystem::Instance()->get_commandmanager()->redoStack.top();
			delete temp;
			EditorSystem::Instance()->get_commandmanager()->redoStack.pop();
			
		}
		std::cout << "After deleting, Undo stack: " << commandManager.undoStack.size() << ", Redo stack:" << commandManager.redoStack.size() << "\n";
		

		camera.exit();
	}

	void EditorSystem::handleMessage(MESSAGE_TYPE _mType)
	{
		if (_mType == MESSAGE_TYPE::MT_START_PREVIEW)
		{
			//tempMode = false;
		}
		else if (_mType == MESSAGE_TYPE::MT_STOP_PREVIEW)
		{
			//tempMode = true;
		}
	}

	void EditorSystem::imguiConsoleAddLog(std::string value)
	{
		std::cout << value << "\n";
		Window::EditorConsole::editorLog.add_logEntry(value);
	}

	void EditorSystem::playMode(bool _enabled)
	{
		if (!_enabled)
		{
			// Swap camera
			//camera.get_framebuffer()->exit();
			glm::vec2 dimension = { windowsSystem.get_window_width(), windowsSystem.get_window_height() };
			// Game Camera
			if (!graphicsSystem.get_cameras().empty())
			{
				(*graphicsSystem.get_cameras().begin())->on_resize(dimension.x, dimension.y);
				glViewport(0, 0, (GLsizei)dimension.x, (GLsizei)dimension.y);
			}
		}
		else if(_enabled)
			camera.get_framebuffer()->init();
	}

	UndoRedo::CommandManager* EditorSystem::get_commandmanager()
	{
		return &commandManager;
	}
}

/*!***************************************************************************************
\file			editor-hierarchy-list.cpp
\project
\author			Matthew Lau

\par			Course: GAM200
\par			Section:
\date			27/10/2022

\brief
	Definition of functions that handle initialization and update of Hierarchy List

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"
#include "Editor/editor-hierarchy-list.h"
#include <Editor/editor-undoredo.h>
#include <Editor/editor-system.h>

//M2
namespace Window::Hierarchy
{
	Copium::Scene * currentScene = nullptr;
	bool sceneSelected;
	Copium::GameObjectID selectedID;



	void init()
	{
		Window::Hierarchy::isHierarchyOpen = true;
		sceneSelected = false;
		currentScene = Copium::NewSceneManager::Instance().get_current_scene();

	}

	void update()
	{
		if (!Window::Hierarchy::isHierarchyOpen)
		{
			//std::cout << "Test\n";
			return;
		}

		// Handle scene change
		if (currentScene != Copium::NewSceneManager::Instance().get_current_scene())
			currentScene = Copium::NewSceneManager::Instance().get_current_scene();


		ImGui::SetNextWindowSize(ImVec2(500, 900), ImGuiCond_FirstUseEver);
		ImGui::Begin("Hierarchy", &Window::Hierarchy::isHierarchyOpen, ImGuiWindowFlags_MenuBar);

		// Menu Bar
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Menu"))
			{
				 //Add menu items
				if (ImGui::MenuItem("Create GameObject", nullptr))
				{
					if (!currentScene)
					{
						Window::EditorConsole::editorLog.add_logEntry("Siao eh, no scene la");
					}
					else
					{
						Copium::GameObject* temp = GOF.instantiate();
						if (temp)
						{
							Copium::UndoRedo::Command* tempUndo = new Copium::UndoRedo::GameObjectCommand(temp, true);
							Copium::EditorSystem::Instance().get_commandmanager()->undoStack.push(tempUndo);
						}
						else
						{
							std::cout << "Error creating game object\n";
						}
					}


				}
				if (ImGui::MenuItem("Delete Selected GameObject", nullptr))
				{
					if (Copium::NewSceneManager::Instance().get_selected_gameobject())
					{
						std::cout << "Delete\n";
						Copium::UndoRedo::Command* tempUndo = new Copium::UndoRedo::GameObjectCommand(MyNewSceneManager.get_selected_gameobject(),false);
						Copium::EditorSystem::Instance().get_commandmanager()->undoStack.push(tempUndo);
						GOF.destroy(MyNewSceneManager.get_selected_gameobject());
						MyNewSceneManager.set_selected_gameobject(nullptr);
					}
					else
					{
						Window::EditorConsole::editorLog.add_logEntry("Siao eh, no scene la");
					}
				}
				if (ImGui::MenuItem("Clone Selected GameObject", nullptr))
				{
					if (Copium::NewSceneManager::Instance().get_selected_gameobject())
					{
						std::cout << "Clone\n";
						GOF.instantiate(*Copium::NewSceneManager::Instance().get_selected_gameobject());
						MyNewSceneManager.set_selected_gameobject(nullptr);
					}
					else
					{
						Window::EditorConsole::editorLog.add_logEntry("Siao eh, no scene la");
					}
				}				
				if (ImGui::MenuItem("Create a Child GameObject"))
				{
					if (MyNewSceneManager.get_selected_gameobject())
					{
						GOF.create_child(*MyNewSceneManager.get_selected_gameobject());
					}
				}

				if (ImGui::BeginMenu("Add Archetype"))
				{
					if (!currentScene)
					{
						Window::EditorConsole::editorLog.add_logEntry("Siao eh, no scene la");
					}
					else
					{
						for (std::map<std::string, Copium::GameObject*>::iterator iter = GOF.get_archetype_map().begin();
							iter != GOF.get_archetype_map().end(); ++iter)
						{
							if (ImGui::MenuItem((*iter).first.c_str()) && currentScene)
							{
								GOF.instantiate(*(*iter).second);
							}

						} 
					}

					ImGui::EndMenu();
				}
				ImGui::EndMenu();

			}
			ImGui::EndMenuBar();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 10.f);

		ImGuiTreeNodeFlags rootFlags = ImGuiTreeNodeFlags_DefaultOpen;
		
		// Ensure that game objects are displayed only if there is a current scene loaded
		if (currentScene)
		{
			// Find all the root nodes
			std::vector<Copium::GameObject*>roots;
			for (size_t i{ 0 }; i < currentScene->get_gameobjcount(); ++i)
			{
				Copium::GameObject* tmp = currentScene->gameObjects[i];
				if (!tmp->transform.hasParent())
					roots.push_back(tmp);
			}

			if (!currentScene->get_gameobjcount())
				rootFlags |= ImGuiTreeNodeFlags_Leaf;


			std::string sceneName;
			if (currentScene->get_filename().empty())
			{
				sceneName = currentScene->get_name();
			}
			else
			{
				size_t offset = currentScene->get_filename().find_last_of("/\\");
				size_t endOffset = currentScene->get_filename().find(".scene")-1;
				sceneName = currentScene->get_filename().substr(offset + 1, endOffset-offset);
				if (currentScene->get_state() == Copium::Scene::SceneState::play)
				{
					sceneName += "\t PREVIEWING";
				}
			}

			// Display scene name as the rootiest node
			if (ImGui::TreeNodeEx(sceneName.c_str(), rootFlags))
			{

				bool isSelected = false;
				for (size_t i{ 0 }; i < roots.size(); ++i)
				{
					display_gameobject_advanced(*(roots[i]), selectedID);
					
					if (isSelected)
						sceneSelected = false;
				}	

				ImGui::TreePop();
			}

		}


		ImGui::PopStyleVar();			
		ImGui::End();
	}
	
	void display_gameobject(const Copium::GameObject& _go)
	{
		const Copium::Transform& transform{ _go.transform };
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
		// Prevent arrow from showing up which will cause assert if no children
		if (!transform.children.empty())
			flags |= ImGuiTreeNodeFlags_Leaf;
		else
		{
			flags |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		}

		bool result = ImGui::TreeNodeEx(_go.get_name().c_str(), flags);
		if (!result)
			return;

		// If game object has children, recursively display children
		if(!transform.children.empty())
		{
			for (auto iter = transform.children.begin(); iter != transform.children.end(); ++iter)
			{
				display_gameobject(*MyNewSceneManager.findGameObjByID((*iter)->entityID));
			}
		}

		// Remember to pop for every tree node created!!!
		ImGui::TreePop();

	}
	bool display_gameobject_advanced(Copium::GameObject& _go, Copium::GameObjectID& _selected)
	{
		bool isSelected = false;
		ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;

		if (_go.transform.children.empty())
		{
			baseFlags |= ImGuiTreeNodeFlags_Leaf;
		}
			

		if (_selected == _go.id)
		{
			baseFlags |= ImGuiTreeNodeFlags_Selected;				
		}


		if (!ImGui::TreeNodeEx(_go.get_name().c_str(), baseFlags))
			return false;

		if (ImGui::BeginDragDropSource())
		{
			static void* container;
			container = &_go;
			ImGui::SetDragDropPayload("GameObject", &container, sizeof(void*));
			ImGui::EndDragDropSource();
		}
		//for (const auto& pComponent : _go.components)
		//{
		//	if (ImGui::BeginDragDropSource())
		//	{
		//		static void* container;
		//		container = pComponent;
		//		ImGui::SetDragDropPayload(pComponent->Name().c_str(), &container, sizeof(void*));
		//		ImGui::EndDragDropSource();
		//	}
		//}


		if (ImGui::IsItemClicked())
		{
			std::cout << _go.get_name() << " is selected\n";
			_selected = _go.id;
			isSelected = true;
			Copium::NewSceneManager::Instance().set_selected_gameobject(&_go);

		}

		// If game object has children, recursively display children
		if (!_go.transform.children.empty())
		{
			for (auto pChild : _go.transform.children)
			{
				isSelected = display_gameobject_advanced(*MyNewSceneManager.findGameObjByID(pChild->entityID), _selected);
			}
		}

		ImGui::TreePop();

		return isSelected;

	}

	// Create GameObject Button functionality
	bool create_gameobject_btn(const std::string& _btnName)
	{
		static int clicked = 0;
		if (!ImGui::Button(_btnName.c_str()))
			return false;

		++clicked;

		if (clicked == 1)
		{
			std::cout << "btn is clicked\n";
			clicked = 0;

			// Use Shawn's debug system
			if (!GOF.instantiate())
				std::cout << "Error creating game object\n";


		}
		return true;
	}

	bool create_delete_btn(const std::string& _btnName)
	{
		static int deleteClicked = 0;
		if (!ImGui::Button(_btnName.c_str()))
			return false;

		++deleteClicked;

		if (deleteClicked)
		{
			deleteClicked = 0;
		}
		return true;

	}
}

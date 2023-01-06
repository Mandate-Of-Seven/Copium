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
#include <Editor/editor-system.h>
#include "Windows/windows-input.h"
#include "Windows/windows-system.h"
#include <Events/events-system.h>


//M2
namespace Window::Hierarchy
{
	//Copium::Scene * currentScene = nullptr;
	bool sceneSelected;
	//Copium::GameObjectID selectedID;
	Copium::InputSystem& inputSystem = Copium::InputSystem::Instance();
	bool dragging;

	void Init()
	{
		Window::Hierarchy::isHierarchyOpen = true;
		sceneSelected = false;
		dragging = false;
	}

	void Update()
	{
		if (!Window::Hierarchy::isHierarchyOpen)
		{
			//std::cout << "Test\n";
			return;
		}

		// Handle scene change
		//if (currentScene != Copium::SceneManager::Instance().get_current_scene())
		//	currentScene = Copium::SceneManager::Instance().get_current_scene();


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
					EntityID createdID{};
					MyEventSystem.publish(new Copium::InstantiateEntityEvent(&createdID));
					PRINT("NEW GAMEOBJECT ID: " << createdID);
					//if (temp)
					//{
					//	std::shared_ptr<Copium::GameObject>* sptr = MySceneManager.find_gameobject_sptr(temp);
					//	if (sptr)
					//	{
					//		//Copium::UndoRedo::Command* tempUndo = new Copium::UndoRedo::GameObjectCommand(*MySceneManager.find_gameobject_sptr(temp), true);
					//		//Copium::EditorSystem::Instance().get_commandmanager()->undoStack.push(tempUndo);
					//		tempUndo->printCommand();
					//	}

					//}
					//else
					//{
					//	std::cout << "Error creating game object\n";
					//}
				}
				if (ImGui::MenuItem("Delete Selected GameObject", nullptr))
				{
					//if (Copium::SceneManager::Instance().get_selected_gameobject())
					//{
					//	std::cout << "Delete\n";
					//	Copium::UndoRedo::Command* tempUndo = new Copium::UndoRedo::GameObjectCommand(MySceneManager.get_selected_gameobject_sptr(),false);
					//	Copium::EditorSystem::Instance().get_commandmanager()->undoStack.push(tempUndo);
					//	GOF.destroy(MySceneManager.get_selected_gameobject());
					//	MySceneManager.set_selected_gameobject(nullptr);
					//}
					//else
					//{
					//	Window::EditorConsole::editorLog.add_logEntry("Siao eh, no scene la");
					//}
				}
				if (ImGui::MenuItem("Clone Selected GameObject", "Ctrl+D"))
				{
					//if (Copium::SceneManager::Instance().get_selected_gameobject())
					//{
					//	std::cout << "Clone\n";
					//	GOF.instantiate(*Copium::SceneManager::Instance().get_selected_gameobject());
					//	MySceneManager.set_selected_gameobject(nullptr);
					//}
					//else
					//{
					//	Window::EditorConsole::editorLog.add_logEntry("Siao eh, no scene la");
					//}
				}				
				if (ImGui::MenuItem("Create a Child GameObject"))
				{
					//if (MySceneManager.get_selected_gameobject())
					//{
					//	GOF.create_child(*MySceneManager.get_selected_gameobject());
					//}
				}
				if (ImGui::BeginMenu("Add Archetype"))
				{
					//if (!currentScene)
					//{
					//	Window::EditorConsole::editorLog.add_logEntry("Siao eh, no scene la");
					//}
					//else
					//{
					//	for (std::map<std::string, Copium::GameObject*>::iterator iter = GOF.get_archetype_map().begin();
					//		iter != GOF.get_archetype_map().end(); ++iter)
					//	{
					//		if (ImGui::MenuItem((*iter).first.c_str()) && currentScene)
					//		{
					//			GOF.instantiate(*(*iter).second);
					//		}

					//	} 
					//}

					ImGui::EndMenu();
				}
				ImGui::EndMenu();

			}
			ImGui::EndMenuBar();
		}

		//KeyBoard Shortcuts//
		if (inputSystem.is_key_held(GLFW_KEY_LEFT_CONTROL) && inputSystem.is_key_pressed(GLFW_KEY_D))
		{
			/*if (Copium::SceneManager::Instance().get_selected_gameobject())
			{
				std::cout << "Clone\n";
				GOF.instantiate(*Copium::SceneManager::Instance().get_selected_gameobject());
				MySceneManager.set_selected_gameobject(nullptr);
			}
			else
			{
				Window::EditorConsole::editorLog.add_logEntry("No scene is loaded!");
			}*/
		}
		//if (Copium::SceneManager::Instance().get_selected_gameobject() && inputSystem.is_key_pressed(GLFW_KEY_BACKSPACE))
		{
			/*if (Copium::SceneManager::Instance().get_selected_gameobject())
			{
				std::cout << "Delete\n";
				Copium::UndoRedo::Command* tempUndo = new Copium::UndoRedo::GameObjectCommand(MySceneManager.get_selected_gameobject_sptr(), false);
				Copium::EditorSystem::Instance().get_commandmanager()->undoStack.push(tempUndo);
				GOF.destroy(MySceneManager.get_selected_gameobject());
				MySceneManager.set_selected_gameobject(nullptr);
			}
			else
			{
				Window::EditorConsole::editorLog.add_logEntry("No scene is loaded!");
			}*/
		}
		///////////////////


		ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 10.f);
		ImGuiTreeNodeFlags rootFlags = ImGuiTreeNodeFlags_DefaultOpen;
		
		// Ensure that game objects are displayed only if there is a current scene loaded
		//if (currentScene)
		//{
		//	// Find all the root nodes
		//	std::vector<Copium::GameObject*>roots;
		//	for (size_t i{ 0 }; i < currentScene->get_gameobjcount(); ++i)
		//	{
		//		Copium::GameObject* tmp = currentScene->gameObjects[i];
		//		if (!tmp->transform.hasParent())
		//			roots.push_back(tmp);
		//	}

		//	if (!currentScene->get_gameobjcount())
		//		rootFlags |= ImGuiTreeNodeFlags_Leaf;


		//	std::string sceneName;
		//	if (currentScene->get_filename().empty())
		//	{
		//		sceneName = currentScene->get_name();
		//	}
		//	else
		//	{
		//		size_t offset = currentScene->get_filename().find_last_of("/\\");
		//		size_t endOffset = currentScene->get_filename().find(".scene")-1;
		//		sceneName = currentScene->get_filename().substr(offset + 1, endOffset-offset);
		//		if (currentScene->get_state() == Copium::Scene::SceneState::play)
		//		{
		//			sceneName += "\t PREVIEWING";
		//		}
		//	}

			// Display scene name as the rootiest node
			if (ImGui::TreeNodeEx("PLACEHOLDER SCENE NAME", rootFlags))
			{
				SparseSet<Copium::Entity, MAX_ENTITIES>* pEntities{};
				MyEventSystem.publish(new Copium::GetEntitiesEvent(pEntities));
				if (pEntities)
				{
					for (size_t i = 0; i < (*pEntities).GetSize(); ++i)
					{
						display_gameobject((*pEntities).GetDenseIndex(i));
					}
				}
				//bool isSelected = false;
				//for (size_t i{ 0 }; i < roots.size(); ++i)
				//{
					//display_gameobject_advanced(*(roots[i]), selectedID);
					
					//if (isSelected)
						//sceneSelected = false;
				//}	
				ImGui::TreePop();
			}

		//}


		ImGui::PopStyleVar();			
		ImGui::End();
	}
	
	void display_gameobject(EntityID entityID)
	{
		//const Copium::Transform& transform{ _go.transform };
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
		// Prevent arrow from showing up which will cause assert if no children
		//if (!transform.children.empty())
		flags |= ImGuiTreeNodeFlags_Leaf;
		//else
		//{
		//	flags |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		//}

		Copium::Entity* selectedEntity{};
		MyEventSystem.publish(new Copium::GetEntityEvent{entityID,selectedEntity});


		if (selectedEntity && ImGui::TreeNodeEx(selectedEntity->name.c_str(), flags))
		{
			if (ImGui::IsItemClicked())
			{
				std::cout << selectedEntity->name << " is selected\n";
				MyEditorSystem.SetSelectedEntityID(entityID);
				//isSelected = true;
				//Copium::SceneManager::Instance().set_selected_gameobject(&_go);
			}
		//	// Remember to pop for every tree node created!!!
			ImGui::TreePop();
		}

		// If game object has children, recursively display children
		//if(!transform.children.empty())
		//{
		//	for (auto iter = transform.children.begin(); iter != transform.children.end(); ++iter)
		//	{
		//		display_gameobject(*MySceneManager.findGameObjByID((*iter)->entityID));
		//	}
		//}


	}
	bool display_gameobject_advanced(Copium::Entity& entity)
	{
		bool isSelected = false;
		ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;


		//// If root node does not have children, it is simply a leaf node (end of the branch)
		//if (_go.transform.children.empty())
		//{
		//	baseFlags |= ImGuiTreeNodeFlags_Leaf;
		//}
		//	
		//// If there is a node that is already selected, set the selected flag
		//if (_selected == _go.id)
		//{
		//	baseFlags |= ImGuiTreeNodeFlags_Selected;				
		//}

		//if (!ImGui::TreeNodeEx(_go.get_name().c_str(), baseFlags))
		//	return false;

		//if (ImGui::BeginDragDropSource())
		//{
		//	static void* container; 
		//	container = &_go;
		//	ImGui::SetDragDropPayload("GameObject", &container, sizeof(void*));
		//	ImGui::EndDragDropSource();

		//	//std::cout << "ID of selected Game Object: " << _selected << std::endl;

		//}

		if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
		{
			//if (!_go.transform.hasParent())
			//{
			//	//std::cout << "ID of selected Game Object: " << _selected << std::endl;
			//	int n_next = (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
			//	//std::cout << "move up or down: " << n_next << std::endl;
			//	dragging = true;
			//}



		}			
		if (dragging && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		{
			dragging = false;
			//PRINT("mouse released over: " << _go.id);
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



		//if (ImGui::IsItemClicked())
		//{
		//	std::cout << _go.get_name() << " is selected\n";
		//	_selected = _go.id;
		//	isSelected = true;
		//	Copium::SceneManager::Instance().set_selected_gameobject(&_go);

		//}

		// If game object has children, recursively display children
		//if (!_go.transform.children.empty())
		//{
		//	for (auto pChild : _go.transform.children)
		//	{
		//		isSelected = display_gameobject_advanced(*MySceneManager.findGameObjByID(pChild->entityID), _selected);
		//	}
		//}

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
			//if (!GOF.instantiate())
			//	std::cout << "Error creating game object\n";


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

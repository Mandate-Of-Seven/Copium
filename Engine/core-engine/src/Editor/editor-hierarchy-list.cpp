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
#include "Windows/windows-input.h"
#include "Windows/windows-system.h"

namespace Copium
{
	namespace
	{
		SceneManager* sm = SceneManager::Instance();
		InputSystem* is = InputSystem::Instance();
	}
	void EditorHierarchyList::init()
	{
		if (sm)
			currentScene = sm->get_current_scene();

		isHierarchyOpen = true;

	}
	void EditorHierarchyList::update()
	{
		if (!isHierarchyOpen)
			return;
		

		// Handle scene change
		if (currentScene != sm->get_current_scene())
			currentScene = sm->get_current_scene();


		ImGui::SetNextWindowSize(ImVec2(500, 900), ImGuiCond_FirstUseEver);
		ImGui::Begin("Hierarchy", &isHierarchyOpen, ImGuiWindowFlags_MenuBar);

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
						Copium::GameObject* temp = MyGOF.instantiate();
						if (temp)
						{
							std::shared_ptr<Copium::GameObject>* sptr = MySceneManager.find_gameobject_sptr(temp);
							if (sptr)
							{
								Copium::UndoRedo::Command* tempUndo = new Copium::UndoRedo::GameObjectCommand(*MySceneManager.find_gameobject_sptr(temp), true);
								Copium::EditorSystem::Instance()->get_commandmanager()->undoStack.push(tempUndo);
								tempUndo->printCommand();
							}

						}
						else
						{
							std::cout << "Error creating game object\n";
						}
					}


				}
				if (ImGui::MenuItem("Delete Selected GameObject", nullptr))
				{
					if (Copium::SceneManager::Instance()->get_selected_gameobject())
					{
						std::cout << "Delete\n";
						Copium::UndoRedo::Command* tempUndo = new Copium::UndoRedo::GameObjectCommand(MySceneManager.get_selected_gameobject_sptr(), false);
						Copium::EditorSystem::Instance()->get_commandmanager()->undoStack.push(tempUndo);
						MyGOF.destroy(MySceneManager.get_selected_gameobject());
						MySceneManager.set_selected_gameobject(nullptr);
					}
					else
					{
						Window::EditorConsole::editorLog.add_logEntry("Siao eh, no scene la");
					}
				}
				if (ImGui::MenuItem("Clone Selected GameObject", "Ctrl+D"))
				{
					if (Copium::SceneManager::Instance()->get_selected_gameobject())
					{
						std::cout << "Clone\n";
						MyGOF.instantiate(*Copium::SceneManager::Instance()->get_selected_gameobject());
						MySceneManager.set_selected_gameobject(nullptr);
					}
					else
					{
						Window::EditorConsole::editorLog.add_logEntry("Siao eh, no scene la");
					}
				}
				if (ImGui::MenuItem("Create a Child GameObject"))
				{
					if (MySceneManager.get_selected_gameobject())
					{
						MyGOF.create_child(*MySceneManager.get_selected_gameobject());
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
						for (std::map<std::string, Copium::GameObject*>::iterator iter = MyGOF.get_archetype_map().begin();
							iter != MyGOF.get_archetype_map().end(); ++iter)
						{
							if (ImGui::MenuItem((*iter).first.c_str()) && currentScene)
							{
								MyGOF.instantiate(*(*iter).second);
							}

						}
					}

					ImGui::EndMenu();
				}
				ImGui::EndMenu();

			}
			ImGui::EndMenuBar();
		}

		//KeyBoard Shortcuts//
		if (is->is_key_held(GLFW_KEY_LEFT_CONTROL) && is->is_key_pressed(GLFW_KEY_D))
		{
			if (Copium::SceneManager::Instance()->get_selected_gameobject())
			{
				std::cout << "Clone\n";
				MyGOF.instantiate(*Copium::SceneManager::Instance()->get_selected_gameobject());
				MySceneManager.set_selected_gameobject(nullptr);
			}
			else
			{
				Window::EditorConsole::editorLog.add_logEntry("No scene is loaded!");
			}
		}
		if (sm->get_selected_gameobject() && is->is_key_pressed(GLFW_KEY_BACKSPACE))
		{
			if (Copium::SceneManager::Instance()->get_selected_gameobject())
			{
				std::cout << "Delete\n";
				Copium::UndoRedo::Command* tempUndo = new Copium::UndoRedo::GameObjectCommand(MySceneManager.get_selected_gameobject_sptr(), false);
				Copium::EditorSystem::Instance()->get_commandmanager()->undoStack.push(tempUndo);
				MyGOF.destroy(MySceneManager.get_selected_gameobject());
				MySceneManager.set_selected_gameobject(nullptr);
			}
			else
			{
				Window::EditorConsole::editorLog.add_logEntry("No scene is loaded!");
			}
		}
		///////////////////


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
				size_t endOffset = currentScene->get_filename().find(".scene") - 1;
				sceneName = currentScene->get_filename().substr(offset + 1, endOffset - offset);
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
					display_gameobject(*(roots[i]), selectedID, roots, i);

				}

				ImGui::TreePop();
			}

		}


		ImGui::PopStyleVar();
		ImGui::End();
	}
	void EditorHierarchyList::exit()
	{
		currentScene = nullptr;
	}

	bool EditorHierarchyList::display_gameobject(GameObject& _go, GameObjectID& _selected, std::vector<GameObject*>& _vector, int _index)
	{
		bool isSelected = false;
		ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;


		// If root node does not have children, it is simply a leaf node (end of the branch)
		if (_go.transform.children.empty())
		{
			baseFlags |= ImGuiTreeNodeFlags_Leaf;
		}

		// If there is a node that is already selected, set the selected flag
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

			//std::cout << "ID of selected Game Object: " << _selected << std::endl;
		}

		if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
		{
			if (!_go.transform.hasParent())
			{
				//std::cout << "ID of selected Game Object: " << _selected << std::endl;
				int n_next = (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
				if (n_next > 0)
					PRINT("Move up\n");
				else
					PRINT("Move down\n");

				n_next += _index;
				if (n_next >= 0 && n_next < _vector.size())
				{
					PRINT("there will be movement\n");
					GameObject* temp = _vector[n_next];
					size_t idx1{0}, idx2{0};
					for (size_t i{ 0 }; i < currentScene->get_gameobjcount(); ++i)
					{
						if (temp == currentScene->gameObjects[i])
						{
							idx1 = i;
							break;
						}
					}

					for (size_t i{ 0 }; i < currentScene->get_gameobjcount(); ++i)
					{
						if (&_go == currentScene->gameObjects[i])
						{
							idx2 = i;
							break;
						}
					}

					currentScene->gameObjects[idx1] = currentScene->gameObjects[idx2];
					currentScene->gameObjects[idx2] = temp;
			

				}
				ImGui::ResetMouseDragDelta();


			}

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
			sm->set_selected_gameobject(&_go);

		}

		// If game object has children, recursively display children
		if (!_go.transform.children.empty())
		{
			int idx{ 0 };
			for (auto pChild : _go.transform.children)
			{
				isSelected = display_gameobject(pChild->gameObj, _selected, _go.transform.children, idx);
				++idx;
			}
		}

		ImGui::TreePop();

		return isSelected;
	}
	bool EditorHierarchyList::display_gameobject(GameObject& _go, GameObjectID& _selected, std::list<Transform*>& _list, int _index)
	{
		bool isSelected = false;
		ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;


		// If root node does not have children, it is simply a leaf node (end of the branch)
		if (_go.transform.children.empty())
		{
			baseFlags |= ImGuiTreeNodeFlags_Leaf;
		}

		// If there is a node that is already selected, set the selected flag
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

			//std::cout << "ID of selected Game Object: " << _selected << std::endl;
		}

		// Handle any reordering
		if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
		{
				//std::cout << "ID of selected Game Object: " << _selected << std::endl;
				int n_next = (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);

				if (n_next + _index >= 0 && n_next + _index < _list.size())
				{		
					Transform* tmp = &_go.transform;
					std::list<Transform*>::iterator iter1, iter2;

					iter1 = _list.begin();
					for (int i{ 0 }; i < _index; ++i)
					{
						++iter1;
					}
					iter2 = iter1;

					if (n_next > 0)
					{
						++iter2;
					}
					else
					{
						--iter2;
					}

					*iter1 = *iter2;
					*iter2 = tmp;
				}

				ImGui::ResetMouseDragDelta();
		
		}

		if (ImGui::IsItemClicked())
		{
			std::cout << _go.get_name() << " is selected\n";
			_selected = _go.id;
			isSelected = true;
			sm->set_selected_gameobject(&_go);

		}

		// If game object has children, recursively display children
		if (!_go.transform.children.empty())
		{
			int idx{ 0 };
			for (auto pChild : _go.transform.children)
			{
				isSelected = display_gameobject(pChild->gameObj, _selected, _list, idx);
				++idx;
			}
		}

		ImGui::TreePop();

		return isSelected;
	}

	bool EditorHierarchyList::create_gameobject_btn(const std::string& _btnName)
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
			if (!MyGOF.instantiate())
				std::cout << "Error creating game object\n";


		}
		return true;
	}
	bool EditorHierarchyList::create_delete_btn(const std::string& _btnName)
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

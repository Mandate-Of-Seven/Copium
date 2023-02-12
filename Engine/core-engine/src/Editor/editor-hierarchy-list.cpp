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
		isParenting = false;
		isPopUpOpen = false;

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
						//std::cout << "Delete\n";
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
				if (ImGui::MenuItem("Shift Up"))
				{
					ShiftUp();
				}
				if (ImGui::MenuItem("Shift Down"))
				{
					isParenting = true;
					PRINT("Shift down");
				}


				/*
				//if (ImGui::BeginMenu("Add Archetype"))
				//{
				//	if (!currentScene)
				//	{
				//		Window::EditorConsole::editorLog.add_logEntry("Siao eh, no scene la");
				//	}
				//	else
				//	{
				//		for (std::map<std::string, Copium::GameObject*>::iterator iter = MyGOF.get_archetype_map().begin();
				//			iter != MyGOF.get_archetype_map().end(); ++iter)
				//		{
				//			if (ImGui::MenuItem((*iter).first.c_str()) && currentScene)
				//			{
				//				MyGOF.instantiate(*(*iter).second);
				//			}

				//		}
				//	}

				//	ImGui::EndMenu();
				//}
				ImGui::EndMenu();
				*/

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		if (isParenting)
			DisplayAdoptionWindow();

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
		if (sm->get_selected_gameobject() && is->is_key_pressed(GLFW_KEY_DELETE) && ImGui::IsWindowFocused())
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
		if (is->is_key_held(GLFW_KEY_LEFT_SHIFT))
		{
			Reorder(sm->get_selected_gameobject());
		}
		///////////////////


		ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 10.f);
		ImGuiTreeNodeFlags rootFlags = ImGuiTreeNodeFlags_DefaultOpen;




		// Ensure that game objects are displayed only if there is a current scene loaded
		if (currentScene)
		{		
			//PRINT("Start Editor Update");
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

			}

			if (sm->GetSceneState() == Copium::Scene::SceneState::play)
			{
				sceneName += "\t PREVIEWING";
			}

			int test{ 0 };
			// Display scene name as the rootiest node
			bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)test, rootFlags, sceneName.c_str());


			if (nodeOpen)
			{			
				//bool isSelected = false;
				for (size_t i{ 0 }; i < roots.size(); ++i)
				{
					display_gameobject(*(roots[i]), selectedID, roots, i);

				}
				ImGui::TreePop();

			}
			
			//PRINT("End Editor Update");
		} 

		if (ImGui::IsWindowHovered())
		{
			//PRINT("Hovering over hierarchy list");
			if (is->is_mousebutton_pressed(GLFW_MOUSE_BUTTON_RIGHT) && sm->get_current_scene())
			{				
				//PRINT("Right Clicking within window boundaries");

				if (!isPopUpOpen)
				{
					ImGui::OpenPopup("HOps");
					isHierarchyPopUpOpen = true;
				}

			}
		}
		if(isHierarchyPopUpOpen)
			DisplayHierarchyOptionsPopUpWindow();

		ImGui::PopStyleVar();
		ImGui::End();
	}
	void EditorHierarchyList::exit()
	{
		currentScene = nullptr;
	}

	bool EditorHierarchyList::display_gameobject(GameObject& _go, GameObjectID& _selected, std::vector<GameObject*>& _vector, size_t _index)
	{
		bool isSelected = false, nodeOpen = false;
		ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick;


		// If root node does not have children, it is simply a leaf node (end of the branch)
		if (_go.transform.children.empty())
		{
			baseFlags |= ImGuiTreeNodeFlags_Leaf;
		}

		// If there is a node that is already selected, set the selected flag
		if (_selected == _go.uuid)
		{
			baseFlags |= ImGuiTreeNodeFlags_Selected;
		}

		nodeOpen = ImGui::TreeNodeEx(_go.get_name().c_str(), baseFlags);
/*
		if (is->is_key_held(GLFW_KEY_LEFT_SHIFT))
		{
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
			
						ImGui::ResetMouseDragDelta();
					}



				}

			}
		}*/			
		if (ImGui::BeginDragDropSource())
		{
			static void* container;
			container = &_go;
			ImGui::SetDragDropPayload("GameObject", &container, sizeof(void*));
			ImGui::EndDragDropSource();

			//std::cout << "ID of selected Game Object: " << _selected << std::endl;
		}else if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
		{
			PRINT("selected");
			std::cout << _go.get_name() << " is selected\n";
			_selected = _go.uuid;
			isSelected = true;
			sm->set_selected_gameobject(&_go);

		}
		else if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
		{
			std::cout << _go.get_name() << " is right clicked\n";
			_selected = _go.uuid;
			isSelected = true;
			sm->set_selected_gameobject(&_go);

			if (!isHierarchyPopUpOpen)
			{
				ImGui::OpenPopup("Options");
				isPopUpOpen = true;
			}

		}	
		int res = 0;
		if (isPopUpOpen && _selected == _go.uuid)
		{
			res = DisplayOptionsPopUpWindow();
		}
		for (std::list<Transform*>::iterator iter = _go.transform.children.begin(); iter != _go.transform.children.end(); ++iter)
			{
				if (!(*iter))
				{
					_go.transform.children.erase(iter);
					break;
				}
		}		
		if (nodeOpen)
		{
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

			if (res != 1 && !ImGui::IsItemToggledOpen())
			{
				// If game object has children, recursively display children
				if (!_go.transform.children.empty())
				{
					int idx{ 0 };
					for (auto pChild : _go.transform.children)
					{
						if (!pChild)
							continue;

						isSelected = display_gameobject(pChild->gameObj, _selected, _go.transform.children, idx);
						++idx;
					}
				}
			}

			ImGui::TreePop();
		}


		return isSelected;
	}
	bool EditorHierarchyList::display_gameobject(GameObject& _go, GameObjectID& _selected, std::list<Transform*>& _list, size_t _index)
	{
		bool isSelected = false, nodeOpen = false;
		ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;


		// If root node does not have children, it is simply a leaf node (end of the branch)
		if (_go.transform.children.empty())
		{
			baseFlags |= ImGuiTreeNodeFlags_Leaf;
		}

		// If there is a node that is already selected, set the selected flag
		if (_selected == _go.uuid)
		{
			baseFlags |= ImGuiTreeNodeFlags_Selected;
		}

		nodeOpen = ImGui::TreeNodeEx(_go.get_name().c_str(), baseFlags);
/*
		//if (is->is_key_held(GLFW_KEY_LEFT_SHIFT))
		//{
		//	//PRINT("Holding shift");		
		//	// Handle any reordering
		//	if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
		//	{
		//		PRINT("Selected GameObject's Index:" << _index);
		//			//std::cout << "ID of selected Game Object: " << _selected << std::endl;
		//			int n_next = (ImGui::GetMouseDragDelta(0).y < 0.f ? -1 : 1);
		//			bool increment;
		//			if (n_next < 0)
		//			{
		//				increment = false;
		//				PRINT("neg");
		//			}
		//			else
		//			{
		//				increment = true;
		//				PRINT("pos");
		//			}

		//			n_next += _index;
		//			PRINT("n_next:" << n_next);
		//			PRINT("Generation size:" << _list.size());

		//			if (n_next >= 0 && n_next < _list.size())
		//			{		
		//				PRINT("test");
		//				Transform* tmp = &_go.transform;
		//				std::list<Transform*>::iterator iter1, iter2;

		//				iter1 = _list.begin();
		//				for (int i{ 0 }; i < _list.size(); ++i)
		//				{
		//					if (*iter1 == tmp)
		//					{
		//						PRINT("Found it");
		//						break;

		//					}

		//					++iter1;
		//				}
		//				iter2 = iter1;

		//				if (increment)
		//				{
		//					PRINT("increment");
		//					++iter2;
		//				}
		//				else
		//				{
		//					PRINT("decrement");
		//					--iter2;
		//				}

		//			

		//				if (iter2 != _list.end())
		//				{
		//					PRINT("Swapping " << (*iter1)->gameObj.get_name() << " and " << (*iter2)->gameObj.get_name());
		//					//*iter1 = *iter2;
		//					//*iter2 = tmp;
		//					std::swap(*iter1, *iter2);
		//				}

		//			

		//				ImGui::ResetMouseDragDelta();
		//			}

		//
		//	}
		//}
		//else
		//{
		*/

		if (ImGui::BeginDragDropSource())
		{
			static void* container;
			container = &_go;
			ImGui::SetDragDropPayload("GameObject", &container, sizeof(void*));
			ImGui::EndDragDropSource();

			//std::cout << "ID of selected Game Object: " << _selected << std::endl;
		}

		// Handle mouse clicks on item
		if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
		{
			std::cout << _go.get_name() << " is selected\n";
			_selected = _go.uuid;
			isSelected = true;
			sm->set_selected_gameobject(&_go);

		}
		else if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
		{
			std::cout << _go.get_name() << " is right clicked\n";
			_selected = _go.uuid;
			isSelected = true;
			sm->set_selected_gameobject(&_go);

			if (!isHierarchyPopUpOpen)
			{
				ImGui::OpenPopup("Options");
				isPopUpOpen = true;
			}

		}
		int res = 0;
		if (isPopUpOpen && _selected == _go.uuid)
		{
			 res = DisplayOptionsPopUpWindow();
		}


		if (nodeOpen)
		{
			if (res != 1 && !ImGui::IsItemToggledOpen())
			{
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
			}

			ImGui::TreePop();
		}



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

	void EditorHierarchyList::ShiftUp()
	{
		if (!sm->selectedGameObject)
			return;

		GameObject* target = sm->selectedGameObject;

		if (!target->transform.hasParent())
			return;

		// Erase from parent's children list
		GameObject* parent = &target->transform.parent->gameObj;
		for (std::list<Transform*>::iterator it = parent->transform.children.begin(); it != parent->transform.children.end(); ++it)
		{
			if (&target->transform == *it)
			{
				parent->transform.children.erase(it);
				break;
			}

		}

		// If this game object's parent is a root, simply turn this game object into a root
		if (!parent->transform.hasParent())
		{
			target->transform.parent = nullptr;
		}
		else
		{
			GameObject* grandparent = &parent->transform.parent->gameObj;
			grandparent->transform.children.push_back(&target->transform);
			target->transform.parent = &grandparent->transform;
		}


	}

	void EditorHierarchyList::ShiftDown()
	{
		if (!sm->get_selected_gameobject())
			return;

		//Scene* scene = sm->get_current_scene();
		GameObject* target = sm->get_selected_gameobject();

		// Do nothing if gameobject is in youngest generation
		if (target->transform.children.empty())
			return;


	}

	bool EditorHierarchyList::DisplayAdoptionWindow()
	{
		if (!sm->get_selected_gameobject())
			return false;

		PRINT("Display Adoption Window");
		Scene* scene = sm->get_current_scene();
		GameObject* target = sm->get_selected_gameobject();

		// Do nothing if gameobject is in youngest generation
		//if (target->transform.parent.empty())
		//	return false;

		GameObject* parent;
		if (target->transform.hasParent())
		{
			parent = &target->transform.parent->gameObj;
			if (isParenting)
			{
				// Open pop-up window
				ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
				ImGui::SetNextWindowPos(ImVec2(0.f, 0.f), ImGuiCond_FirstUseEver);
				ImGui::Begin("Adoption Drive", &isParenting);
				ImVec2 buttonSize = ImGui::GetWindowSize();
				buttonSize.y *= (float)0.1;
				static ImGuiTextFilter filter;
				ImGui::PushItemWidth(-1);
				filter.Draw("##Name");
				ImGui::PopItemWidth();
				for (std::list<Transform*>::iterator it = parent->transform.children.begin(); it != parent->transform.children.end(); ++it)
				{
					GameObject* go = &(*it)->gameObj;

					// Skip itself
					if (go == target)
						continue;

					if (ImGui::Button(go->get_name().c_str(), buttonSize))
					{
						if (filter.PassFilter(go->get_name().c_str()))
						{
							isParenting = false;

							(*it)->gameObj.transform.children.push_back(&target->transform);
							target->transform.parent = (*it);
							for (std::list<Transform*>::iterator iter = parent->transform.children.begin(); iter != parent->transform.children.end(); ++iter)
							{
								GameObject* go1 = &(*iter)->gameObj;

								// Skip itself
								if (go1 == target)
								{
									parent->transform.children.erase(iter);
									break;
								}

							}
						}
					}

				}

				ImGui::End();
			}

		}
		else
		{
			// Look through the roots
			// Open pop-up window
			if (isParenting)
			{

				ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
				ImGui::SetNextWindowPos(ImVec2(0.f, 0.f), ImGuiCond_FirstUseEver);
				ImGui::Begin("Adoption Drive", &isParenting);
				ImVec2 buttonSize = ImGui::GetWindowSize();
				buttonSize.y *= (float)0.1;
				static ImGuiTextFilter filter;
				ImGui::PushItemWidth(-1);
				filter.Draw("##Name");
				ImGui::PopItemWidth();
				for (size_t i{0}; i < scene->get_gameobjcount(); ++i)
				{
					PRINT("bloop");

					GameObject* go = scene->gameObjects[i];

					if (go->transform.hasParent() || go == target)
						continue;

					if (ImGui::Button(go->get_name().c_str(), buttonSize))
					{
						if (filter.PassFilter(go->get_name().c_str()))
						{
							isParenting = false;
							PRINT("bleep");

							go->transform.children.push_back(&target->transform);

							target->transform.parent = &go->transform;


						}
					}

				}
				ImGui::End();
			}


		}


		return true;

	}
	int EditorHierarchyList::DisplayOptionsPopUpWindow()
	{
		int result{ 0 };
		// Pop-up menu
		ImVec2 pos = ImGui::GetMousePos();
		ImGui::SetNextWindowPos(pos, ImGuiCond_Appearing, ImVec2(0.f, 0.f));
		if (ImGui::BeginPopup("Options"))
		{
			//isPopUpOpen = true;
			//PRINT("test");

			if (ImGui::MenuItem("Clone", "Ctrl+D"))
			{
				PRINT("Cloning game object");
				if (Copium::SceneManager::Instance()->get_selected_gameobject())
				{
					std::cout << "Clone\n";
					MyGOF.instantiate(*Copium::SceneManager::Instance()->get_selected_gameobject());
					MySceneManager.set_selected_gameobject(nullptr);
				}
				else
				{
					Window::EditorConsole::editorLog.add_logEntry("No Scene is Loaded!");
				}

				isPopUpOpen = false;
			}
			if (ImGui::MenuItem("Add a kid"))
			{
				PRINT("Plus one mouth to feed");
				if (MySceneManager.get_selected_gameobject())
				{
					MyGOF.create_child(*MySceneManager.get_selected_gameobject());
				}

				isPopUpOpen = false;
			}
			/*
			//if (ImGui::MenuItem("Delete"))
			//{
			//	PRINT("die bitch");
			//	if (Copium::SceneManager::Instance()->get_selected_gameobject())
			//	{
			//		std::cout << "Delete\n";
			//		Copium::UndoRedo::Command* tempUndo = new Copium::UndoRedo::GameObjectCommand(MySceneManager.get_selected_gameobject_sptr(), false);
			//		Copium::EditorSystem::Instance()->get_commandmanager()->undoStack.push(tempUndo);
			//		MyGOF.destroy(MySceneManager.get_selected_gameobject());
			//		MySceneManager.set_selected_gameobject(nullptr);
			//		result = 1;
			//	}
			//	else
			//	{
			//		Window::EditorConsole::editorLog.add_logEntry("Siao eh, no scene la");
			//	}
			//	//isPopUpOpen = false;
			//}
			//if (ImGui::MenuItem("Shift Up"))
			//{
			//	ShiftUp();
			//	result = 2;
			//	isPopUpOpen = false;
			//}
			//if (ImGui::MenuItem("Shift Down"))
			//{
			//	isParenting = true;
			//	
			//	isPopUpOpen = false;
			//}*/
			ImGui::EndPopup();
		}
		else
		{
			if (isPopUpOpen)
				isPopUpOpen = false;

			return -1;
		}
		//PRINT("End Display Options PopUp");
		return result;
	}
	int EditorHierarchyList::DisplayHierarchyOptionsPopUpWindow()
	{
		int result{ 0 };
		ImVec2 pos = ImGui::GetMousePos();
		ImGui::SetNextWindowPos(pos, ImGuiCond_Appearing, ImVec2(0.f, 0.f));
		if (ImGui::BeginPopup("HOps"))
		{
			//isPopUpOpen = true;
			if (ImGui::MenuItem("Create GameObject"))
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

			ImGui::EndPopup();
		}
		else
		{
			if (isHierarchyPopUpOpen)
			{
				isHierarchyPopUpOpen = false;
			}

			return -1;
		}


		return result;
	}
	void EditorHierarchyList::Reorder(GameObject* _go)
	{
		bool pressed{ false };
		bool increment{ false };
		if (is->is_key_pressed(GLFW_KEY_UP))
		{
			increment = false;
			pressed = true;
		}
		else if(is->is_key_pressed(GLFW_KEY_DOWN))
		{
			increment = true;
			pressed = true;
		}

		if (pressed == false)
			return;
		if (_go->transform.hasParent())
		{
			GameObject* parent = &_go->transform.parent->gameObj;
			Transform* pt = &parent->transform;
			std::list<Transform*>::iterator pos, dest;
			int idx{ 0 };
			for (std::list<Transform*>::iterator iter = pt->children.begin(); iter != pt->children.end(); ++iter)
			{
				GameObject* tmp = &(*iter)->gameObj;
				if (tmp->id == _go->id)
				{
					PRINT("FOUND");
					pos = iter;
					break;
				}
				++idx;
			}

			dest = pos;

			// Find source game object in the main vector
			size_t vecPos{ 0 }, vecDest{ 0 };
			Scene* scene = sm->get_current_scene();
			for (size_t i{ 0 }; i < scene->get_gameobjcount(); ++i)
			{
				if (_go == scene->gameObjects[i])
				{
					vecPos = i;
					break;
				}
			}

			if (increment)
			{				
				if (idx >= pt->children.size()-1)
					return;

				PRINT("Incrementing");

				++dest;
				std::swap(*pos, *dest);
				for (std::list<Transform*>::iterator iter = pt->children.begin(); iter != pt->children.end(); ++iter)
				{
					std::cout << (*iter)->gameObj.get_name() << std::endl;
				}

			}
			else
			{				
				if (idx == 0)
					return;

				PRINT("decrementing");
				--dest;
				std::swap(*dest, *pos);
				for (std::list<Transform*>::iterator iter = pt->children.begin(); iter != pt->children.end(); ++iter)
				{
					std::cout << (*iter)->gameObj.get_name() << std::endl;
				}
			}

			// Find destination game object in main vector
			for (size_t i{ 0 }; i < scene->get_gameobjcount(); ++i)
			{
				// Note that pos and dest had swapped position earlier so pos now points to the destination
				if ((*pos)->gameObj.id == scene->gameObjects[i]->id)
				{
					vecDest = i;
					break;
				}
			}

			if (vecPos >= scene->get_gameobjcount() || vecDest >= scene->get_gameobjcount())
				return;

			PRINT("Swapping game objects in main vector!" << vecPos << '|' << vecDest);
			std::swap(scene->gameObjects[vecPos], scene->gameObjects[vecDest]);

		}
		else
		{
			Scene* scene = sm->get_current_scene();
			if (!scene)
				return;

			size_t pos{ 0 };
			size_t dest{ 0 };

			for (size_t i{ 0 }; i < scene->get_gameobjcount(); ++i)
			{
				if (_go == scene->gameObjects[i])
				{
					pos = i;
					break;
				}
			}

			if (increment)
			{
				if (pos >= scene->get_gameobjcount() + 1)
					return;

				for (size_t i{ pos + 1 }; i < scene->get_gameobjcount(); ++i)
				{
					if (!scene->gameObjects[i]->transform.hasParent())
					{
						dest = i;
						break;
					}

				}
			}
			else
			{
				if (!pos)
					return;

				for (size_t i{ pos - 1 }; i >= 0; --i)
				{
					if (!scene->gameObjects[i]->transform.hasParent())
					{
						dest = i;
						break;
					}

				}
			}

			std::swap(scene->gameObjects[pos], scene->gameObjects[dest]);

		}
	}
}

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
#include <Events/events-system.h>
#include <Editor/editor-system.h>
#include <Editor/editor-consolelog.h>

namespace Copium
{

	namespace
	{
		InputSystem* is = InputSystem::Instance();
		std::string sceneName{};
	}
	void EditorHierarchyList::init()
	{
		isHierarchyOpen = true;
		isParenting = false;
		isPopUpOpen = false;

	}
	void EditorHierarchyList::update()
	{
		Scene* pScene{ MySceneManager.get_current_scene() };
		if (!isHierarchyOpen)
			return;

		ImGui::SetNextWindowSize(ImVec2(500, 900), ImGuiCond_FirstUseEver);
		ImGui::Begin("Hierarchy", &isHierarchyOpen, ImGuiWindowFlags_MenuBar);

		// Menu Bar

		if (ImGui::BeginMenuBar())
		{
			if (!pScene && ImGui::BeginMenu("Menu"))
			{
				if (ImGui::MenuItem("No scene loaded", nullptr))
				{
				}
				ImGui::EndMenu();
			}
			else if (ImGui::BeginMenu("Menu"))
			{

				//Add menu items
				if (ImGui::MenuItem("Create GameObject", nullptr))
				{
					Copium::GameObject* temp{};
					MyEventSystem->publish(new GameObjectInstantiateEvent(temp));
					//if (temp)
					//{
					//	std::shared_ptr<Copium::GameObject>* sptr = MySceneManager.find_gameobject_sptr(temp);
					//	if (sptr)
					//	{
					//		Copium::UndoRedo::Command* tempUndo = new Copium::UndoRedo::GameObjectCommand(*MySceneManager.find_gameobject_sptr(temp), true);
					//		Copium::EditorSystem::Instance()->get_commandmanager()->undoStack.push(tempUndo);
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
					if (MyEditorSystem.pSelectedGameObject)
					{
						//std::cout << "Delete\n";
						Copium::UndoRedo::Command* tempUndo = new Copium::UndoRedo::GameObjectCommand(MySceneManager.get_selected_gameobject_sptr(), false);
						Copium::EditorSystem::Instance()->get_commandmanager()->undoStack.push(tempUndo);
						MyEventSystem->publish(new GameObjectDestroyEvent(*MyEditorSystem.pSelectedGameObject));
						MyEditorSystem.pSelectedGameObject = nullptr;
					}
				}
				if (ImGui::MenuItem("Clone Selected GameObject", "Ctrl+D"))
				{
					if (MyEditorSystem.pSelectedGameObject)
					{
						Copium::GameObject* temp{};
						MyEventSystem->publish(new GameObjectInstantiateEvent(temp, MyEditorSystem.pSelectedGameObject));
					}
				}
				//if (ImGui::MenuItem("Create a Child GameObject"))
				//{
				//	if (MySceneManager.get_selected_gameobject())
				//	{
				//		MyGOF.create_child(*MySceneManager.get_selected_gameobject());
				//	}
				//}
				if (ImGui::MenuItem("Shift Up"))
				{
					ShiftUp();
				}
				if (ImGui::MenuItem("Shift Down"))
				{
					isParenting = true;
					PRINT("Shift down");
				}

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		if (isParenting)
			DisplayAdoptionWindow();

		//KeyBoard Shortcuts//
		if (is->is_key_held(GLFW_KEY_LEFT_CONTROL) && is->is_key_pressed(GLFW_KEY_D))
		{
			if (MyEditorSystem.pSelectedGameObject)
			{
				std::cout << "Clone\n";
				Copium::GameObject* temp{};
				MyEventSystem->publish(new GameObjectInstantiateEvent(temp, MyEditorSystem.pSelectedGameObject));
			}
			else
			{
				Window::EditorConsole::editorLog.add_logEntry("No scene is loaded!");
			}
		}
		if (is->is_key_pressed(GLFW_KEY_DELETE) && ImGui::IsWindowFocused())
		{
			if (MyEditorSystem.pSelectedGameObject)
			{
				std::cout << "Delete\n";
				Copium::UndoRedo::Command* tempUndo = new Copium::UndoRedo::GameObjectCommand(MySceneManager.get_selected_gameobject_sptr(), false);
				Copium::EditorSystem::Instance()->get_commandmanager()->undoStack.push(tempUndo);
				MyEventSystem->publish(new GameObjectDestroyEvent(*MyEditorSystem.pSelectedGameObject));
				MyEditorSystem.pSelectedGameObject = nullptr;
			}
		}
		if (is->is_key_held(GLFW_KEY_LEFT_SHIFT))
		{
			Reorder(MyEditorSystem.pSelectedGameObject);
		}
		///////////////////


		ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 10.f);
		ImGuiTreeNodeFlags rootFlags = ImGuiTreeNodeFlags_DefaultOpen;




		// Ensure that game objects are displayed only if there is a current scene loaded
		if (pScene)
		{		
			//PRINT("Start Editor Update");
			// Find all the root nodes
			GameObjectsPtrArray roots{};
			for (GameObject& gameObject : pScene->gameObjects)
			{
				if (!gameObject.transform.HasParent())
				{
					roots.push_back(&gameObject);
				}

			}

			if (!pScene->gameObjects.empty())
				rootFlags |= ImGuiTreeNodeFlags_Leaf;

			//if (sm->GetSceneState() == Copium::Scene::SceneState::play)
			//{
			//	sceneName += "\t PREVIEWING";
			//}

			int test{ 0 };
			// Display scene name as the rootiest node
			bool nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)test, rootFlags, pScene->name.c_str());


			if (nodeOpen)
			{			
				//bool isSelected = false;
				for (size_t i{ 0 }; i < roots.size(); ++i)
				{
					display_gameobject(*(roots[i]), roots, i);

				}
				ImGui::TreePop();

			}
			
			//PRINT("End Editor Update");
		} 

		if (ImGui::IsWindowHovered())
		{
			//PRINT("Hovering over hierarchy list");
			if (is->is_mousebutton_pressed(GLFW_MOUSE_BUTTON_RIGHT))
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
	}

	bool EditorHierarchyList::display_gameobject(GameObject& _go, std::vector<GameObject*>& _vector, size_t _index)
	{
		bool isSelected = false, nodeOpen = false;
		ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_OpenOnDoubleClick;


		// If root node does not have children, it is simply a leaf node (end of the branch)
		if (_go.transform.children.empty())
		{
			baseFlags |= ImGuiTreeNodeFlags_Leaf;
		}

		// If there is a node that is already selected, set the selected flag
		if (MyEditorSystem.pSelectedGameObject == &_go)
		{
			baseFlags |= ImGuiTreeNodeFlags_Selected;
		}

		nodeOpen = ImGui::TreeNodeEx(_go.name.c_str(), baseFlags);
/*
		if (is->is_key_held(GLFW_KEY_LEFT_SHIFT))
		{
			if (ImGui::IsItemActive() && !ImGui::IsItemHovered())
			{
				if (!_go.transform.HasParent())
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
							if (temp == currentScene->gameObjectects[i])
							{
								idx1 = i;
								break;
							}
						}

						for (size_t i{ 0 }; i < currentScene->get_gameobjcount(); ++i)
						{
							if (&_go == currentScene->gameObjectects[i])
							{
								idx2 = i;
								break;
							}
						}

						currentScene->gameObjectects[idx1] = currentScene->gameObjectects[idx2];
						currentScene->gameObjectects[idx2] = temp;
			
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
		}
		else if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
		{
			PRINT("selected");
			std::cout << _go.name << " is selected\n";
			isSelected = true;
			MyEditorSystem.pSelectedGameObject = &_go;
		}
		else if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
		{
			std::cout << _go.name << " is right clicked\n";
			isSelected = true;
			MyEditorSystem.pSelectedGameObject = &_go;

			if (!isHierarchyPopUpOpen)
			{
				ImGui::OpenPopup("Options");
				isPopUpOpen = true;
			}

		}	
		int res = 0;
		if (isPopUpOpen && MyEditorSystem.pSelectedGameObject == &_go)
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

						isSelected = display_gameobject(pChild->gameObject, _go.transform.children, idx);
						++idx;
					}
				}
			}

			ImGui::TreePop();
		}


		return isSelected;
	}
	bool EditorHierarchyList::display_gameobject(GameObject& _go, std::list<Transform*>& _list, size_t _index)
	{
		bool isSelected = false, nodeOpen = false;
		ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;


		// If root node does not have children, it is simply a leaf node (end of the branch)
		if (_go.transform.children.empty())
		{
			baseFlags |= ImGuiTreeNodeFlags_Leaf;
		}

		// If there is a node that is already selected, set the selected flag
		if (MyEditorSystem.pSelectedGameObject == &_go)
		{
			baseFlags |= ImGuiTreeNodeFlags_Selected;
		}

		nodeOpen = ImGui::TreeNodeEx(_go.name.c_str(), baseFlags);
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
		//					PRINT("Swapping " << (*iter1)->gameObject.name << " and " << (*iter2)->gameObject.name);
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
			std::cout << _go.name << " is selected\n";
			isSelected = true;
			MyEditorSystem.pSelectedGameObject = &_go;

		}
		else if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
		{
			std::cout << _go.name << " is right clicked\n";
			isSelected = true;
			MyEditorSystem.pSelectedGameObject = &_go;

			if (!isHierarchyPopUpOpen)
			{
				ImGui::OpenPopup("Options");
				isPopUpOpen = true;
			}

		}
		int res = 0;
		if (isPopUpOpen && MyEditorSystem.pSelectedGameObject == &_go)
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
						isSelected = display_gameobject(pChild->gameObject, _go.transform.children, idx);
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
			//if (!MyGOF.instantiate())
			//	std::cout << "Error creating game object\n";


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
		if (!MyEditorSystem.pSelectedGameObject)
			return;

		GameObject* target = MyEditorSystem.pSelectedGameObject;

		if (!target->transform.HasParent())
			return;

		// Erase from parent's children list
		GameObject* parent = &target->transform.parent->gameObject;
		for (std::list<Transform*>::iterator it = parent->transform.children.begin(); it != parent->transform.children.end(); ++it)
		{
			if (&target->transform == *it)
			{
				parent->transform.children.erase(it);
				break;
			}

		}

		// If this game object's parent is a root, simply turn this game object into a root
		if (!parent->transform.HasParent())
		{
			target->transform.parent = nullptr;
		}
		else
		{
			GameObject* grandparent = &parent->transform.parent->gameObject;
			grandparent->transform.children.push_back(&target->transform);
			target->transform.parent = &grandparent->transform;
		}


	}

	void EditorHierarchyList::ShiftDown()
	{
		if (!MyEditorSystem.pSelectedGameObject)
			return;

		//Scene* scene = sm->get_current_scene();
		GameObject* target = MyEditorSystem.pSelectedGameObject;

		// Do nothing if gameobject is in youngest generation
		if (target->transform.children.empty())
			return;


	}

	bool EditorHierarchyList::DisplayAdoptionWindow()
	{
		if (!MyEditorSystem.pSelectedGameObject)
			return false;

		PRINT("Display Adoption Window");
		GameObject* target = MyEditorSystem.pSelectedGameObject;

		// Do nothing if gameobject is in youngest generation
		//if (target->transform.parent.empty())
		//	return false;
		Scene* pScene{ MySceneManager.get_current_scene() };

		GameObject* parent;
		if (target->transform.HasParent())
		{
			parent = &target->transform.parent->gameObject;
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
					GameObject* go = &(*it)->gameObject;

					// Skip itself
					if (go == target)
						continue;

					if (ImGui::Button(go->name.c_str(), buttonSize))
					{
						if (filter.PassFilter(go->name.c_str()))
						{
							isParenting = false;

							(*it)->gameObject.transform.children.push_back(&target->transform);
							target->transform.parent = (*it);
							for (std::list<Transform*>::iterator iter = parent->transform.children.begin(); iter != parent->transform.children.end(); ++iter)
							{
								GameObject* go1 = &(*iter)->gameObject;

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
				for (GameObject& gameObject : pScene->gameObjects)
				{
					if (gameObject.transform.HasParent() || &gameObject == target)
						continue;

					if (filter.PassFilter(gameObject.name.c_str()) && ImGui::Button(gameObject.name.c_str(), buttonSize))
					{
						isParenting = false;
						target->transform.SetParent(&gameObject.transform);
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
				if (MyEditorSystem.pSelectedGameObject)
				{
					std::cout << "Clone\n";
					GameObject* tmp{};
					MyEventSystem->publish(new GameObjectInstantiateEvent(tmp, MyEditorSystem.pSelectedGameObject));
				}
				else
				{
					Window::EditorConsole::editorLog.add_logEntry("No Scene is Loaded!");
				}

				isPopUpOpen = false;
			}
			//if (ImGui::MenuItem("Add a kid"))
			//{
			//	PRINT("Plus one mouth to feed");
			//	if (MySceneManager.get_selected_gameobject())
			//	{
			//		MyGOF.create_child(*MySceneManager.get_selected_gameobject());
			//	}

			//	isPopUpOpen = false;
			//}
			/*
			//if (ImGui::MenuItem("Delete"))
			//{
			//	PRINT("die bitch");
			//	if (MyEditorSystem.pSelectedGameObject)
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
		Scene* pScene{ MySceneManager.get_current_scene() };
		if (ImGui::BeginPopup("HOps"))
		{
			//isPopUpOpen = true;
			if (ImGui::MenuItem("Create GameObject"))
			{
				if (!pScene)
				{
					Window::EditorConsole::editorLog.add_logEntry("Siao eh, no scene la");
				}
				else
				{
					Copium::GameObject* temp{};
					MyEventSystem->publish(new GameObjectInstantiateEvent(temp, nullptr));
					//if (temp)
					//{
					//	std::shared_ptr<Copium::GameObject>* sptr = MySceneManager.find_gameobject_sptr(temp);
					//	if (sptr)
					//	{
					//		Copium::UndoRedo::Command* tempUndo = new Copium::UndoRedo::GameObjectCommand(*MySceneManager.find_gameobject_sptr(temp), true);
					//		Copium::EditorSystem::Instance()->get_commandmanager()->undoStack.push(tempUndo);
					//		tempUndo->printCommand();
					//	}

					//}
					//else
					//{
					//	std::cout << "Error creating game object\n";
					//}
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
		if (_go->transform.HasParent())
		{
			GameObject* parent = &_go->transform.parent->gameObject;
			Transform* pt = &parent->transform;
			std::list<Transform*>::iterator pos, dest;
			int idx{ 0 };
			for (std::list<Transform*>::iterator iter = pt->children.begin(); iter != pt->children.end(); ++iter)
			{
				GameObject* tmp = &(*iter)->gameObject;
				if (tmp->uuid == _go->uuid)
				{
					PRINT("FOUND");
					pos = iter;
					break;
				}
				++idx;
			}

			dest = pos;

			if (increment)
			{				
				if (idx >= pt->children.size()-1)
					return;

				PRINT("Incrementing");

				++dest;
				std::swap(*pos, *dest);
				for (std::list<Transform*>::iterator iter = pt->children.begin(); iter != pt->children.end(); ++iter)
				{
					std::cout << (*iter)->gameObject.name << std::endl;
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
					std::cout << (*iter)->gameObject.name << std::endl;
				}
			}

			//UUID uuid{ (*pos)->gameObject.uuid };
			//GameObject* rhs{};
			// Find destination game object in main vector
			//for (GameObject& gameObject: *pGameObjectsArray)
			//{
			//	// Note that pos and dest had swapped position earlier so pos now points to the destination
			//	if (uuid == scene->gameObjectects[i].uuid)
			//	{
			//		rhs = &gameObject;
			//		break;
			//	}
			//}

			//if (vecPos >= scene->get_gameobjcount() || vecDest >= scene->get_gameobjcount())
			//	return;

			Scene* pScene{ MySceneManager.get_current_scene() };
			pScene->gameObjects.swap(*_go, (*pos)->gameObject);
		}
		else
		{
			Scene* pScene = MySceneManager.get_current_scene();
			if (!pScene)
				return;
			size_t pos{ 0 };
			size_t dest{ 0 };
			for (size_t i{ 0 }; i < pScene->gameObjects.size(); ++i)
			{
				if (_go == &pScene->gameObjects[i])
				{
					pos = i;
					break;
				}
			}
			if (increment)
			{
				if (pos >= pScene->gameObjects.size() + 1)
					return;
				for (size_t i{ pos + 1 }; i < pScene->gameObjects.size(); ++i)
				{
					if (!pScene->gameObjects[i].transform.HasParent())
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
					if (!pScene->gameObjects[i].transform.HasParent())
					{
						dest = i;
						break;
					}
				}
			}
			std::swap(pScene->gameObjects[pos], pScene->gameObjects[dest]);

		}
	}
}

#include "pch.h"
#include "Editor/editor-hierarchy-list.h"

//M2
namespace Window::Hierarchy
{
	Scene* currentScene = nullptr;
	bool isOpen;
	int selectedID = -1;

	bool showAddGameObject, showDeleteGameObject;

	void init()
	{
		isOpen = true;
		showAddGameObject = false;
		showDeleteGameObject = false;
		if (Copium::NewSceneManager::Instance())
			currentScene = Copium::NewSceneManager::Instance()->get_current_scene();


	}

	void update()
	{
		if (!isOpen)
			return;

		// Handle scene change
		if (currentScene != Copium::NewSceneManager::Instance()->get_current_scene())
			currentScene = Copium::NewSceneManager::Instance()->get_current_scene();


		ImGui::SetNextWindowSize(ImVec2(500, 900), ImGuiCond_FirstUseEver);
		ImGui::Begin("Hierarchy", &isOpen, ImGuiWindowFlags_MenuBar);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Menu"))
			{
				// Add menu items
				if (ImGui::MenuItem("Add a GameObject", nullptr, showAddGameObject))
				{
					std::cout << "Add\n";
					if (!Copium::NewSceneManager::Instance()->get_gof().build_gameobject())
						std::cout << "Error creating game object\n";
				}
				if (ImGui::MenuItem("Delete Selected GameObject", nullptr, showDeleteGameObject))
				{
					if (Copium::NewSceneManager::Instance()->get_selected_gameobject())
					{
						std::cout << "Delete\n";
						Copium::NewSceneManager::Instance()->get_gof().delete_gameobject(Copium::NewSceneManager::Instance()->get_selected_gameobject());
						Copium::NewSceneManager::Instance()->set_selected_gameobject(nullptr);
					}
					else
					{
						std::cout << "no game object selected, can't delete\n";
					}
					
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 10.f);

		//create_gameobject_btn("Add a GameObject");
		//ImGui::SameLine;
		//create_delete_btn("Delete GameObject");

		ImGuiTreeNodeFlags rootFlags = ImGuiTreeNodeFlags_DefaultOpen;

		if (currentScene)
		{


			// Find all the root nodes
			std::vector<GameObject*>roots;
			for (size_t i{ 0 }; i < currentScene->get_gameobjcount(); ++i)
			{
				GameObject* tmp = currentScene->get_gameobjectvector()[i];
				if (!tmp->has_parent())
					roots.push_back(tmp);
			}

			if (!currentScene->get_gameobjcount())
				rootFlags |= ImGuiTreeNodeFlags_Leaf;

			// Display scene name as the rootiest node
			if (ImGui::TreeNodeEx(currentScene->get_name().c_str(), rootFlags))
			{
				bool isSelected = false;

				for (size_t i{ 0 }; i < roots.size(); ++i)
				{
					isSelected = display_gameobject_advanced(*roots[i], selectedID);
				}	

				ImGui::TreePop();
			}

		}


		ImGui::PopStyleVar();			
		ImGui::End();
	}
	
	void display_gameobject(const GameObject& _go)
	{

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
		// Prevent arrow from showing up which will cause assert if no children
		if (!_go.is_parent())
			flags |= ImGuiTreeNodeFlags_Leaf;
		else
		{
			flags |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		}

		bool result = ImGui::TreeNodeEx(_go.get_name().c_str(), flags);
		if (!result)
			return;

		// If game object has children, recursively display children
		if(_go.is_parent())
		{
			for (std::list<GameObject*>::const_iterator iter = _go.childList().begin(); iter != _go.childList().end(); ++iter)
			{
				display_gameobject(**iter);
			}
		}

		// Remember to pop for every tree node created!!!
		ImGui::TreePop();

	}
	bool display_gameobject_advanced(GameObject& _go, int& _selected)
	{
		bool isSelected = false;
		ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		int node_clicked = -1;

		if (!_go.is_parent())
		{
			baseFlags |= ImGuiTreeNodeFlags_Leaf;
		}
			

		if (_selected == _go.id)
		{
			baseFlags |= ImGuiTreeNodeFlags_Selected;
		}
			

		if (!ImGui::TreeNodeEx(_go.get_name().c_str(), baseFlags))
			return false;

		if (ImGui::IsItemClicked())
		{
			if (!isSelected)
			{
				std::cout << _go.get_name() << " is selected\n";
				_selected = _go.id;
				isSelected = true;
				Copium::NewSceneManager::Instance()->set_selected_gameobject(&_go);
			}

		}
		// If game object has children, recursively display children
		if (_go.is_parent())
		{
			for (std::list<GameObject*>::const_iterator iter = _go.childList().begin(); iter != _go.childList().end(); ++iter)
			{
				isSelected = display_gameobject_advanced(**iter, _selected);
			}
		}

		ImGui::TreePop();

		if (ImGui::IsItemClicked())
			std::cout << "clicking\n";

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
			if (!Copium::NewSceneManager::Instance()->get_gof().build_gameobject())
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


#include "pch.h"
#include "Editor/editor-scene-hierarchy-list.h"

//M2

namespace Window::Hierarchy
{
	const Scene* currentScene = nullptr;
	bool isOpen;
	void init()
	{
		isOpen = true;
		//if (Copium::NewSceneManager::Instance())
			//currentScene = Copium::NewSceneManager::Instance()->get_current_scene();


	}

	void update()
	{
		if (!isOpen)
			return;
		ImGui::SetNextWindowSize(ImVec2(500, 900), ImGuiCond_FirstUseEver);
		ImGui::Begin("Hierarchy");
		ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 10.f);
		int id = 0;

		for (int i{ 0 }; i < 2; ++i, ++id)
		{
			bool isSelected = false;
			ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;			
			if (i % 2)
				flags |= ImGuiTreeNodeFlags_Leaf;
			else
				flags |= ImGuiTreeNodeFlags_OpenOnArrow;
			std::string tmp = std::to_string(i);

			if (ImGui::IsItemClicked())
				flags |= ImGuiTreeNodeFlags_Selected;

			bool nodeStatus = ImGui::TreeNodeEx(tmp.c_str(), flags);
			if (nodeStatus && !(i%2))
			{
				ImGuiTreeNodeFlags childFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Leaf;
				tmp += 'C';
				ImGui::TreeNodeEx(tmp.c_str(), childFlags);
				ImGui::TreePop();
			}



			ImGui::TreePop();

		}

		ImGui::PopStyleVar();
		ImGui::End();
	}
	/*
	void display_gameobject(const GameObject& _go)
	{

		bool isSelected = false;

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
		// Prevent arrow from showing up which will cause assert if no children
		if (!_go.is_parent())
			flags |= ImGuiTreeNodeFlags_Leaf;
		else
		{
			flags |= ImGuiTreeNodeFlags_OpenOnArrow;
		}

		if (ImGui::IsItemClicked())
			isSelected = true;

		if (isSelected)
			flags |= ImGuiTreeNodeFlags_Selected;

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

	}*/
}

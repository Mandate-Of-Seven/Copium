/*!***************************************************************************************
\file			editor-content-browser.cpp
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			01/11/2022

\brief
	Contains function definitions for the editor content browser.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/
#include "pch.h"

#include <algorithm>
#include "Editor/editor-content-browser.h"
#include "Messaging/message-system.h"

namespace Copium
{
	namespace
	{
		std::filesystem::path assets = "../PackedTracks/Assets";

		const float padding = 16.f;
		const float thumbnailSize = 128.f;
		float cellSize = thumbnailSize + padding;

		int finalCount = 0;
	}

	void EditorContentBrowser::init()
	{
		currentDirectory = assets;

		Texture directoryIcon("Data/DirectoryIcon.png");
		Texture fileIcon("Data/FileIcon.png");
		 
		icons.push_back(directoryIcon);
		icons.push_back(fileIcon);
	}

	void EditorContentBrowser::update()
	{
		ImGui::Begin("Content Browser");

		if (currentDirectory != std::filesystem::path(assets))
		{
			if (ImGui::Button("Back"))
			{
				currentDirectory = currentDirectory.parent_path();
			}
		}
		else
		{
			if (ImGui::Button("Reload"))
			{
				MessageSystem::Instance()->dispatch(MESSAGE_TYPE::MT_RELOAD_ASSETS);
			}
		}

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		
		if (columnCount < 1)
			columnCount = 1;

		int fileCount = 0;
		for (auto& dirEntry : std::filesystem::directory_iterator(currentDirectory))
		{
			(void) dirEntry;
			fileCount++;
		}

		if (fileCount >= columnCount)
			finalCount = columnCount;
		else
			finalCount = fileCount;

		ImGuiTableFlags tableFlags = ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_SizingStretchProp;
		if (ImGui::BeginTable("Table: Content Browser", finalCount, tableFlags))
		{
			ImGui::TableSetupColumn("Column 0: Files", 0, 0.01f);
			for (int i = 1; i < finalCount; i++)
			{
				std::string str = "Column " + i;
				str += ": Files";
				ImGui::TableSetupColumn(str.c_str(), 0, 1.f / finalCount);
			}
			ImGui::TableNextColumn();
			for (auto& dirEntry : std::filesystem::directory_iterator(currentDirectory))
			{
				if (ImGui::TableGetColumnIndex() >= finalCount - 1)
				{
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
				}
				ImGui::TableNextColumn();

				const auto& path = dirEntry.path();
				auto relativePath = std::filesystem::relative(path, assets);
				std::string fileName = relativePath.filename().string();

				ImGui::PushID(fileName.c_str());

				unsigned int index = (dirEntry.is_directory()) ? 0 : 1;
				ImTextureID icon = (ImTextureID)(size_t)icons[index].get_object_id();
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				ImGui::ImageButton(icon, { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 });
				
				if (ImGui::BeginDragDropSource())
				{
					std::string str = path.generic_string();
					const char* filePath = str.c_str();
					ImGui::SetDragDropPayload("ContentBrowserItem", filePath, str.size() + 1);
					
					ImGui::EndDragDropSource();
				}

				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					if (dirEntry.is_directory())
					{
						currentDirectory /= path.filename();
					}
				}
				ImGui::PopStyleColor();

				float textWidth = ImGui::CalcTextSize(fileName.c_str()).x;
				float indent = (cellSize - textWidth) * 0.5f;
				ImGui::Indent(indent);
				ImGui::Text(fileName.c_str());
				ImGui::PopID();
			}

			ImGui::EndTable();
		}

		ImGui::End();
	}

	void EditorContentBrowser::exit()
	{

	}
}
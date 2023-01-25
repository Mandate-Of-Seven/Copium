/*!***************************************************************************************
\file			editor-content-browser.cpp
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			01/11/2022

\brief
	Contains function definitions for the editor content browser, where the user can view
	and interact with the items in the assets folder.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/
#include "pch.h"

#include "Editor/editor-content-browser.h"
#include "Editor/editor-system.h"
#include "Messaging/message-system.h"
#include "Files/file-system.h"
#include "Files/assets-system.h"

namespace Copium
{
	namespace
	{
		EditorSystem* editor = EditorSystem::Instance();
		FileSystem* fs = FileSystem::Instance();
		AssetsSystem* assetSys = AssetsSystem::Instance();

		std::filesystem::path assets = "../PackedTracks/Assets";

		const float padding = 16.f;
		const float thumbnailSize = 128.f;
		float imageAR = 1.f, framePadding = 3.f;
		float cellSize = thumbnailSize + padding;

		//std::filesystem::path currentDirectory;

		std::vector<Texture> icons;
	}

	void EditorContentBrowser::init()
	{
		currentDirectory = &fs->get_asset_directory();

		Texture directoryIcon("Data/Resource/DirectoryIcon.png");
		Texture fileIcon("Data/Resource/FileIcon.png");
		Texture engineLogo("Data/Resource/CopiumLogo.png");
		 
		icons.push_back(directoryIcon);
		icons.push_back(fileIcon);
		icons.push_back(engineLogo);
	}

	void EditorContentBrowser::update()
	{
		ImGui::Begin("Content Browser", 0);

		inputs();

		if (currentDirectory->path() != std::filesystem::path(assets))
		{
			if (ImGui::Button("Back"))
			{
				currentDirectory = currentDirectory->get_parent_directory();
			}
		}
		else
		{
			if (ImGui::Button("Reload"))
			{
				MessageSystem::Instance()->dispatch(MESSAGE_TYPE::MT_RELOAD_ASSETS);
			}
		}

		ImGui::SameLine();
		if (ImGui::BeginMenu("Add Assets"))
		{
			if (currentDirectory != nullptr)
			{
				// Length here would be the number of scriptable objects
				std::vector<std::string> scriptableObjects = assetSys->GetScriptableObjects();

				for (int i = 0; i < scriptableObjects.size(); i++)
				{
					std::string assetName = scriptableObjects[i];
					if (ImGui::MenuItem(assetName.c_str(), nullptr))
					{
						// Find script in relation to the assetname
						std::list<File> scriptFiles = fs->get_files_with_extension(".cs");
						for (File file : scriptFiles)
						{
							if (!file.stem().string().compare(assetName))
							{
								// Copy the script file but change the extension
								assetSys->CopyAsset(file, ".asset");
								break;
							}
						}
					}
				}
			}
			ImGui::EndMenu();
		}

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);

		if (columnCount < 1)
			columnCount = 1;

		ImGuiTableFlags tableFlags = ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_SizingStretchSame
			| ImGuiTableFlags_PreciseWidths | ImGuiTableFlags_NoPadInnerX;

		if (ImGui::BeginTable("Table: Content Browser", columnCount, tableFlags))
		{
			ImGuiTableColumnFlags columnFlags = ImGuiTableColumnFlags_WidthFixed;
			for (int i = 0; i < columnCount; i++)
			{
				std::string str = "Column " + i;
				str += ": Assets";

				ImGui::TableSetupColumn(str.c_str(), columnFlags, cellSize);
			}

			// Only for buttons
			const ImVec4 white(1.f, 1.f, 1.f, 1.f);
			const ImVec4 transparent(0.f, 0.f, 0.f, 0.f);

			// Directory/Folder iterator
			for (auto dirEntry : currentDirectory->get_child_directory())
			{
				if (ImGui::TableGetColumnIndex() >= columnCount - 1)
				{
					ImGui::TableNextRow();
				}

				ImGui::TableNextColumn();

				const auto& path = dirEntry->path();
				auto relativePath = std::filesystem::relative(path, assets);
				std::string fileName = relativePath.filename().string();

				ImGui::PushID(fileName.c_str());
				ImGui::BeginGroup();

				ImTextureID icon = (ImTextureID)(size_t)icons[0].get_object_id();
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.f, 0.f));
				ImGui::ImageButtonEx(dirEntry->get_id(), icon, { thumbnailSize, thumbnailSize }, { 0, 1 }, { 1, 0 }, transparent, white);
				
				//ImGui::PopStyleVar();
				ImGui::PopStyleColor();

				float textWidth = ImGui::CalcTextSize(fileName.c_str()).x;
				float indent = abs((cellSize - textWidth - padding) * 0.5f);
				ImGui::Indent(indent);
				ImGui::Text(fileName.c_str());

				ImGui::EndGroup();
				ImGui::PopID();
			}

			// File iterator
			for (auto& file : currentDirectory->get_files())
			{
				if (ImGui::TableGetColumnIndex() >= columnCount - 1)
				{
					ImGui::TableNextRow();
				}
				ImGui::TableNextColumn();

				ImGui::PushID(file.get_id());
				ImGui::BeginGroup();

				// Get the image icon
				unsigned int objectID = icons[1].get_object_id();
				for (unsigned int i = 0; i < assetSys->get_textures().size(); i++)
				{
					std::string texturePath;
					switch (file.get_file_type().fileType)
					{
					case Copium::AUDIO:
						break;

					case Copium::FONT:
						break;

					case Copium::SCENE:
						objectID = icons[2].get_object_id();
						imageAR = 1.f;
						framePadding = 3.f;
						break;

					case Copium::SCRIPT:
						break;

					case Copium::SHADER:
						break;

					case Copium::SPRITE:
						texturePath = assetSys->get_texture(i)->get_file_path();
						if (!file.generic_string().compare(texturePath))
						{
							Texture* temp = assetSys->get_texture(i);
							objectID = temp->get_object_id();
							float asRatio = temp->get_width() / (float)temp->get_height();
							imageAR = thumbnailSize / ((asRatio > 0.98f && asRatio < 1.f) ? 1.f : asRatio);
							imageAR /= thumbnailSize;
							framePadding = (thumbnailSize - thumbnailSize * imageAR) * 0.5f + 3.f;
						}
						break;

					case Copium::TEXT:
						objectID = icons[1].get_object_id();
						imageAR = 1.f;
						framePadding = 3.f;
						break;
					}
				}

				ImTextureID icon = (ImTextureID)(size_t)objectID;
				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0.f, framePadding));

				ImGui::ImageButtonEx(file.get_id(),icon, { thumbnailSize, thumbnailSize * imageAR }, { 0, 1 }, { 1, 0 }, transparent, white);

				if (ImGui::BeginDragDropSource())
				{
					std::string str = file.generic_string();
					const char* filePath = str.c_str();
					ImGui::SetDragDropPayload("ContentBrowserItem", filePath, str.size() + 1);

					ImGui::EndDragDropSource();
				}

				ImGui::PopStyleVar();
				ImGui::PopStyleColor();

				float textWidth = ImGui::CalcTextSize(file.get_name().c_str()).x;
				float indent = (cellSize - textWidth - padding) * 0.5f;
				ImGui::Indent(indent);
				ImGui::Text(file.get_name().c_str());

				ImGui::EndGroup();
				ImGui::PopID();
			}

			ImGui::EndTable();
		}

		ImGui::End();
	}

	void EditorContentBrowser::exit()
	{

	}

	void EditorContentBrowser::inputs()
	{
		if (!ImGui::IsMouseDragging(ImGuiMouseButton_Left) && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		{
			if (!editor->get_inspector()->getFocused())
			{
				fs->set_selected_file(nullptr);
				fs->set_selected_directory(nullptr);
			}
		}

		if (ImGui::IsWindowFocused())
		{
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				for (File& file : currentDirectory->get_files())
				{
					if (file.get_id() == ImGui::GetHoveredID())
						file.access_file();
				}

				for (Directory* dir : currentDirectory->get_child_directory())
				{
					if (dir->get_id() == ImGui::GetHoveredID())
						currentDirectory = dir;
				}
			}

			if (!ImGui::IsMouseDragging(ImGuiMouseButton_Left) && ImGui::IsMouseReleased(ImGuiMouseButton_Left))
			{
				for (File& file : currentDirectory->get_files())
				{
					if (file.get_id() == ImGui::GetHoveredID())
					{
						fs->set_selected_file(&file);
					}
				}

				for (Directory* dir : currentDirectory->get_child_directory())
				{
					if (dir->get_id() == ImGui::GetHoveredID())
						fs->set_selected_directory(dir);
				}
			}
			
			if (ImGui::IsKeyPressed(ImGuiKey_Delete))
			{
				fs->delete_from_browser();
			}
		}
	}
}
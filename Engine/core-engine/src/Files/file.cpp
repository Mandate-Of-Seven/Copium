/*!***************************************************************************************
\file			file.cpp
\project
\author			Sean Ngo

\par			Course: GAM250
\par			Section:
\date			10/11/2022

\brief
	Contains function definitions for files

All content � 2023 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/
#include "pch.h"

#include "Files/file.h"
#include "Windows/windows-utils.h"
#include "SceneManager/scene-manager.h"

// Bean: Temporary for executing file
//#include <shellapi.h>

namespace Copium
{
	File::File() : filePath{}
	{

		modified = true;
		//PRINT(uuid);
	}

	File::File(const std::filesystem::path& pathRef) : filePath(pathRef)
	{
		modified = true;
		//PRINT(uuid);

	}

	bool File::is_modified()
	{
		if (modified)
		{
			modified = false;
			return true;
		}
		return modified;
	}

	void File::update_modification_timing()
	{
		struct _stat64i32 statsBuffer;
		_stat(filePath.string().c_str(), &statsBuffer);
		if (lastModifiedTime != statsBuffer.st_mtime)
		{
			modified = true;
			lastModifiedTime = statsBuffer.st_mtime;
		}
	}

	void File::access_file()
	{
		if (fileType.fileType == FILE_TYPE::SCENE)
		{
			if (Copium::SceneManager::Instance()->load_scene(filePath.string().c_str()))
				PRINT("Loading success");
			else
				PRINT("loading fail");
		}
		else
		{
			MyEventSystem->publish(new FileAccessEvent(filePath.string().c_str()));
		}

		/*switch (fileType.fileType)
		{
		case AUDIO:
			break;
		case FONT:
			break;
		case SCENE:
			break;
		case SCRIPT:
			break;
		case SHADER:
			break;
		case SPRITE:
			break;
		case TEXT:
			break;
		default:
			break;
		}*/
	}

	void File::inspector_view()
	{
		// Display file's parameters
		if (ImGui::BeginTable("Table: File View", 1))
		{
			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			std::string str = "Instance ID: " + std::to_string(instanceID);
			ImGui::Text(str.c_str());

			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			str = "File name: " + name;
			ImGui::Text(str.c_str());

			ImGui::TableNextRow();
			ImGui::TableNextColumn();

			str = "File type: ";
			str	+= fileType.stringType;
			ImGui::Text(str.c_str());

			ImGui::EndTable();
		}

		if (fileType.fileType == FILE_TYPE::ASSET)
		{
			std::string str = "Placeholder 01";
			std::string str2 = "Placeholder 02";

			ImGuiWindowFlags windowFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody
				| ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_SizingStretchProp
				| ImGuiTableFlags_PadOuterX;

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 0));
			ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(4, 2));

			if (ImGui::BeginTable("Table: File Data", 2, windowFlags))
			{
				windowFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody
					| ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_SizingStretchProp;

				ImGui::Indent();

				ImGui::TableSetupColumn("Text", 0, 0.4f);
				ImGui::TableSetupColumn("Input", 0, 0.6f);

				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
				ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 0));
				ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(4, 0));

				ImGui::TableNextColumn();
				ImGui::PushID(0);
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Data 01");
				ImGui::TableNextColumn();
				ImGui::InputText("", &str[0], str.length());
				ImGui::PopID();

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::PushID(1);
				ImGui::AlignTextToFramePadding();
				ImGui::Text("Data 02");
				ImGui::TableNextColumn();
				ImGui::InputText("", &str2[0], str2.length());
				ImGui::PopID();

				ImGui::PopStyleVar();
				ImGui::PopStyleVar();
				ImGui::PopStyleVar();
				ImGui::Unindent();
				ImGui::EndTable();
			}
			ImGui::PopStyleVar();
			ImGui::PopStyleVar();
		}
	}
}
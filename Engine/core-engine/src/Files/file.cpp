/*!***************************************************************************************
\file			file.cpp
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			10/11/2022

\brief
	Contains function definitions for files

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/
#include "pch.h"

#include "Files/file.h"
#include "Windows/windows-utils.h"
#include "SceneManager/scene-manager.h"

// Bean: Temporary for executing file
//#include <shellapi.h>

namespace Copium
{
	File::File() : std::filesystem::path()
	{

		modified = true;
	}

	File::File(const std::filesystem::path& pathRef) : std::filesystem::path(pathRef)
	{
		modified = true;
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
		_stat(string().c_str(), &statsBuffer);
		if (lastModifiedTime != statsBuffer.st_mtime)
		{
			modified = true;
			lastModifiedTime = statsBuffer.st_mtime;
		}
	}

	void File::access_file()
	{
		if (fileType.fileType == SCENE)
		{
			if (Copium::SceneManager::Instance()->get_current_scene() != nullptr)
			{
				std::cout << "change scene\n";
				Copium::SceneManager::Instance()->change_scene(string().c_str());
			}
			else
			{
				if (Copium::SceneManager::Instance()->load_scene(string().c_str()))
					std::cout << "loading success\n";
				else
					std::cout << "loading fail\n";
			}
		}
		else
		{
			PRINT("Opening file: " << filename().string() << "...");
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

			str = "File type: " + fileType.stringType;
			ImGui::Text(str.c_str());

			ImGui::EndTable();
		}
	}
}
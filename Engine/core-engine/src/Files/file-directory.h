/*!***************************************************************************************
\file			file-directory.h
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			10/11/2022

\brief
	Contains function declarations for file-directory.cpp

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/

#ifndef FILE_DIRECTORY_H
#define FILE_DIRECTORY_H

#include "Files/file.h"

namespace Copium
{
	class Directory final : public std::filesystem::directory_entry
	{
	public:
		// Bean: When user clicks on the file to view
		void inspector_view();

		const unsigned int& get_id() const { return instanceID; }
		void set_id(unsigned int const& _id) { instanceID = _id; }

		const int& get_file_count() const { return fileCount; }
		void set_file_count(int const& _count) { fileCount = _count; }

		const std::string& get_name() const { return name; }
		void set_name(std::string const& _name) { name = _name; }

		std::vector<File>& get_files() { return files; }
		void add_files(File& _file) { files.push_back(_file); }

		std::vector<Directory*>& get_child_directory() { return folders; }
		void add_child_directory(Directory* _directory) { folders.push_back(_directory); }

		Directory* get_parent_directory() { return parentFolder; }
		void set_parent_directory(Directory* _directory) { parentFolder = _directory; }

	private:
		unsigned int instanceID = 0; // The id to reference for the asset
		int fileCount = 0; // The number of files and folders
		std::string name = ""; // The name of the asset
		std::vector<File> files; // Files within the directory
		std::vector<Directory*> folders; // Folders within the directory
		Directory* parentFolder = nullptr; // A reference to the parent directory
	};
}

#endif // !FILE_DIRECTORY_H


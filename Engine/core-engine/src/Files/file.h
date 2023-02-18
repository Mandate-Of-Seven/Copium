/*!***************************************************************************************
\file			file.h
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			10/11/2022

\brief
	Contains function declarations for file.cpp

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/
#ifndef FILE_H
#define FILE_H

#include <filesystem>
#include "CopiumCore/uuid.h"

namespace Copium
{
	enum class FILE_TYPE
	{
		FOLDER, // For empty file type
		ASSET,
		AUDIO,
		CONFIG,
		PREFAB,
		FONT,
		META,
		SCENE,
		SCRIPT,
		SHADER,
		SPRITE,
		TEXT,

		NUM_TYPES
	};

	struct FileType
	{
		const char* stringType;
		FILE_TYPE fileType{};
	};

	class File
	{
	public:
		/*******************************************************************************
		/*!
		\brief
			Constructor of a file to use std::filesystem constructor
		*/
		/*******************************************************************************/
		File();

		/*******************************************************************************
		/*!
		\brief
			Conversion constructor of a file
		\param pathRef
			std::filesystem::path to convert into a file
		*/
		/*******************************************************************************/
		File(const std::filesystem::path& pathRef);

		/*******************************************************************************
		/*!
		\brief
			Copy assignment operator of a file
		\param rhs
			Other file to copy from
		\return

		*/
		/*******************************************************************************/
		File& operator=(const File& rhs) { modified = true; filePath = rhs.filePath; return *this; }

		/*******************************************************************************
		/*!
		\brief
			Uses winAPI to track for modification timings and set modified back to
			false if it were true
		\return
			If file was modified, return true, else return false
		*/
		/*******************************************************************************/
		bool is_modified();

		/*******************************************************************************
		/*!
		\brief
			Queries the system whether the modification timings changed
		*/
		/*******************************************************************************/
		void update_modification_timing();

		/*******************************************************************************
		/*!
		\brief
			Access the file when the user double clicks on the file
		*/
		/*******************************************************************************/
		void access_file();

		/*******************************************************************************
		/*!
		\brief
			Shows the file information when the user selects the file in the content 
			browser
		*/
		/*******************************************************************************/
		void inspector_view();

		const unsigned int& get_id() const { return instanceID; }
		void set_id(unsigned int const& _id) { instanceID = _id; }

		const std::string& get_name() const { return name; }
		void set_name(std::string const& _name) { name = _name; }

		const FileType& get_file_type() const { return fileType; }
		void set_file_type(FileType const& _fileType) { fileType = _fileType; }
		std::filesystem::path filePath;

	private:
		unsigned int instanceID = 0; // The id to reference for the asset
		std::string name = ""; // Name of the asset

		bool modified = false;
		time_t lastModifiedTime = 0;

		FileType fileType; // The type of file

		UUID uuid;
	};
}

#endif // !FILE_H
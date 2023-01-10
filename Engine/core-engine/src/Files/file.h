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

namespace Copium
{
	enum FileType
	{
		FOLDER, // For empty file type
		AUDIO,
		CONFIG,
		FONT,
		SCENE,
		SCRIPT,
		SHADER,
		SPRITE,
		TEXT,

		NUM_TYPES
	};

	static std::unordered_map<std::string, FileType> fileTypes
	{
		{"", FileType::FOLDER},
		{".wav", FileType::AUDIO },
		{".theme", FileType::CONFIG},
		{".json", FileType::CONFIG},
		{".ttf", FileType::FONT},
		{".scene", FileType::SCENE}, // Bean: change to .scene in the future
		{".cs", FileType::SCRIPT},
		{".vert", FileType::SHADER}, // Bean: change to .shader in the future
		{".frag", FileType::SHADER},
		{".png", FileType::SPRITE},
		{".txt", FileType::TEXT}
	};

	class File final : public std::filesystem::path
	{
	public:
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

		File& operator=(const File& rhs);

		const unsigned int& get_id() const { return instanceID; }
		void set_id(unsigned int const& _id) { instanceID = _id; }

		const std::string& get_name() const { return name; }
		void set_name(std::string const& _name) { name = _name; }

		const FileType fileType; // The type of file

	private:
		unsigned int instanceID = 0; // The id to reference for the asset
		std::string name = ""; // Name of the asset

		bool modified = false;
		time_t lastModifiedTime = 0;
	};
}

#endif // !FILE_H
/*!***************************************************************************************
\file			textures.h
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			22/09/2022

\brief
	This file holds the declaration of 2D textures and its data which is then parse into 
	sprites or into the Graphics system.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef TEXTURES_H
#define TEXTURES_H

#include <glm/glm.hpp>
#include <string>

namespace Copium
{
	class Texture
	{
	public:
		// Constructors
		/***************************************************************************/
		/*!
		\brief
			Generates the texture based on the file path using stbi_load
		*/
		/***************************************************************************/
		Texture(const std::string& _filePath);

		/***************************************************************************/
		/*!
		\brief
			Delete texture object id
		*/
		/***************************************************************************/
		~Texture();

		unsigned int get_width() { return width; }
		unsigned int get_height() { return height; }
		unsigned int get_object_id() { return textureObjectID; }
		std::string& get_file_path() { return filePath; }

	protected:
		std::string filePath;
		unsigned int width = 0;
		unsigned int height = 0;
		unsigned int textureObjectID = 0; // To store the texture

		unsigned int internalFormat = 0;
		unsigned int dataFormat = 0;
	};
}

#endif // !TEXTURES_H

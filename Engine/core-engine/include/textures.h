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

namespace Copium::Graphics
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

		GLuint get_width() { return width; }
		GLuint get_height() { return height; }
		GLuint get_object_id() { return textureObjectID; }
		std::string& get_file_path() { return filePath; }

	private:
		std::string filePath;
		GLuint width = 0;
		GLuint height = 0;
		GLuint textureObjectID = 0; // To store the texture

		GLenum internalFormat = 0;
		GLenum dataFormat = 0;
	};
}

#endif // !TEXTURES_H

/*!***************************************************************************************
\file			textures.h
\project
\author			Sean Ngo

\par			Course: GAM250
\par			Section:
\date			22/09/2022

\brief
	This file holds the declaration of 2D textures and its data which is then parse into 
	sprites or into the Graphics system.

All content � 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef TEXTURES_H
#define TEXTURES_H

#include <glm/glm.hpp>
#include <string>

namespace Copium
{
#define PIXELRATIO 1000.f
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
		void exit();

		const unsigned int& get_id() const { return instanceID; }
		void set_id(unsigned int const& _id) { instanceID = _id; }

		const unsigned int& get_width() const { return width; }
		const unsigned int& get_height() const { return height; }
		float get_pixel_width() const { return width / PIXELRATIO; }
		float get_pixel_height() const { return height / PIXELRATIO; }
		const unsigned int& get_object_id() const { return textureObjectID; }
		const std::string& get_file_path() const { return filePath; }

	protected:
		unsigned int instanceID = 0; // To reference to the file that contains the texture
		std::string filePath;
		unsigned int width = 0;
		unsigned int height = 0;

		unsigned int textureObjectID = 0; // To store the texture

		unsigned int internalFormat = 0;
		unsigned int dataFormat = 0;
	};
}

#endif // !TEXTURES_H

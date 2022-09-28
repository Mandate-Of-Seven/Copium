/*!***************************************************************************************
\file			textures.cpp
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			22/09/2022

\brief
	This file holds the definition of 2D textures and its data which is then parse into
	sprites or into the Graphics system.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"
#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include <../stb/stb_image.h>

#include "Graphics/textures.h"

namespace Copium::Graphics
{
	Texture::Texture(const std::string& _filePath) : filePath(_filePath)
	{
		int iWidth, iHeight, channels;

		stbi_set_flip_vertically_on_load(true);
		stbi_uc * data = stbi_load(_filePath.c_str(), &iWidth, &iHeight, &channels, 0);

		// File cannot be loaded
		M_Assert(data != NULL, "File cannot be loaded! Check the file path...");
			
		width = (GLuint)iWidth;
		height = (GLuint)iHeight;

		// Assign format based on the number of channels
		if (channels == 4)
		{
			// Include alpha
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		// Check if format exists
		M_Assert(internalFormat & dataFormat, "Format of texture not supported!!");

		// Align pixels of each row in memory to "1" (byte-alignment)
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glCreateTextures(GL_TEXTURE_2D, 1, &textureObjectID);
		glTextureStorage2D(textureObjectID, 1, internalFormat, width, height);

		glTextureParameteri(textureObjectID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(textureObjectID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(textureObjectID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(textureObjectID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(textureObjectID, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	Texture::~Texture()
	{
		//glDeleteTextures(1, &textureObjectID);
	}
}
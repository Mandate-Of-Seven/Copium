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

#include <GL/glew.h>
#include <string>

namespace Copium::Graphics
{
	class Texture
	{
	public:
		Texture(GLuint _width, GLuint _height);
		Texture(const std::string& _filePath);
		~Texture();

		GLuint get_width() { return width; }
		GLuint get_height() { return height; }

	private:
		std::string filePath;
		GLuint width = 0;
		GLuint height = 0;

	};
}

#endif // !TEXTURES_H

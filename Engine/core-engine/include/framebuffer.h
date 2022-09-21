/*!***************************************************************************************
\file			framebuffer.h
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			16/09/2022

\brief
	This file holds the declaration of functions for framebuffer.cpp.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <GL/glew.h>

namespace Copium::Graphics
{
	// Forward declare
	class GraphicsSystem;

	class Framebuffer
	{
	public:
		void init();

		void bind();

		void unbind();

		void resize(GLuint _width, GLuint _height);

		void exit();

		GLuint get_color_attachment_id() const { return colorAttachment; }

	private:

		GLuint colorAttachment = 0;
		GLuint depthAttachment = 0;

		GraphicsSystem * graphics;
	};
}
#endif // !FRAMEBUFFER_H

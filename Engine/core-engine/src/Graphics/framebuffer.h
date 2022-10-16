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

#include <glm/glm.hpp>

namespace Copium::Graphics
{
	class Framebuffer
	{
	public:
		// Constructors

		/***************************************************************************/
		/*!
		\brief
			Initialises the framebuffer by creating a frame buffer and binding it to
			a texture
		*/
		/***************************************************************************/
		void init();

		/***************************************************************************/
		/*!
		\brief
			Deletes the framebuffer and its textures
		*/
		/***************************************************************************/
		void exit();
		
		/***************************************************************************/
		/*!
		\brief
			Binds the framebuffer
		*/
		/***************************************************************************/
		void bind();

		/***************************************************************************/
		/*!
		\brief
			Unbinds the framebuffer
		*/
		/***************************************************************************/
		void unbind();

		/***************************************************************************/
		/*!
		\brief
			Resizes the scene view
		*/
		/***************************************************************************/
		void resize();

		GLuint get_color_attachment_id() const { return colorAttachment; }

		GLuint& get_buffer_object_id() { return frameBufferObjectID; }

	private:
		GLuint frameBufferObjectID = 0;

		GLuint colorAttachment = 0;
		GLuint depthAttachment = 0;
	};
}
#endif // !FRAMEBUFFER_H

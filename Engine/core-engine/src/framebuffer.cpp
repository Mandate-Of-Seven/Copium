/*!***************************************************************************************
\file			framebuffer.cpp
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			16/09/2022

\brief
	This file holds the definition of functions for the graphics framebuffer.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"
#include "framebuffer.h"

namespace Copium::Graphics
{
	void Framebuffer::init()
	{
		if (renderer.get_vertex_array_id())
			exit();

		// Temporary get the viewport dimensions
		/*GLint viewportProperties[4]{ 0 };
		glGetIntegerv(GL_VIEWPORT, viewportProperties);*/

		glCreateFramebuffers(1, &renderer.get_vertex_array_id());
		glBindFramebuffer(GL_FRAMEBUFFER, renderer.get_vertex_array_id());

		// Creating the color attachment
		glCreateTextures(GL_TEXTURE_2D, 1, &colorAttachment);
		glBindTexture(GL_TEXTURE_2D, colorAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, sceneWidth,
			sceneHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Attaching color attachment onto framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorAttachment, 0);

		// Creating the depth and stencil attachment
		glCreateTextures(GL_TEXTURE_2D, 1, &depthAttachment);
		glBindTexture(GL_TEXTURE_2D, depthAttachment);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, sceneWidth,
			sceneHeight, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);

		// Attaching depth and stencil attachment onto framebuffer
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthAttachment, 0);

		// Checking Completeness
		int errorId = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if (errorId == GL_FRAMEBUFFER_UNDEFINED)
			PRINT("Specified framebuffer is the default read or draw framebuffer, but the default framebuffer does not exist.");
		else if (errorId == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
			PRINT("Any of the framebuffer attachment points are framebuffer incomplete");
		else if (errorId == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT)
			PRINT("Framebuffer does not have at least one image attached to it");
		else if (errorId == GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER)
			PRINT("Value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for any color attachment point(s) named by GL_DRAW_BUFFERi");
		else if (errorId == GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER)
			PRINT("GL_READ_BUFFER is not GL_NONE and the value of GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE is GL_NONE for the color attachment point named by GL_READ_BUFFER");
		else if (errorId == GL_FRAMEBUFFER_UNSUPPORTED)
			PRINT("Combination of internal formats of the attached images violates an implementation-dependent set of restrictions");
		else if (errorId == GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE)
			PRINT("Value of GL_RENDERBUFFER_SAMPLES is not the same for all attached renderbuffers; if the value of GL_TEXTURE_SAMPLES is the not same for all attached textures; or, if the attached images are a mix of renderbuffers and textures, the value of GL_RENDERBUFFER_SAMPLES does not match the value of GL_TEXTURE_SAMPLES");
		else if (errorId == GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS)
			PRINT("Any framebuffer attachment is layered, and any populated attachment is not layered, or if all populated color attachments are not from textures of the same target");

		M_Assert(errorId == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::bind()
	{
		glViewport(0, 0, sceneWidth, sceneHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, renderer.get_vertex_array_id());
	}

	void Framebuffer::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void Framebuffer::resize(GLuint _width, GLuint _height)
	{
		sceneWidth = _width;
		sceneHeight = _height;

		init();
	}

	void Framebuffer::exit()
	{
		glDeleteFramebuffers(1, &renderer.get_vertex_array_id());
		glDeleteTextures(1, &colorAttachment);
		glDeleteTextures(1, &depthAttachment);
	}
}
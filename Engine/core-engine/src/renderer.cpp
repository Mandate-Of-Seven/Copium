/*!***************************************************************************************
\file			renderer.cpp
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			21/09/2022

\brief
	This file holds the definition of the Renderer class. The Renderer renders all
	objects in the game/scene. Use this class to access the renderer of any object,
	sprite or particle system (if any). Renderers can be disabled to make objects
	invisble (see enabled).

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"
#include "graphics-system.h"
#include <renderer.h>

namespace Copium::Graphics
{
	void Renderer::init()
	{
		graphics = GraphicsSystem::Instance();

		// Setup Line Vertex Array Object
		setup_line_vao();

		// Setup Quad Vertex Array Object
		setup_quad_vao();

		glCreateTextures(GL_TEXTURE_2D, 1, &graphics->whiteTexture);
		glBindTexture(GL_TEXTURE_2D, graphics->whiteTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		GLuint color = 0xffffffff;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);

		graphics->textureSlots.resize(maxTextures);
		graphics->textureSlots[0] = graphics->whiteTexture;

		for (GLuint i = 1; i < maxTextures; i++)
		{
			graphics->textureSlots[i] = 0;
		}
	}

	void Renderer::setup_quad_vao() 
	{	
		quadBuffer = new QuadVertex[maxVertexCount];

		// Vertex Array Object
		glCreateVertexArrays(1, &quadVertexArrayID);
		glBindVertexArray(quadVertexArrayID);

		// Quad Buffer Object
		glCreateBuffers(1, &quadVertexBufferID);
		glNamedBufferData(quadVertexBufferID, maxVertexCount * sizeof(QuadVertex), nullptr, GL_DYNAMIC_DRAW);
		glVertexArrayVertexBuffer(quadVertexArrayID, 0, quadVertexBufferID, 0, sizeof(QuadVertex));

		glEnableVertexArrayAttrib(quadVertexBufferID, 0);
		glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(QuadVertex, pos));
		glVertexAttribBinding(0, 0);

		glEnableVertexArrayAttrib(quadVertexBufferID, 1);
		glVertexAttribFormat(1, 4, GL_FLOAT, GL_FALSE, offsetof(QuadVertex, color));
		glVertexAttribBinding(1, 0);

		glEnableVertexArrayAttrib(quadVertexBufferID, 2);
		glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, offsetof(QuadVertex, textCoord));
		glVertexAttribBinding(2, 0);

		glEnableVertexArrayAttrib(quadVertexBufferID, 3);
		glVertexAttribFormat(3, 1, GL_FLOAT, GL_FALSE, offsetof(QuadVertex, texID));
		glVertexAttribBinding(3, 0);

		// Element Buffer Object
		GLushort indices[maxIndexCount];
		GLuint offset = 0;
		for (GLuint i = 0; i < maxIndexCount; i += 6)
		{
			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;
			indices[i + 3] = 2 + offset;
			indices[i + 4] = 3 + offset;
			indices[i + 5] = 0 + offset;

			offset += 4;
		}

		glCreateBuffers(1, &quadIndexBufferID);
		glNamedBufferStorage(quadIndexBufferID, sizeof(indices), indices, GL_DYNAMIC_STORAGE_BIT);

		glVertexArrayElementBuffer(quadVertexArrayID, quadIndexBufferID);
		glBindVertexArray(0);
	}

	void Renderer::setup_line_vao()
	{
		lineBuffer = new LineVertex[maxVertexCount];

		// Vertex Array Object
		glCreateVertexArrays(1, &lineVertexArrayID);
		glBindVertexArray(lineVertexArrayID);

		// Line Buffer Object
		glCreateBuffers(1, &lineVertexBufferID);
		glNamedBufferData(lineVertexBufferID, maxVertexCount * sizeof(LineVertex), nullptr, GL_DYNAMIC_DRAW);
		glVertexArrayVertexBuffer(lineVertexArrayID, 1, lineVertexBufferID, 0, sizeof(LineVertex));

		glEnableVertexArrayAttrib(lineVertexBufferID, 0);
		glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(LineVertex, pos));
		glVertexAttribBinding(0, 1);

		glEnableVertexArrayAttrib(lineVertexBufferID, 1);
		glVertexAttribFormat(1, 4, GL_FLOAT, GL_FALSE, offsetof(LineVertex, color));
		glVertexAttribBinding(1, 1);

		glBindVertexArray(0);

		glLineWidth(1.f);
	}

	void Renderer::shutdown()
	{
		glDeleteVertexArrays(1, &quadVertexArrayID);
		glDeleteVertexArrays(1, &lineVertexArrayID);
		glDeleteBuffers(1, &quadVertexBufferID);
		glDeleteBuffers(1, &lineVertexBufferID);
		glDeleteBuffers(1, &quadIndexBufferID);
		glDeleteTextures(1, &graphics->whiteTexture);

		delete[] quadBuffer;
		delete[] lineBuffer;
	}

	void Renderer::begin_batch()
	{
		quadIndexCount = 0;
		quadBufferPtr = quadBuffer;

		lineVertexCount = 0;
		lineBufferPtr = lineBuffer;
	}

	void Renderer::flush()
	{
		if (quadIndexCount)
		{
			graphics->shaderProgram[0].Use();
			for (GLuint i = 0; i < graphics->textureSlotIndex; i++)
				glBindTextureUnit(i, graphics->textureSlots[i]);

			glBindVertexArray(quadVertexArrayID);
			glDrawElements(GL_TRIANGLES, quadIndexCount, GL_UNSIGNED_SHORT, NULL);
			drawCount++;

			
			graphics->textureSlotIndex = 1;
			glBindVertexArray(0);
			graphics->shaderProgram[0].UnUse();
		}

		if (lineVertexCount)
		{
			graphics->shaderProgram[1].Use();
			glBindVertexArray(lineVertexArrayID);
			glDrawArrays(GL_LINES, 0, lineVertexCount);

			drawCount++;

			graphics->textureSlotIndex = 1;
			glBindVertexArray(0);
			graphics->shaderProgram[1].UnUse();
		}
		
	}

	void Renderer::end_batch()
	{
		if (quadIndexCount)
		{
			glBindVertexArray(quadVertexArrayID);
			GLsizeiptr size = (GLuint*)quadBufferPtr - (GLuint*)quadBuffer;
			//glBindBuffer(GL_ARRAY_BUFFER, quadVertexBufferID);
			glNamedBufferSubData(quadVertexBufferID, 0, size * 4, quadBuffer);
			glVertexArrayVertexBuffer(quadVertexArrayID, 0, quadVertexBufferID, 0, sizeof(QuadVertex));
			glBindVertexArray(0);
		}

		if (lineVertexCount)
		{
			glBindVertexArray(lineVertexArrayID);
			GLsizeiptr size = (GLuint*)lineBufferPtr - (GLuint*)lineBuffer;
			//glBindBuffer(GL_ARRAY_BUFFER, lineVertexBufferID);
			glNamedBufferSubData(lineVertexBufferID, 0, size * 4, lineBuffer);
			glVertexArrayVertexBuffer(lineVertexArrayID, 1, lineVertexBufferID, 0, sizeof(LineVertex));
			glBindVertexArray(0);
		}
		
	}

	void Renderer::draw_quad(const glm::vec2& _position, const glm::vec2& _size, const glm::vec4& _color)
	{
		if (quadIndexCount >= maxIndexCount)
		{
			end_batch();
			flush();
			begin_batch();
		}

		GLfloat textureIndex = 0.f;

		glm::vec2 halfsize = { _size.x / 2, _size.y / 2 };

		quadBufferPtr->pos = { _position.x - halfsize.x, _position.y - halfsize.y, 0.0f };
		//quadBufferPtr->pos = { _position.x, _position.y, 0.0f };
		quadBufferPtr->color = _color;
		quadBufferPtr->textCoord = { 0.f, 0.f };
		quadBufferPtr->texID = textureIndex;
		quadBufferPtr++;

		quadBufferPtr->pos = { _position.x + halfsize.x, _position.y - halfsize.y, 0.0f };
		//quadBufferPtr->pos = { _position.x + _size.x, _position.y, 0.0f };
		quadBufferPtr->color = _color;
		quadBufferPtr->textCoord = { 1.f, 0.f };
		quadBufferPtr->texID = textureIndex;
		quadBufferPtr++;

		quadBufferPtr->pos = { _position.x + halfsize.x, _position.y + halfsize.y, 0.0f };
		//quadBufferPtr->pos = { _position.x + _size.x, _position.y + _size.y, 0.0f };
		quadBufferPtr->color = _color;
		quadBufferPtr->textCoord = { 1.f, 1.f };
		quadBufferPtr->texID = textureIndex;
		quadBufferPtr++;

		quadBufferPtr->pos = { _position.x - halfsize.x, _position.y + halfsize.y, 0.0f };
		//quadBufferPtr->pos = { _position.x, _position.y + _size.y, 0.0f };
		quadBufferPtr->color = _color;
		quadBufferPtr->textCoord = { 0.f, 1.f };
		quadBufferPtr->texID = textureIndex;
		quadBufferPtr++;

		quadIndexCount += 6;
		quadCount++;
	}

	void Renderer::draw_quad(const glm::vec2& _position, const glm::vec2& _size, int _textureID)
	{
		if (quadIndexCount >= maxIndexCount || graphics->textureSlotIndex > maxTextures - 1)
		{
			end_batch();
			flush();
			begin_batch();
		}

		glm::vec4 color = { 1.f, 1.f, 1.f, 1.f };

		GLfloat textureIndex = 0.f;

		for (GLuint i = 1; i < graphics->textureSlotIndex; i++)
		{
			if (graphics->textureSlots[i] == _textureID)
			{
				textureIndex = (GLfloat)i;
				break;
			}
		}

		if (textureIndex == 0.f)
		{
			textureIndex = (GLfloat) graphics->textureSlotIndex;
			graphics->textureSlots[graphics->textureSlotIndex] = _textureID;
			graphics->textureSlotIndex++;
		}

		glm::vec2 halfsize = { _size.x / 2, _size.y / 2 };

		quadBufferPtr->pos = { _position.x - halfsize.x, _position.y - halfsize.y, 0.0f };
		quadBufferPtr->color = color;
		quadBufferPtr->textCoord = { 0.f, 0.f };
		quadBufferPtr->texID = textureIndex;
		quadBufferPtr++;

		quadBufferPtr->pos = { _position.x + halfsize.x, _position.y - halfsize.y, 0.0f };
		quadBufferPtr->color = color;
		quadBufferPtr->textCoord = { 1.f, 0.f };
		quadBufferPtr->texID = textureIndex;
		quadBufferPtr++;

		quadBufferPtr->pos = { _position.x + halfsize.x, _position.y + halfsize.y, 0.0f };
		quadBufferPtr->color = color;
		quadBufferPtr->textCoord = { 1.f, 1.f };
		quadBufferPtr->texID = textureIndex;
		quadBufferPtr++;

		quadBufferPtr->pos = { _position.x - halfsize.x, _position.y + halfsize.y, 0.0f };
		quadBufferPtr->color = color;
		quadBufferPtr->textCoord = { 0.f, 1.f };
		quadBufferPtr->texID = textureIndex;
		quadBufferPtr++;

		quadIndexCount += 6;
		quadCount++;
	}

	void Renderer::draw_quad(const glm::mat4& _transform, const glm::vec2& _position, const glm::vec2& _size, const glm::vec4& _color)
	{

		if (quadIndexCount >= maxIndexCount)
		{
			end_batch();
			flush();
			begin_batch();
		}

		GLfloat textureIndex = 0.f;

		glm::vec2 halfsize = { _size.x / 2, _size.y / 2 };

		quadBufferPtr->pos = _transform * glm::vec4(-halfsize.x, -halfsize.y, 1.f, 1.f);
		//quadBufferPtr->pos = glm::vec4(_position.x - halfsize.x, _position.y - halfsize.y, 1.f, 1.f);
		//quadBufferPtr->pos = { _position.x, _position.y, 0.0f };
		quadBufferPtr->color = _color;
		quadBufferPtr->textCoord = { 0.f, 0.f };
		quadBufferPtr->texID = textureIndex;
		quadBufferPtr++;

		quadBufferPtr->pos = _transform * glm::vec4(halfsize.x, -halfsize.y, 1.f, 1.f);
		//quadBufferPtr->pos = glm::vec4(_position.x + halfsize.x, _position.y - halfsize.y, 1.f, 1.f);
		//quadBufferPtr->pos = { _position.x + _size.x, _position.y, 0.0f };
		quadBufferPtr->color = _color;
		quadBufferPtr->textCoord = { 1.f, 0.f };
		quadBufferPtr->texID = textureIndex;
		quadBufferPtr++;

		quadBufferPtr->pos = _transform * glm::vec4(halfsize.x, halfsize.y, 1.f, 1.f);
		//quadBufferPtr->pos = glm::vec4(_position.x + halfsize.x, _position.y + halfsize.y, 1.f, 1.f);
		//quadBufferPtr->pos = { _position.x + _size.x, _position.y + _size.y, 0.0f };
		quadBufferPtr->color = _color;
		quadBufferPtr->textCoord = { 1.f, 1.f };
		quadBufferPtr->texID = textureIndex;
		quadBufferPtr++;

		quadBufferPtr->pos = _transform * glm::vec4(-halfsize.x, halfsize.y, 1.f, 1.f);
		//quadBufferPtr->pos = glm::vec4(_position.x - halfsize.x, _position.y + halfsize.y, 1.f, 1.f);
		//quadBufferPtr->pos = { _position.x, _position.y + _size.y, 0.0f };
		quadBufferPtr->color = _color;
		quadBufferPtr->textCoord = { 0.f, 1.f };
		quadBufferPtr->texID = textureIndex;
		quadBufferPtr++;

		quadIndexCount += 6;
		quadCount++;
	}

	void Renderer::draw_line(const glm::vec2& _position0, const glm::vec2& _position1, const glm::vec4& _color)
	{
		if (lineVertexCount >= maxLineCount)
		{
			end_batch();
			flush();
			begin_batch();
		}

		lineBufferPtr->pos = glm::vec3(_position0, 0);
		lineBufferPtr->color = _color;
		lineBufferPtr++;

		lineBufferPtr->pos = glm::vec3(_position1, 0);
		lineBufferPtr->color = _color;
		lineBufferPtr++;

		lineVertexCount += 2;
	}
}

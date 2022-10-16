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
#include "Graphics/graphics-system.h"
#include "Graphics/renderer.h"

#include "Editor/editor-system.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Graphics/camera.h"

namespace Copium::Graphics
{
	void Renderer::init()
	{
		graphics = GraphicsSystem::Instance();

		// Setup Line Vertex Array Object
		setup_line_vao();

		// Setup Quad Vertex Array Object
		setup_quad_vao();

		GLuint texture = graphics->get_white_texture();
		glCreateTextures(GL_TEXTURE_2D, 1, &texture);
		glTextureStorage2D(texture, 1, GL_RGBA8, 1, 1);

		glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		GLuint color = 0xffffffff;
		glTextureSubImage2D(texture, 0, 0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &color);

		graphics->get_texture_slots().resize(maxTextures);
		graphics->get_texture_slots()[0] = texture;

		for (GLuint i = 1; i < maxTextures; i++)
		{
			graphics->get_texture_slots()[i] = 0;
		}
	}

	// Setup the quads vertex array object
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
		GLushort offset = 0;
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

		// Setup default quad vertex positions
		quadVertexPosition[0] = { -0.5f, -0.5f, 0.f, 1.f };
		quadVertexPosition[1] = { 0.5f, -0.5f, 0.f, 1.f };
		quadVertexPosition[2] = { 0.5f, 0.5f, 0.f, 1.f };
		quadVertexPosition[3] = { -0.5f, 0.5f, 0.f, 1.f };

		// Setup default textutre coordinates
		quadTextCoord[0] = { 0.f, 0.f };
		quadTextCoord[1] = { 1.f, 0.f };
		quadTextCoord[2] = { 1.f, 1.f };
		quadTextCoord[3] = { 0.f, 1.f };
	}

	// Setup the line vertex array object
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
		glDeleteTextures(1, &graphics->get_white_texture());

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
			// Alpha blending for transparent objects
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			graphics->get_shader_program()[0].Use();
			glBindVertexArray(quadVertexArrayID);

			// Bean: Matrix assignment to be placed somewhere else
			GLuint uProjection = glGetUniformLocation(
				graphics->get_shader_program()[0].GetHandle(), "uViewProjection");

			glm::mat4 projection = Copium::Editor::EditorSystem::Instance()->get_camera().get_view_projection();
			glUniformMatrix4fv(uProjection, 1, GL_FALSE, glm::value_ptr(projection));
			/*PRINT("View Projection Matrix:");
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					std::cout << projection[j][i] << " ";
				}
				std::cout << "\n";
			}*/
			// End of matrix assignment
			
			for (GLuint i = 0; i < graphics->get_texture_slot_index(); i++)
				glBindTextureUnit(i, graphics->get_texture_slots()[i]);

			glDrawElements(GL_TRIANGLES, quadIndexCount, GL_UNSIGNED_SHORT, NULL);
			drawCount++;

			graphics->set_texture_slot_index(1);
			glBindVertexArray(0);
			graphics->get_shader_program()[0].UnUse();
			glDisable(GL_BLEND);
		}

		if (lineVertexCount)
		{
			graphics->get_shader_program()[1].Use();
			glBindVertexArray(lineVertexArrayID);

			// Bean: Matrix assignment to be placed somewhere else
			GLuint uProjection = glGetUniformLocation(
				graphics->get_shader_program()[1].GetHandle(), "uViewProjection");

			glm::mat4 projection = Copium::Editor::EditorSystem::Instance()->get_camera().get_view_projection();
			glUniformMatrix4fv(uProjection, 1, GL_FALSE, glm::value_ptr(projection));

			// End of matrix assignment

			glDrawArrays(GL_LINES, 0, lineVertexCount);
			drawCount++;

			glBindVertexArray(0);
			graphics->get_shader_program()[1].UnUse();
		}
		
	}

	void Renderer::end_batch()
	{
		if (quadIndexCount)
		{
			glBindVertexArray(quadVertexArrayID);
			GLsizeiptr size = (GLuint*)quadBufferPtr - (GLuint*)quadBuffer;
			glNamedBufferSubData(quadVertexBufferID, 0, size * 4, quadBuffer);
			glVertexArrayVertexBuffer(quadVertexArrayID, 0, quadVertexBufferID, 0, sizeof(QuadVertex));
			glBindVertexArray(0);
		}

		if (lineVertexCount)
		{
			glBindVertexArray(lineVertexArrayID);
			GLsizeiptr size = (GLuint*)lineBufferPtr - (GLuint*)lineBuffer;
			glNamedBufferSubData(lineVertexBufferID, 0, size * 4, lineBuffer);
			glVertexArrayVertexBuffer(lineVertexArrayID, 1, lineVertexBufferID, 0, sizeof(LineVertex));
			glBindVertexArray(0);
		}
		
	}

	void Renderer::draw_quad(const glm::vec3& _position, const glm::vec2& _scale, const float _rotation, const glm::vec4& _color)
	{
		glm::mat4 translate = glm::translate(glm::mat4(1.f), _position);
		
		float radians = glm::radians(_rotation);

		glm::mat4 rotation = {
			glm::vec4(cos(radians), sin(radians), 0.f, 0.f),
			glm::vec4(-sin(radians), cos(radians), 0.f, 0.f),
			glm::vec4(0.f, 0.f, 1.f, 0.f),
			glm::vec4(0.f, 0.f, 0.f, 1.f)
		};

		glm::mat4 scale = {
			glm::vec4(_scale.x, 0.f, 0.f, 0.f),
			glm::vec4(0.f, _scale.y, 0.f, 0.f),
			glm::vec4(0.f, 0.f, 1.f, 0.f),
			glm::vec4(0.f, 0.f, 0.f, 1.f)
		};

		glm::mat4 transform = translate * rotation * scale;
		draw_quad(transform, _color);
	}

	void Renderer::draw_quad(const glm::vec3& _position, const glm::vec2& _scale, const float _rotation, GLuint _textureID)
	{
		glm::mat4 translate = glm::translate(glm::mat4(1.f), _position);

		float radians = glm::radians(_rotation);

		glm::mat4 rotation = {
			glm::vec4(cos(radians), sin(radians), 0.f, 0.f),
			glm::vec4(-sin(radians), cos(radians), 0.f, 0.f),
			glm::vec4(0.f, 0.f, 1.f, 0.f),
			glm::vec4(0.f, 0.f, 0.f, 1.f)
		};

		glm::mat4 scale = {
			glm::vec4(_scale.x, 0.f, 0.f, 0.f),
			glm::vec4(0.f, _scale.y, 0.f, 0.f),
			glm::vec4(0.f, 0.f, 1.f, 0.f),
			glm::vec4(0.f, 0.f, 0.f, 1.f)
		};

		glm::mat4 transform = translate * rotation * scale;
		draw_quad(transform, _textureID);
	}

	void Renderer::draw_quad(const glm::mat4& _transform, const glm::vec4& _color)
	{
		if (quadIndexCount >= maxIndexCount)
		{
			end_batch();
			flush();
			begin_batch();
		}

		glm::mat4 projection = Copium::Editor::EditorSystem::Instance()->get_camera().get_view_projection();
		

		GLfloat textureIndex = 0.f;

		for (GLint i = 0; i < 4; i++)
		{
			quadBufferPtr->pos = _transform * quadVertexPosition[i];
			quadBufferPtr->textCoord = quadTextCoord[i];
			quadBufferPtr->color = _color;
			quadBufferPtr->texID = textureIndex;
			quadBufferPtr++;
		}

		glm::vec3 pos = (quadBufferPtr - 4)->pos;
		pos = projection * glm::vec4(pos, 1.f);
		//std::cout << "Position: " << pos.x << "," << pos.y << "," << pos.z << "\n";

		quadIndexCount += 6;
		quadCount++;
	}

	void Renderer::draw_quad(const glm::mat4& _transform, GLuint _textureID)
	{
		if (quadIndexCount >= maxIndexCount)
		{
			end_batch();
			flush();
			begin_batch();
		}

		glm::vec4 color = { 1.f, 1.f, 1.f, 1.f };

		GLfloat textureIndex = 0.f;

		for (GLuint i = 1; i < graphics->get_texture_slot_index(); i++)
		{
			if (graphics->get_texture_slots()[i] == _textureID)
			{
				textureIndex = (GLfloat) i;
				break;
			}
		}

		if (textureIndex == 0.f)
		{
			textureIndex = (GLfloat) graphics->get_texture_slot_index();
			graphics->get_texture_slots()[graphics->get_texture_slot_index()] = _textureID;
			graphics->set_texture_slot_index((GLuint)textureIndex + 1);
		}

		for (GLint i = 0; i < 4; i++)
		{
			quadBufferPtr->pos = _transform * quadVertexPosition[i];
			quadBufferPtr->textCoord = quadTextCoord[i];
			quadBufferPtr->color = color;
			quadBufferPtr->texID = textureIndex;
			quadBufferPtr++;
		}

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

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
#include <GL/glew.h> // for access to OpenGL API declarations 

#include "Graphics/graphics-system.h"
#include "Graphics/renderer.h"
#include "Graphics/fonts.h"

#include <glm/gtc/type_ptr.hpp>

namespace Copium
{
	void Renderer::init(BaseCamera* _camera)
	{
		graphics = &MyGraphicsSystem;
		camera = _camera;

		// Setup Quad Vertex Array Object
		setup_quad_vao();

		// Setup Line Vertex Array Object
		setup_line_vao();

		// Setup Circle Vertex Array Object
		setup_circle_vao();

		// Setup Text Vertex Array Object
		setup_text_vao();

		GLuint texture = graphics->get_white_texture();
		glCreateTextures(GL_TEXTURE_2D, 1, &texture);
		glTextureStorage2D(texture, 1, GL_RGBA8, 1, 1);

		glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		GLuint color = 0xFFFFFFFF;
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

		// Quad Buffer Object
		glCreateBuffers(1, &quadVertexBufferID);
		glNamedBufferStorage(quadVertexBufferID, maxVertexCount * sizeof(QuadVertex), nullptr, GL_DYNAMIC_STORAGE_BIT);

		glEnableVertexArrayAttrib(quadVertexArrayID, 0);
		glVertexArrayAttribFormat(quadVertexArrayID, 0, 3, GL_FLOAT, GL_FALSE, offsetof(QuadVertex, pos));
		glVertexArrayAttribBinding(quadVertexArrayID, 0, 0);

		glEnableVertexArrayAttrib(quadVertexArrayID, 1);
		glVertexArrayAttribFormat(quadVertexArrayID, 1, 4, GL_FLOAT, GL_FALSE, offsetof(QuadVertex, color));
		glVertexArrayAttribBinding(quadVertexArrayID, 1, 0);

		glEnableVertexArrayAttrib(quadVertexArrayID, 2);
		glVertexArrayAttribFormat(quadVertexArrayID, 2, 2, GL_FLOAT, GL_FALSE, offsetof(QuadVertex, textCoord));
		glVertexArrayAttribBinding(quadVertexArrayID, 2, 0);

		glEnableVertexArrayAttrib(quadVertexArrayID, 3);
		glVertexArrayAttribFormat(quadVertexArrayID, 3, 1, GL_FLOAT, GL_FALSE, offsetof(QuadVertex, texID));
		glVertexArrayAttribBinding(quadVertexArrayID, 3, 0);

		glEnableVertexArrayAttrib(quadVertexArrayID, 4);
		glVertexArrayAttribFormat(quadVertexArrayID, 4, 1, GL_INT, GL_FALSE, offsetof(QuadVertex, entityID));
		glVertexArrayAttribBinding(quadVertexArrayID, 4, 0);

		// Element Buffer Object
		GLushort indices[maxIndexCount]{0};
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

		// Setup default quad vertex positions
		quadVertexPosition[0] = { -0.5f, -0.5f, 0.f, 1.f };
		quadVertexPosition[1] = { 0.5f, -0.5f, 0.f, 1.f };
		quadVertexPosition[2] = { 0.5f, 0.5f, 0.f, 1.f };
		quadVertexPosition[3] = { -0.5f, 0.5f, 0.f, 1.f };

		// Setup default texture coordinates
		quadTextCoord[0] = { 0.f, 0.f };
		quadTextCoord[1] = { 1.f, 0.f };
		quadTextCoord[2] = { 1.f, 1.f };
		quadTextCoord[3] = { 0.f, 1.f };
	}

	// Setup the line vertex array object
	void Renderer::setup_line_vao()
	{
		lineBuffer = new LineVertex[maxLineVertexCount];

		// Vertex Array Object
		glCreateVertexArrays(1, &lineVertexArrayID);

		// Line Buffer Object
		glCreateBuffers(1, &lineVertexBufferID);
		glNamedBufferStorage(lineVertexBufferID, maxLineVertexCount * sizeof(LineVertex), nullptr, GL_DYNAMIC_STORAGE_BIT);

		glEnableVertexArrayAttrib(lineVertexArrayID, 0);
		glVertexArrayAttribFormat(lineVertexArrayID, 0, 3, GL_FLOAT, GL_FALSE, offsetof(LineVertex, pos));
		glVertexArrayAttribBinding(lineVertexArrayID, 0, 1);

		glEnableVertexArrayAttrib(lineVertexArrayID, 1);
		glVertexArrayAttribFormat(lineVertexArrayID, 1, 4, GL_FLOAT, GL_FALSE, offsetof(LineVertex, color));
		glVertexArrayAttribBinding(lineVertexArrayID, 1, 1);

		glLineWidth(1.f);
	}

	// Setup the circle vertex array object
	void Renderer::setup_circle_vao()
	{
		circleBuffer = new CircleVertex[maxCircleVertexCount];

		// Vertex Array Object
		glCreateVertexArrays(1, &circleVertexArrayID);

		// Circle Buffer Object
		glCreateBuffers(1, &circleVertexBufferID);
		glNamedBufferStorage(circleVertexBufferID, maxCircleVertexCount * sizeof(CircleVertex), nullptr, GL_DYNAMIC_STORAGE_BIT);

		glEnableVertexArrayAttrib(circleVertexArrayID, 0);
		glVertexArrayAttribFormat(circleVertexArrayID, 0, 3, GL_FLOAT, GL_FALSE, offsetof(CircleVertex, pos));
		glVertexArrayAttribBinding(circleVertexArrayID, 0, 2);

		glEnableVertexArrayAttrib(circleVertexArrayID, 1);
		glVertexArrayAttribFormat(circleVertexArrayID, 1, 4, GL_FLOAT, GL_FALSE, offsetof(CircleVertex, color));
		glVertexArrayAttribBinding(circleVertexArrayID, 1, 2);

		// Element Buffer Object
		GLushort indices[maxCircleIndexCount]{ 0 };
		GLushort counter = 0;
		for (GLushort i = 0; i < maxCircleIndexCount; i += (circleVertices + 1))
		{
			for (GLushort j = 0; j < circleVertices; j++)
			{
				indices[i + j] = counter;
				counter++;
			}

			indices[i + circleVertices] = maxCircleIndexCount + 1;
		}

		glCreateBuffers(1, &circleIndexBufferID);
		glNamedBufferStorage(circleIndexBufferID, sizeof(indices), indices, GL_DYNAMIC_STORAGE_BIT);

		float degree = 360.f / (float)(circleVertices);
		float rads = glm::radians(degree);
		for (GLint i = 0; i < circleVertices; i++)
		{
			circleVertexPosition[i] = glm::vec4(cos(rads * i), sin(rads * i), 0.f, 1.f);
		}

		glLineWidth(1.f);
	}

	void Renderer::setup_text_vao()
	{
		textBuffer = new TextVertex[maxTextVertexCount];

		// Vertex Array Object
		glCreateVertexArrays(1, &textVertexArrayID);

		// Text Buffer Object
		glCreateBuffers(1, &textVertexBufferID);
		glNamedBufferStorage(textVertexBufferID, maxTextVertexCount * sizeof(TextVertex), nullptr, GL_DYNAMIC_STORAGE_BIT);

		glEnableVertexArrayAttrib(textVertexArrayID, 0);
		glVertexArrayAttribFormat(textVertexArrayID, 0, 3, GL_FLOAT, GL_FALSE, offsetof(TextVertex, pos));
		glVertexArrayAttribBinding(textVertexArrayID, 0, 3);

		glEnableVertexArrayAttrib(textVertexArrayID, 1);
		glVertexArrayAttribFormat(textVertexArrayID, 1, 4, GL_FLOAT, GL_FALSE, offsetof(TextVertex, color));
		glVertexArrayAttribBinding(textVertexArrayID, 1, 3);

		glEnableVertexArrayAttrib(textVertexArrayID, 2);
		glVertexArrayAttribFormat(textVertexArrayID, 2, 2, GL_FLOAT, GL_FALSE, offsetof(TextVertex, textCoord));
		glVertexArrayAttribBinding(textVertexArrayID, 2, 3);

		/*glEnableVertexArrayAttrib(textVertexArrayID, 3);
		glVertexArrayAttribFormat(textVertexArrayID, 3, 1, GL_FLOAT, GL_FALSE, offsetof(TextVertex, fontID));
		glVertexArrayAttribBinding(textVertexArrayID, 3, 3);*/

		// Setup default text texture coordinates
		textTextCoord[0] = { 0.f, 0.f };
		textTextCoord[1] = { 0.f, 1.f };
		textTextCoord[2] = { 1.f, 1.f };
		textTextCoord[3] = { 0.f, 0.f };
		textTextCoord[4] = { 1.f, 1.f };
		textTextCoord[5] = { 1.f, 0.f };
	}

	void Renderer::shutdown()
	{
		glDeleteVertexArrays(1, &quadVertexArrayID);
		glDeleteVertexArrays(1, &lineVertexArrayID);
		glDeleteVertexArrays(1, &circleVertexArrayID);
		glDeleteVertexArrays(1, &textVertexArrayID);
		glDeleteBuffers(1, &quadVertexBufferID);
		glDeleteBuffers(1, &lineVertexBufferID);
		glDeleteBuffers(1, &circleVertexBufferID);
		glDeleteBuffers(1, &textVertexBufferID);
		glDeleteBuffers(1, &quadIndexBufferID);
		glDeleteBuffers(1, &circleIndexBufferID);
		glDeleteTextures(1, &graphics->get_white_texture());

		delete[] quadBuffer;
		delete[] lineBuffer;
		delete[] circleBuffer;
		delete[] textBuffer;
	}

	void Renderer::begin_batch()
	{
		lineWidth = 1.f;
		circleWidth = 1.f;

		quadIndexCount = 0;
		quadBufferPtr = quadBuffer;

		lineVertexCount = 0;
		lineBufferPtr = lineBuffer;

		circleIndexCount = 0;
		circleBufferPtr = circleBuffer;

		textVertexCount = 0;
		textBufferPtr = textBuffer;
	}

	void Renderer::flush()
	{
		if (quadIndexCount)
		{
			// Alpha blending for transparent objects
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			graphics->get_shader_program()[QUAD_SHADER].Use();
			glBindVertexArray(quadVertexArrayID);

			// Bean: Matrix assignment to be placed somewhere else
			GLuint uProjection = glGetUniformLocation(
				graphics->get_shader_program()[QUAD_SHADER].GetHandle(), "uViewProjection");
			/*GLuint uTransform = glGetUniformLocation(
				graphics->get_shader_program()[QUAD_SHADER].GetHandle(), "uTransform");*/
			glm::mat4 projection = camera->get_view_proj_matrix();
			glUniformMatrix4fv(uProjection, 1, GL_FALSE, glm::value_ptr(projection));
			//glm::vec3 pos = camera->get_eye();
			//glm::mat4 transform = glm::translate(glm::mat4(1.f), glm::vec3(0.f, 0.f, 0.f));
			//glUniformMatrix4fv(uTransform, 1, GL_FALSE, glm::value_ptr(transform));
			
			/*PRINT("Transform Matrix:");
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					std::cout << transform[j][i] << " ";
				}
				std::cout << "\n";
			}*/
			// End of matrix assignment
			
			if (graphics->get_texture_slot_index() >= 32)
			{
				graphics->set_texture_slot_index(1);
				end_batch();
				flush();
				begin_batch();
			}

			for (GLuint i = 1; i < 32; i++)
				glBindTextureUnit(i, graphics->get_texture_slots()[i]);

			glDrawElements(GL_TRIANGLES, quadIndexCount, GL_UNSIGNED_SHORT, NULL);
			drawCount++;

			
			glBindVertexArray(0);
			graphics->get_shader_program()[QUAD_SHADER].UnUse();
			glDisable(GL_BLEND);
		}

		if (lineVertexCount)
		{
			graphics->get_shader_program()[LINE_SHADER].Use();
			glBindVertexArray(lineVertexArrayID);

			// Bean: Matrix assignment to be placed somewhere else
			GLuint uProjection = glGetUniformLocation(
				graphics->get_shader_program()[LINE_SHADER].GetHandle(), "uViewProjection");

			glm::mat4 projection = camera->get_view_proj_matrix();
			glUniformMatrix4fv(uProjection, 1, GL_FALSE, glm::value_ptr(projection));

			// End of matrix assignment

			glLineWidth(lineWidth);
			glDrawArrays(GL_LINES, 0, lineVertexCount);
			drawCount++;

			glBindVertexArray(0);
			graphics->get_shader_program()[LINE_SHADER].UnUse();
		}

		if (circleIndexCount)
		{
			graphics->get_shader_program()[LINE_SHADER].Use();
			glBindVertexArray(circleVertexArrayID);

			// Bean: Matrix assignment to be placed somewhere else
			GLuint uProjection = glGetUniformLocation(
				graphics->get_shader_program()[LINE_SHADER].GetHandle(), "uViewProjection");

			glm::mat4 projection = camera->get_view_proj_matrix();
			glUniformMatrix4fv(uProjection, 1, GL_FALSE, glm::value_ptr(projection));

			// End of matrix assignment

			glEnable(GL_PRIMITIVE_RESTART);
			glPrimitiveRestartIndex(maxCircleIndexCount + 1);
			glLineWidth(circleWidth);
			glDrawElements(GL_LINE_LOOP, circleIndexCount, GL_UNSIGNED_SHORT, NULL);
			drawCount++;

			glBindVertexArray(0);
			graphics->get_shader_program()[LINE_SHADER].UnUse();
		}

		if (textVertexCount)
		{
			// Alpha blending for transparent objects
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			graphics->get_shader_program()[TEXT_SHADER].Use();
			glActiveTexture(GL_TEXTURE0);
			glBindVertexArray(textVertexArrayID);

			// Bind texture unit
			// 
			
			// Cannot just bind texture because there is multiple textures in this one draw
			// Requires an array of textures and reference from there
			//glBindTexture(GL_TEXTURE_2D, ch.textureID);

			// Bean: Matrix assignment to be placed somewhere else
			GLuint uProjection = glGetUniformLocation(
				graphics->get_shader_program()[TEXT_SHADER].GetHandle(), "uViewProjection");

			glm::mat4 projection = camera->get_view_proj_matrix();
			glUniformMatrix4fv(uProjection, 1, GL_FALSE, glm::value_ptr(projection));

			// End of matrix assignment

			// Draws the entire text out
			glDrawArrays(GL_TRIANGLES, 0, textVertexCount);
			drawCount++;

			glBindVertexArray(0);
			glBindTexture(GL_TEXTURE_2D, 0);
			graphics->get_shader_program()[TEXT_SHADER].UnUse();
			glDisable(GL_BLEND);
		}
	}

	void Renderer::end_batch()
	{
		if (quadIndexCount)
		{
			// Quad Buffer Object
			glBindVertexArray(quadVertexArrayID);
			GLsizeiptr size = (GLuint*)quadBufferPtr - (GLuint*)quadBuffer;
			glNamedBufferSubData(quadVertexBufferID, 0, sizeof(float) * size, quadBuffer);
			glVertexArrayVertexBuffer(quadVertexArrayID, 0, quadVertexBufferID, 0, sizeof(QuadVertex));
			glVertexArrayElementBuffer(quadVertexArrayID, quadIndexBufferID);
			glBindVertexArray(0);
		}

		if (lineVertexCount)
		{
			glBindVertexArray(lineVertexArrayID);
			GLsizeiptr size = (GLuint*)lineBufferPtr - (GLuint*)lineBuffer;
			glNamedBufferSubData(lineVertexBufferID, 0, sizeof(float) * size, lineBuffer);
			glVertexArrayVertexBuffer(lineVertexArrayID, 1, lineVertexBufferID, 0, sizeof(LineVertex));
			glBindVertexArray(0);
		}

		if (circleIndexCount)
		{
			glBindVertexArray(circleVertexArrayID);
			GLsizeiptr size = (GLuint*)circleBufferPtr - (GLuint*)circleBuffer;
			glNamedBufferSubData(circleVertexBufferID, 0, sizeof(float) * size, circleBuffer);
			glVertexArrayVertexBuffer(circleVertexArrayID, 2, circleVertexBufferID, 0, sizeof(CircleVertex));
			glVertexArrayElementBuffer(circleVertexArrayID, circleIndexBufferID);
			glBindVertexArray(0);
		}

		if (textVertexCount)
		{
			glBindVertexArray(textVertexArrayID);
			GLsizeiptr size = (GLuint*)textBufferPtr - (GLuint*)textBuffer;
			glNamedBufferSubData(textVertexBufferID, 0, sizeof(float) * size, textBuffer);
			glVertexArrayVertexBuffer(textVertexArrayID, 3, textVertexBufferID, 0, sizeof(TextVertex));
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

	void Renderer::draw_quad(const glm::vec3& _position, const glm::vec2& _scale, const float _rotation, const GLuint _textureID)
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

	void Renderer::draw_quad(const glm::vec3& _position, const glm::vec2& _scale, const float _rotation, const Sprite& _sprite, glm::fvec4* tintColor)
	{
		glm::mat4 translate = glm::translate(glm::mat4(1.f), _position);

		float radians = glm::radians(_rotation);

		glm::mat4 rotation = {
			glm::vec4(cos(radians), sin(radians), 0.f, 0.f),
			glm::vec4(-sin(radians), cos(radians), 0.f, 0.f),
			glm::vec4(0.f, 0.f, 1.f, 0.f),
			glm::vec4(0.f, 0.f, 0.f, 1.f)
		};

		float pixelWidth = 1.f, pixelHeight = 1.f;
		if (_sprite.get_texture() != nullptr)
		{
			pixelWidth = _sprite.get_texture()->get_pixel_width();
			pixelHeight = _sprite.get_texture()->get_pixel_height();
		}

		glm::mat4 scale = {
			glm::vec4(_scale.x * pixelWidth, 0.f, 0.f, 0.f),
			glm::vec4(0.f, _scale.y * pixelHeight, 0.f, 0.f),
			glm::vec4(0.f, 0.f, 1.f, 0.f),
			glm::vec4(0.f, 0.f, 0.f, 1.f)
		};

		glm::mat4 transform = translate * rotation * scale;
		draw_quad(transform, _sprite, tintColor);
	}

	void Renderer::draw_quad(const glm::vec3& _position, const glm::vec2& _scale, const float _rotation, const Spritesheet& _spritesheet, GLuint _offsetID, int _frames)
	{
		glm::mat4 translate = glm::translate(glm::mat4(1.f), _position);

		float radians = glm::radians(_rotation);

		glm::mat4 rotation = {
			glm::vec4(cos(radians), sin(radians), 0.f, 0.f),
			glm::vec4(-sin(radians), cos(radians), 0.f, 0.f),
			glm::vec4(0.f, 0.f, 1.f, 0.f),
			glm::vec4(0.f, 0.f, 0.f, 1.f)
		};

		if (!_spritesheet.texture)
			return;

		float pixelWidth = _spritesheet.texture->get_pixel_width() / (float)_spritesheet.columns;
		float pixelHeight = _spritesheet.texture->get_pixel_height() / (float)_spritesheet.rows;


		//PRINT("Pixel width:" << pixelWidth);


		glm::mat4 scale = {
			glm::vec4(_scale.x * pixelWidth, 0.f, 0.f, 0.f),
			glm::vec4(0.f, _scale.y * pixelHeight, 0.f, 0.f),
			glm::vec4(0.f, 0.f, 1.f, 0.f),
			glm::vec4(0.f, 0.f, 0.f, 1.f)
		};

		glm::mat4 transform = translate * rotation * scale;

		//PRINT("Drawing spritesheet");

		draw_quad(transform, _spritesheet, _offsetID, _frames);
	}

	void Renderer::draw_quad(const glm::mat4& _transform, const glm::vec4& _color)
	{
		if (quadIndexCount >= maxIndexCount)
		{
			end_batch();
			flush();
			begin_batch();
		}

		GLfloat textureIndex = 0.f;

		for (GLint i = 0; i < 4; i++)
		{
			quadBufferPtr->pos = _transform * quadVertexPosition[i];
			quadBufferPtr->textCoord = quadTextCoord[i];
			quadBufferPtr->color = _color;
			quadBufferPtr->texID = textureIndex;
			quadBuffer->entityID = quadCount;
			quadBufferPtr++;
		}

		quadIndexCount += 6;
		quadCount++;
	}

	void Renderer::draw_quad(const glm::mat4& _transform, const GLuint _textureID)
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

		// Change texture index only if ID retrieved is more than 0 (0 is white texture)
		if (textureIndex == 0.f && _textureID != 0)
		{
			// Add new texture into the texture slot
			textureIndex = (GLfloat) graphics->get_texture_slot_index();
			graphics->get_texture_slots()[graphics->get_texture_slot_index()] = _textureID;
			graphics->set_texture_slot_index((GLuint) textureIndex + 1);
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

	void Renderer::draw_quad(const glm::mat4& _transform, const Sprite& _sprite, glm::vec4* tint)
	{
		if (quadIndexCount >= maxIndexCount)
		{
			end_batch();
			flush();
			begin_batch();
		}

		GLfloat textureIndex = 0.f;

		for (GLuint i = 1; i < graphics->get_texture_slot_index(); i++)
		{
			if (!_sprite.get_texture())
				break;

			if (graphics->get_texture_slots()[i] == _sprite.get_texture()->get_object_id())
			{
				textureIndex = (GLfloat) i;
				break;
			}
		}

		// Change texture index only if ID retrieved is more than 0 (0 is white texture)
		if (textureIndex == 0.f && _sprite.get_sprite_id() != 0)
		{
			// Add new texture into the texture slot
			textureIndex = (GLfloat) graphics->get_texture_slot_index();
			graphics->get_texture_slots()[graphics->get_texture_slot_index()] = _sprite.get_texture()->get_object_id();
			graphics->set_texture_slot_index((GLuint)textureIndex + 1);
		}

		for (GLint i = 0; i < 4; i++)
		{
			quadBufferPtr->pos = _transform * quadVertexPosition[i];
			quadBufferPtr->textCoord = quadTextCoord[i];

			if (!tint)
				quadBufferPtr->color = _sprite.get_color();
			else
			{
				glm::fvec4 mixedColor{ 0 };
				glm::fvec4 color{ _sprite.get_color() };
				glm::fvec4 layeredColor{ *tint };
				mixedColor.a = 1 - (1 - layeredColor.a) * (1 - color.a); // 0.75
				if (mixedColor.a < 0.01f)
					return;
				mixedColor.r = layeredColor.r * layeredColor.a / mixedColor.a + color.r * color.a * (1 - layeredColor.a) / mixedColor.a; // 0.67
				mixedColor.g = layeredColor.g * layeredColor.a / mixedColor.a + color.g * color.a * (1 - layeredColor.a) / mixedColor.a; // 0.33
				mixedColor.b = layeredColor.b * layeredColor.a / mixedColor.a + color.b * color.a * (1 - layeredColor.a) / mixedColor.a; // 0.00
				quadBufferPtr->color = mixedColor;
			}
			quadBufferPtr->texID = textureIndex;
			quadBufferPtr++;
		}

		quadIndexCount += 6;
		quadCount++;
	}

	void Renderer::draw_quad(const glm::mat4& _transform, const Spritesheet& _spritesheet, GLuint _offsetID, int _frames)
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
			if (graphics->get_texture_slots()[i] == _spritesheet.texture->get_object_id())
			{
				textureIndex = (GLfloat) i;
				break;
			}
		}

		if (_spritesheet.columns == 0)
			return;

		GLuint rowOffset = (_spritesheet.rows-1)-(_offsetID / _spritesheet.columns);
		GLuint colOffset = _offsetID % _spritesheet.columns;

		//PRINT("Row offset:" << rowOffset);
		// Change texture index only if ID retrieved is more than 0 (0 is white texture)
		if (textureIndex == 0.f && _spritesheet.spriteID != 0)
		{
			// Add new texture into the texture slot
			textureIndex = (GLfloat) graphics->get_texture_slot_index();
			graphics->get_texture_slots()[graphics->get_texture_slot_index()] = _spritesheet.texture->get_object_id();
			graphics->set_texture_slot_index((GLuint) textureIndex + 1);
		}

		if (!_frames)
			return;

		float xStep = (1.0f / (float)_spritesheet.columns);
		float yStep = (1.0f / (float)_spritesheet.rows);
		float xOffset = colOffset * xStep;
		float yOffset = rowOffset * yStep;
		//PRINT("Y step:" << yStep);
		glm::vec2 spriteTextCoord[4] =
		{
			glm::vec2(xOffset, yOffset),
			glm::vec2(xOffset + xStep, yOffset),
			glm::vec2(xOffset + xStep, yOffset+yStep),
			glm::vec2(xOffset, yOffset+yStep)
		};

		for (GLint i = 0; i < 4; i++)
		{
			quadBufferPtr->pos = _transform * quadVertexPosition[i];
			quadBufferPtr->textCoord = spriteTextCoord[i];
			quadBufferPtr->color = color;
			quadBufferPtr->texID = textureIndex;
			quadBufferPtr++;
		}

		quadIndexCount += 6;
		quadCount++;
	}

	void Renderer::draw_line(const glm::vec3& _position0, const glm::vec3& _position1, const glm::vec4& _color)
	{
		if (lineVertexCount >= maxLineVertexCount)
		{
			end_batch();
			flush();
			begin_batch();
		}

		lineBufferPtr->pos = _position0;
		lineBufferPtr->color = _color;
		lineBufferPtr++;

		lineBufferPtr->pos = _position1;
		lineBufferPtr->color = _color;
		lineBufferPtr++;

		lineVertexCount += 2;
		lineCount++;
	}

	void Renderer::draw_circle(const glm::vec3& _position, const float& _radius, const float _rotation, const glm::vec4& _color)
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
			glm::vec4(_radius, 0.f, 0.f, 0.f),
			glm::vec4(0.f, _radius, 0.f, 0.f),
			glm::vec4(0.f, 0.f, 1.f, 0.f),
			glm::vec4(0.f, 0.f, 0.f, 1.f)
		};

		glm::mat4 transform = translate * rotation * scale;
		draw_circle(transform, _color);
	}

	void Renderer::draw_circle(const glm::mat4& _transform, const glm::vec4& _color)
	{
		if (circleIndexCount >= maxCircleIndexCount)
		{
			end_batch();
			flush();
			begin_batch();
		}

		for (GLint i = 0; i < circleVertices; i++)
		{
			circleBufferPtr->pos = _transform * circleVertexPosition[i];
			circleBufferPtr->color = _color;
			circleBufferPtr++;
		}

		circleIndexCount += circleVertices + 1;
		circleCount++;

		//// Update VBO for each circle
		//CircleVertex vertices[circleVertices];

		//for (GLint i = 0; i < circleVertices; i++)
		//{
		//	vertices[i].pos = _transform * glm::vec4(circleVertexPosition[i], 0.f, 1.f);
		//	vertices[i].color = _color;
		//}

		//// End batch
		//glBindVertexArray(circleVertexArrayID);
		//glNamedBufferSubData(circleVertexBufferID, 0, sizeof(vertices), vertices);
		//glVertexArrayVertexBuffer(circleVertexArrayID, 2, circleVertexBufferID, 0, sizeof(CircleVertex));

		//// Flushing
		//graphics->get_shader_program()[LINE_SHADER].Use();

		//// Bean: Matrix assignment to be placed somewhere else
		//GLuint uProjection = glGetUniformLocation(
		//	graphics->get_shader_program()[LINE_SHADER].GetHandle(), "uViewProjection");

		//glm::mat4 projection = camera->get_view_proj_matrix();
		//glUniformMatrix4fv(uProjection, 1, GL_FALSE, glm::value_ptr(projection));

		//// End of matrix assignment

		//glLineWidth(circleWidth);
		//glDrawArrays(GL_LINE_LOOP, 0, circleVertices);
		//drawCount++;

		//glBindVertexArray(0);
		//graphics->get_shader_program()[LINE_SHADER].UnUse();
	}

	void Renderer::draw_text(const std::string& _text, const glm::vec3& _position, const glm::vec4& _color, const float _scale, Font* _font)
	{
		if (textVertexCount >= maxTextCount)
		{
			end_batch();
			flush();
			begin_batch();
		}

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		graphics->get_shader_program()[TEXT_SHADER].Use();

		GLuint uProjection = glGetUniformLocation(
			graphics->get_shader_program()[TEXT_SHADER].GetHandle(), "uViewProjection");

		glm::mat4 projection = camera->get_view_proj_matrix();
		glUniformMatrix4fv(uProjection, 1, GL_FALSE, glm::value_ptr(projection));

		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(_font->get_VAO());

		float x = _position.x;
		float y = _position.y;
		float z = _position.z;

		glm::vec2 fontTextCoord[6] = {
			glm::vec2(0.f, 0.f),
			glm::vec2(0.f, 1.f),
			glm::vec2(1.f, 1.f),
			glm::vec2(0.f, 0.f),
			glm::vec2(1.f, 1.f),
			glm::vec2(1.f, 0.f)
		};

		int newLine = 0;
		float xpos = 0.f, ypos = 0.f;
		const float scaler = 0.01f;

		std::map<char, Character> characters = _font->get_characters();

		for (char c : _text)
		{
			Character ch = characters[c];

			// If it is a newline
			if (c == '\n')
			{
				newLine++;
				x = _position.x;
				continue;
			}

			xpos = x + ch.bearing.x * (_scale * scaler);
			ypos = y - (ch.size.y - ch.bearing.y) * (_scale * scaler) - newLine * (_scale * 2.5f);

			float w = ch.size.x * (_scale * scaler);
			float h = ch.size.y * (_scale * scaler);

			// Update VBO for each character
			TextVertex vertices[6];
			vertices[0].pos = glm::vec3(xpos, ypos + h, z);
			vertices[1].pos = glm::vec3(xpos, ypos, z);
			vertices[2].pos = glm::vec3(xpos + w, ypos, z);
			vertices[3].pos = glm::vec3(xpos, ypos + h, z);
			vertices[4].pos = glm::vec3(xpos + w, ypos, z);
			vertices[5].pos = glm::vec3(xpos + w, ypos + h, z);

			for (int i = 0; i < 6; i++)
			{
				vertices[i].color = _color;
				vertices[i].textCoord = fontTextCoord[i];
			}

			// Render glyph texture over quad
			glBindTexture(GL_TEXTURE_2D, ch.textureID);

			// Update content of VBO memory
			glNamedBufferSubData(_font->get_VBO(), 0, sizeof(vertices), vertices);
			glVertexArrayVertexBuffer(_font->get_VAO(), 2, _font->get_VBO(), 0, sizeof(TextVertex));

			glDrawArrays(GL_TRIANGLES, 0, 6);

			x += (ch.advance >> 6) * (_scale * scaler); // Bitshift by 6 to get value in pixels
		}
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		graphics->get_shader_program()[TEXT_SHADER].UnUse();
		
		glDisable(GL_BLEND);

		//textCount++;
	}
	
}

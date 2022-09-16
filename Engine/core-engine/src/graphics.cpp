/*!***************************************************************************************
\file			graphics.cpp
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			07/09/2022

\brief
	This file holds the definition of the Graphics class. The Graphic system handles
	various sub-systems which includes asset loading, matrice calculations and rendering.
	Components and objects which require rendering would refer to this class.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"
#include <graphics.h>
#include <renderer.h>
#include <vector>
#include <input.h>
#include <glhelper.h>

namespace Copium::Graphics
{
	// Global variables
	GLfloat movement_x = 0.f, movement_y = 0.f;

	void Graphics::init()
	{

		glClearColor(1.f, 1.f, 1.f, 1.f);

		sceneWidth = 1280;
		sceneHeight = 720;
		glViewport((GLHelper::width - sceneWidth) / 2, (GLHelper::height - sceneHeight) / 2, sceneWidth, sceneHeight);

		init_geometry();
	}

	void Graphics::update()
	{
		glClear(GL_COLOR_BUFFER_BIT);

		if (Input::isKeyPressed(GLFW_KEY_A))
			movement_x -= GLHelper::delta_time;
		else if (Input::isKeyPressed(GLFW_KEY_D))
			movement_x += GLHelper::delta_time;

		if (Input::isKeyPressed(GLFW_KEY_W))
			movement_y += GLHelper::delta_time;
		else if (Input::isKeyPressed(GLFW_KEY_S))
			movement_y -= GLHelper::delta_time;

		static float time = 0.f;
		static bool hasPressed = false;
		if (Input::isKeyPressed(GLFW_KEY_T) && time < 1.f && !hasPressed)
		{
			hasPressed = true;
			Copium::Component::Sprite* sprite = new Copium::Component::Sprite;
			glm::vec2 pos = { Input::getMousePosition().first - GLHelper::width / 2, Input::getMousePosition().second - GLHelper::height / 2 };
			pos.x /= 80.f;
			pos.y /= -45.f;

			sprite->set_position(pos);

			sprite->set_size( glm::vec2(0.1f, 0.1f ));
			sprite->set_color(glm::vec4(0.8f, 0.2f, 0.6f, 1.f));
			sprites.push_back(sprite);

			PRINT("Number of sprites: " << sprites.size() << "\n");
		}
		else if (hasPressed && time < 1.f)
		{
			time += GLHelper::delta_time;
		}
		else if (time > 1.f)
		{
			time = 0.f;
			hasPressed = false;
		}

		draw_world();
	}

	void Graphics::exit()
	{
		glDeleteVertexArrays(1, &vertexArrayID);
		glDeleteBuffers(1, &vertexBufferID);
		glDeleteBuffers(1, &indexBufferID);
		glDeleteTextures(1, &whiteTexture);

		//quadBuffer->clear();
		delete[] quadBuffer;

		for (Copium::Component::Sprite* s : sprites)
			delete s;
	}

	// Create a vertex buffer for the sprites
	void Graphics::init_geometry()
	{
		quadBuffer = new Vertex[maxVertexCount];

		// Vertex Array Object
		glCreateVertexArrays(1, &vertexArrayID);
		glBindVertexArray(vertexArrayID);

		// Vertex Buffer Object		
		glCreateBuffers(1, &vertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
		glNamedBufferData(vertexBufferID, maxVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

		glEnableVertexArrayAttrib(vertexArrayID, 0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *) offsetof(Vertex, pos));

		glEnableVertexArrayAttrib(vertexArrayID, 1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *) offsetof(Vertex, color));

		glEnableVertexArrayAttrib(vertexArrayID, 2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *) offsetof(Vertex, textCoord));

		glEnableVertexArrayAttrib(vertexArrayID, 3);
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *) offsetof(Vertex, texID));

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

		glCreateBuffers(1, &indexBufferID);
		glNamedBufferStorage(indexBufferID, sizeof(indices), indices, GL_DYNAMIC_STORAGE_BIT);

		glVertexArrayElementBuffer(vertexArrayID, indexBufferID);
		glBindVertexArray(0);

		glCreateTextures(GL_TEXTURE_2D, 1, &whiteTexture);
		glBindTexture(GL_TEXTURE_2D, whiteTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		GLuint color = 0xffffffff;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);

		textureSlots.resize(32);
		textureSlots[0] = whiteTexture;

		for (GLuint i = 1; i < maxTextures; i++)
		{
			textureSlots[i] = 0;
		}
	}

	// Load assets into the game
	void load_assets()
	{

	}

	// Load a texture into the game
	void load_texture(const std::string & filename)
	{

	}

	// Setup default world, view and projection matrices (May include orthographic)
	void Graphics::setup_matrices()
	{

	}

	// Draw the debug data
	void Graphics::draw_debug_info()
	{

	}

	// Draw the world
	void Graphics::draw_world()
	{
		setup_matrices();

		batch_render();
	}

	void Graphics::batch_render()
	{
		shaderProgram.Use();

		// Include a loop of draw calls

		// One draw call

		begin_batch();

		// Reference all sprites in the world and draw
		// Overflowing sprites gets pushed to next draw call ( Which means dynamic 0.0 )
		for (int i = 0; i < 10; i++)
		{
			glm::vec4 color = { 0.5f, 0.2f, 0.2f, 1.f };
			draw_quad({ i + movement_x, i + movement_y }, { 0.045f, 0.08f }, color);
		}

		for (size_t i = 0; i < sprites.size(); i++)
		{
			/*PRINT(i + 1 << " : Sprite Data: " << sprites[i].pos.x << "," << sprites[i].pos.y
				<< "\t Size: " << sprites[i].size.x << "," << sprites[i].size.y << "\n");*/

			glm::vec2 pos = { sprites[i]->get_position().x + movement_x, sprites[i]->get_position().y + movement_y};

			draw_quad(pos, sprites[i]->get_size(), sprites[i]->get_color());
		}

		end_batch();

		flush();

		shaderProgram.UnUse();
	}

	void Graphics::begin_batch()
	{
		quadBufferPtr = quadBuffer;
	}

	void Graphics::flush()
	{
		for (GLuint i = 0; i < textureSlotIndex; i++)
			glBindTextureUnit(i, textureSlots[i]);

		glBindVertexArray(vertexArrayID);
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, NULL);

		drawCount++;

		indexCount = 0;
		textureSlotIndex = 1;
	}

	void Graphics::end_batch()
	{
		GLsizeiptr size = (GLuint*)quadBufferPtr - (GLuint*)quadBuffer;
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
		glNamedBufferSubData(vertexBufferID, 0, size * 4, quadBuffer);
	}

	void Graphics::draw_quad(const glm::vec2 & position, const glm::vec2 & size, const glm::vec4 & color)
	{
		if (indexCount >= maxIndexCount)
		{
			end_batch();
			flush();
			begin_batch();
		}

		GLfloat textureIndex = 0.f;

		quadBufferPtr->pos = { position.x, position.y, 0.0f };
		quadBufferPtr->color = color;
		quadBufferPtr->textCoord = { 0.f, 0.f };
		quadBufferPtr->texID = textureIndex;
		quadBufferPtr++;

		quadBufferPtr->pos = { position.x + size.x, position.y, 0.0f };
		quadBufferPtr->color = color;
		quadBufferPtr->textCoord = { 1.f, 0.f };
		quadBufferPtr->texID = textureIndex;
		quadBufferPtr++;

		quadBufferPtr->pos = { position.x + size.x, position.y + size.y, 0.0f };
		quadBufferPtr->color = color;
		quadBufferPtr->textCoord = { 1.f, 1.f };
		quadBufferPtr->texID = textureIndex;
		quadBufferPtr++;

		quadBufferPtr->pos = { position.x, position.y + size.y, 0.0f };
		quadBufferPtr->color = color;
		quadBufferPtr->textCoord = { 0.f, 1.f };
		quadBufferPtr->texID = textureIndex;
		quadBufferPtr++;

		indexCount += 6;
		quadCount++;
	}

	void Graphics::draw_quad(const glm::vec2& position, const glm::vec2& size, int textureID)
	{
		if (indexCount >= maxIndexCount || textureSlotIndex > maxTextures - 1)
		{
			end_batch();
			flush();
			begin_batch();
		}

		glm::vec4 color = { 1.f, 1.f, 1.f, 1.f };

		GLfloat textureIndex = 0.f;

		for (GLuint i = 1; i < textureSlotIndex; i++)
		{
			if (textureSlots[i] == textureID)
			{
				textureIndex = (GLfloat)i;
				break;
			}
		}

		if (textureIndex == 0.f)
		{
			textureIndex = (GLfloat)textureSlotIndex;
			textureSlots[textureSlotIndex] = textureID;
			textureSlotIndex++;
		}

		quadBufferPtr->pos = { position.x, position.y, 0.0f };
		quadBufferPtr->color = color;
		quadBufferPtr->textCoord = { 0.f, 0.f };
		quadBufferPtr->texID = textureIndex;
		quadBufferPtr++;

		quadBufferPtr->pos = { position.x + size.x, position.y, 0.0f };
		quadBufferPtr->color = color;
		quadBufferPtr->textCoord = { 1.f, 0.f };
		quadBufferPtr->texID = textureIndex;
		quadBufferPtr++;

		quadBufferPtr->pos = { position.x + size.x, position.y + size.y, 0.0f };
		quadBufferPtr->color = color;
		quadBufferPtr->textCoord = { 1.f, 1.f };
		quadBufferPtr->texID = textureIndex;
		quadBufferPtr++;

		quadBufferPtr->pos = { position.x, position.y + size.y, 0.0f };
		quadBufferPtr->color = color;
		quadBufferPtr->textCoord = { 0.f, 1.f };
		quadBufferPtr->texID = textureIndex;
		quadBufferPtr++;

		indexCount += 6;
		quadCount++;
	}
}
/*!***************************************************************************************
\file			graphics.cpp
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			07/09/2022

\brief
	This file holds the definition of the GraphicsSystem class. The Graphic system handles
	various sub-systems which includes asset loading, matrice calculations and rendering.
	Components and objects which require rendering would refer to this class.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"
#include <graphics-system.h>
#include <framebuffer.h>
#include <renderer.h>
#include <input.h>
#include <windows-system.h>

namespace Copium::Graphics
{
	using Copium::WindowsSystem;

	GLfloat movement_x = 0.f, movement_y = 0.f;
	GLint index = 0;

	void GraphicsSystem::init()
	{
		glClearColor(1.f, 1.f, 1.f, 1.f);

		// Initialise Viewport
		glViewport(0, 0, windowsSystem.get_window_width(), windowsSystem.get_window_height());

		// Setup Shaders
		setup_shader_program();

		// Initialise Sub systems
		renderer.init(WORLD);
		debugRenderer.init(DEBUG);
		framebuffer.init();

		// Bind textures to fragment shader
		shaderProgram.Use();
		GLuint loc = glGetUniformLocation(shaderProgram.GetHandle(), "uTextures");
		GLint samplers[maxTextures];

		for (GLuint i = 0; i < maxTextures; i++)
			samplers[i] = i;

		glUniform1iv(loc, maxTextures, samplers);

		loc = glGetUniformLocation(shaderProgram.GetHandle(), "uIndex");
		glUniform1i(loc, index);
	}

	void GraphicsSystem::update()
	{
		glClearColor(1.f, 1.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (Input::isKeyHeld(GLFW_KEY_A))
			movement_x -= GLHelper::delta_time;
		else if (Input::isKeyHeld(GLFW_KEY_D))
			movement_x += GLHelper::delta_time;

		if (Input::isKeyHeld(GLFW_KEY_W))
			movement_y += GLHelper::delta_time;
		else if (Input::isKeyHeld(GLFW_KEY_S))
			movement_y -= GLHelper::delta_time;

		if (Input::isKeyPressed(GLFW_KEY_T))
		{
			Copium::Component::SpriteRenderer* sprite = new Copium::Component::SpriteRenderer;
			glm::vec2 pos = { Input::getMousePosition().first - windowsSystem.get_window_width() / 2, Input::getMousePosition().second - windowsSystem.get_window_height() / 2};
			pos.x /= 80.f;
			pos.y /= -45.f;

			sprite->set_position(pos);

			sprite->set_size( glm::vec2(0.1f, 0.1f ));
			sprite->set_color(glm::vec4(0.8f, 0.2f, 0.6f, 1.f));
			sprites.push_back(sprite);
		}

		if (Input::isKeyPressed(GLFW_KEY_Y))
		{
			PRINT("Number of sprites: " << sprites.size());
		}
		
		setup_matrices();

		batch_render();
	}

	void GraphicsSystem::exit()
	{
		renderer.shutdown();
		debugRenderer.shutdown();
		framebuffer.exit();

		for (Copium::Component::SpriteRenderer * s : sprites)
			delete s;
	}

	void GraphicsSystem::init_geometry()
	{
		
	}

	// Load assets into the game
	void load_assets()
	{

	}

	// Load a texture into the game
	void load_texture(const std::string & filename)
	{

	}

	// Setup default shaders for the graphics system
	void GraphicsSystem::setup_shader_program()
	{
		std::vector<std::pair<GLenum, std::string>> shdr_files;
		shdr_files.emplace_back(std::make_pair(GL_VERTEX_SHADER, "../core-engine/Assets/shaders/shader-glsl.vert"));
		shdr_files.emplace_back(std::make_pair(GL_FRAGMENT_SHADER, "../core-engine/Assets/shaders/shader-glsl.frag"));
		shaderProgram.CompileLinkValidate(shdr_files);

		if (GL_FALSE == shaderProgram.IsLinked())
		{
			PRINT("Unable to compile/link/validate shader programs");
			PRINT(shaderProgram.GetLog());
			std::exit(EXIT_FAILURE);
		}
	}

	// Setup default world, view and projection matrices (May include orthographic)
	void GraphicsSystem::setup_matrices()
	{

	}

	// Draw the debug data
	void GraphicsSystem::draw_debug_info()
	{
		index = 1;
		debugRenderer.begin_batch();

		// Reference all debug info in the world and draw
		for (size_t i = 0; i < sprites.size(); i++)
		{
			/*PRINT(i + 1 << " : Sprite Data: " << sprites[i]->get_position().x << "," << sprites[i]->get_position().y
				<< "\t Size: " << sprites[i]->get_size().x << "," << sprites[i]->get_size().y);*/

			glm::vec2 pos = { sprites[i]->get_position().x + movement_x, sprites[i]->get_position().y + movement_y };

			glm::vec4 color = { 0.3f, 1.f, 0.3f, 1.f };

			glm::vec2 pos0 = glm::vec2(pos.x - sprites[i]->get_size().x / 2, pos.y - sprites[i]->get_size().y / 2);
			glm::vec2 pos1 = glm::vec2(pos.x + sprites[i]->get_size().x / 2, pos.y - sprites[i]->get_size().y / 2);
			glm::vec2 pos2 = glm::vec2(pos.x + sprites[i]->get_size().x / 2, pos.y + sprites[i]->get_size().y / 2);
			glm::vec2 pos3 = glm::vec2(pos.x - sprites[i]->get_size().x / 2, pos.y + sprites[i]->get_size().y / 2);

			debugRenderer.draw_line(pos0, pos1, color);
			debugRenderer.draw_line(pos1, pos2, color);
			debugRenderer.draw_line(pos2, pos3, color);
			debugRenderer.draw_line(pos3, pos0, color);

			/*glm::vec4 color = { 0.3f, 1.f, 0.3f, 1.f };

			glm::vec2 pos0 = glm::vec2(pos.x - sprites[i]->get_size().x / 2, pos.y - sprites[i]->get_size().y / 2);
			glm::vec2 pos1 = glm::vec2(pos.x + sprites[i]->get_size().x / 2, pos.y - sprites[i]->get_size().y / 2);
			glm::vec2 pos2 = glm::vec2(pos.x + sprites[i]->get_size().x / 2, pos.y + sprites[i]->get_size().y / 2);
			glm::vec2 pos3 = glm::vec2(pos.x - sprites[i]->get_size().x / 2, pos.y + sprites[i]->get_size().y / 2);

			renderer.draw_line(pos0, pos1, color);
			renderer.draw_line(pos1, pos2, color);
			renderer.draw_line(pos2, pos3, color);
			renderer.draw_line(pos3, pos0, color);*/

			debugRenderer.draw_quad(pos, sprites[i]->get_size(), color);
		}

		debugRenderer.end_batch();

		debugRenderer.flush();
	}

	// Draw the world
	void GraphicsSystem::draw_world()
	{
		index = 0;
		renderer.begin_batch();

		// Reference all sprites in the world and draw
		// Overflowing sprites gets pushed to next draw call ( Which means dynamic 0.0 )
		for (int i = 0; i < 1; i++)
		{
			glm::vec4 color = { 0.5f, 0.2f, 0.2f, 1.f };
			renderer.draw_quad({ i + movement_x, i + movement_y }, { 0.09f, 0.16f }, color);
		}

		for (size_t i = 0; i < sprites.size(); i++)
		{
			/*PRINT(i + 1 << " : Sprite Data: " << sprites[i]->get_position().x << "," << sprites[i]->get_position().y
				<< "\t Size: " << sprites[i]->get_size().x << "," << sprites[i]->get_size().y);*/

			glm::vec2 pos = { sprites[i]->get_position().x + movement_x, sprites[i]->get_position().y + movement_y };

			glm::vec4 color = { 0.3f, 1.f, 0.3f, 1.f };

			glm::vec2 pos0 = glm::vec2(pos.x - sprites[i]->get_size().x / 2, pos.y - sprites[i]->get_size().y / 2);
			glm::vec2 pos1 = glm::vec2(pos.x + sprites[i]->get_size().x / 2, pos.y - sprites[i]->get_size().y / 2);
			glm::vec2 pos2 = glm::vec2(pos.x + sprites[i]->get_size().x / 2, pos.y + sprites[i]->get_size().y / 2);
			glm::vec2 pos3 = glm::vec2(pos.x - sprites[i]->get_size().x / 2, pos.y + sprites[i]->get_size().y / 2);

			renderer.draw_line(pos0, pos1, color);
			renderer.draw_line(pos1, pos2, color);
			renderer.draw_line(pos2, pos3, color);
			renderer.draw_line(pos3, pos0, color);

			//renderer.draw_quad(pos, sprites[i]->get_size(), sprites[i]->get_color());
		}

		renderer.end_batch();

		renderer.flush();
	}

	void GraphicsSystem::batch_render()
	{
		shaderProgram.Use();

		// Include a loop of draw calls

		// One draw call
		framebuffer.bind();

		glClearColor(0.7f, 0.7f, 0.7f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		draw_world();

		//draw_debug_info();

		framebuffer.unbind();

		shaderProgram.UnUse();
	}
}
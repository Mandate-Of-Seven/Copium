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
	GLfloat size_x = 0.f, size_y = 0.f;
	GLfloat rotate = 0.f;

	void GraphicsSystem::init()
	{
		glClearColor(1.f, 1.f, 1.f, 1.f);

		// Initialise Viewport
		glViewport(0, 0, windowsSystem.get_window_width(), windowsSystem.get_window_height());

		// Setup Shaders
		setup_shader_program("../core-engine/Assets/shaders/shader-glsl.vert",
			"../core-engine/Assets/shaders/shader-glsl.frag");

		setup_shader_program("../core-engine/Assets/shaders/line-shader-glsl.vert",
			"../core-engine/Assets/shaders/line-shader-glsl.frag");

		// Initialise Sub systems
		renderer.init();
		framebuffer.init();

		// Bind textures to quad fragment shader
		shaderProgram[0].Use();
		GLuint loc = glGetUniformLocation(shaderProgram[0].GetHandle(), "uTextures");
		GLint samplers[maxTextures];

		for (GLuint i = 0; i < maxTextures; i++)
			samplers[i] = i;

		glUniform1iv(loc, maxTextures, samplers);
	}

	void GraphicsSystem::update()
	{
		movement_x = movement_y = size_x = size_y = 0;

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

		// Create sprites
		if (Input::isKeyHeld(GLFW_KEY_C))
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

		if (Input::isKeyHeld(GLFW_KEY_LEFT_SHIFT) && Input::isKeyPressed(GLFW_KEY_D))
		{
			debugMode = !debugMode;
		}

		if (Input::isKeyHeld(GLFW_KEY_Z) && Input::isKeyHeld(GLFW_KEY_LEFT_SHIFT))
		{
			size_x -= GLHelper::delta_time;
			size_y -= GLHelper::delta_time;
		}
		else if (Input::isKeyHeld(GLFW_KEY_Z))
		{
			size_x += GLHelper::delta_time;
			size_y += GLHelper::delta_time;
		}

		if (Input::isKeyHeld(GLFW_KEY_R) && Input::isKeyHeld(GLFW_KEY_LEFT_SHIFT))
		{
			rotate -= GLHelper::delta_time * 75;
		}
		else if (Input::isKeyHeld(GLFW_KEY_R))
		{
			rotate += GLHelper::delta_time * 75;
		}
		
		setup_matrices();

		batch_render();
	}

	void GraphicsSystem::exit()
	{
		renderer.shutdown();
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
	void GraphicsSystem::setup_shader_program(std::string vtx_shdr, std::string frg_shdr)
	{
		std::vector<std::pair<GLenum, std::string>> shdr_files;
		shdr_files.emplace_back(std::make_pair(GL_VERTEX_SHADER, vtx_shdr));
		shdr_files.emplace_back(std::make_pair(GL_FRAGMENT_SHADER, frg_shdr));

		for (int i = 0; i < 2; i++)
		{
			if (shaderProgram[i].IsLinked())
				continue;

			shaderProgram[i].CompileLinkValidate(shdr_files);

			if (GL_FALSE == shaderProgram[i].IsLinked())
			{
				PRINT("Unable to compile/link/validate shader programs");
				PRINT(shaderProgram[i].GetLog());
				std::exit(EXIT_FAILURE);
			}

			break;
		}
	}

	// Setup default world, view and projection matrices (May include orthographic)
	void GraphicsSystem::setup_matrices()
	{

	}

	// Draw the debug data
	void GraphicsSystem::draw_debug_info()
	{
		renderer.begin_batch();

		// Reference all debug info in the world and draw
		for (size_t i = 0; i < sprites.size(); i++)
		{
			/*PRINT(i + 1 << " : Sprite Data: " << sprites[i]->get_position().x << "," << sprites[i]->get_position().y
				<< "\t Size: " << sprites[i]->get_size().x << "," << sprites[i]->get_size().y);*/

			glm::vec2 pos = { sprites[i]->get_position().x + movement_x, sprites[i]->get_position().y + movement_y };
			sprites[i]->set_position(pos);

			glm::mat4 translate = {
				glm::vec4(1.f, 0.f, 0.f, 0.f),
				glm::vec4(0.f, 1.f, 0.f, 0.f),
				glm::vec4(pos.x, pos.y, 1.f, 0.f),
				glm::vec4(0.f, 0.f, 0.f, 1.f)
			};

			float rad = rotate * 3.14159265359f / 180.f;

			glm::mat4 rotate = {
				glm::vec4(cos(rad), sin(rad), 0.f, 0.f),
				glm::vec4(-sin(rad), cos(rad), 0.f, 0.f),
				glm::vec4(0.f, 0.f, 1.f, 0.f),
				glm::vec4(0.f, 0.f, 0.f, 1.f)
			};

			glm::mat4 transform = translate * rotate;

			glm::vec4 color = { 0.3f, 1.f, 0.3f, 1.f };

			glm::vec4 pos0 = transform * glm::vec4(-sprites[i]->get_size().x / 2, -sprites[i]->get_size().y / 2, 1.f, 1.f);
			glm::vec4 pos1 = transform * glm::vec4(sprites[i]->get_size().x / 2, -sprites[i]->get_size().y / 2, 1.f, 1.f);
			glm::vec4 pos2 = transform * glm::vec4(sprites[i]->get_size().x / 2, sprites[i]->get_size().y / 2, 1.f, 1.f);
			glm::vec4 pos3 = transform * glm::vec4(-sprites[i]->get_size().x / 2, sprites[i]->get_size().y / 2, 1.f, 1.f);

			float minX = fminf(pos0.x, fminf(pos1.x, fminf(pos2.x, pos3.x)));
			float minY = fminf(pos0.y, fminf(pos1.y, fminf(pos2.y, pos3.y)));
			float maxX = fmaxf(pos0.x, fmaxf(pos1.x, fmaxf(pos2.x, pos3.x)));
			float maxY = fmaxf(pos0.y, fmaxf(pos1.y, fmaxf(pos2.y, pos3.y)));

			glm::vec2 pos0_1 = { minX, minY };
			glm::vec2 pos1_1 = { maxX, minY };
			glm::vec2 pos2_1 = { maxX, maxY };
			glm::vec2 pos3_1 = { minX, maxY };

			renderer.draw_line(pos0_1, pos1_1, color);
			renderer.draw_line(pos1_1, pos2_1, color);
			renderer.draw_line(pos2_1, pos3_1, color);
			renderer.draw_line(pos3_1, pos0_1, color);
		}

		renderer.end_batch();

		renderer.flush();
	}

	// Draw the world
	void GraphicsSystem::draw_world()
	{
		// Grid
		renderer.begin_batch();
		glm::vec4 color = { 1.f, 1.f, 1.f, 0.15f };
		for (float i = -1.f + 0.225f; i < 1.f; i += 0.225f)
		{
			renderer.draw_line({ i, -1.f }, { i, 1.f }, color);
		}

		for (float j = -1.f + 0.4f; j < 1.f; j += 0.4f)
		{
			renderer.draw_line({ -1.f, j }, { 1.f, j }, color);
		}

		renderer.end_batch();

		renderer.flush();

		// Sprites and Objects
		renderer.begin_batch();

		// Reference all sprites in the world and draw
		// Overflowing sprites gets pushed to next draw call ( Which means dynamic 0.0 )

		for (size_t i = 0; i < sprites.size(); i++)
		{
			/*PRINT(i + 1 << " : Sprite Data: " << sprites[i]->get_position().x << "," << sprites[i]->get_position().y
				<< "\t Size: " << sprites[i]->get_size().x << "," << sprites[i]->get_size().y);*/

			glm::vec2 pos = { sprites[i]->get_position().x + movement_x, sprites[i]->get_position().y + movement_y };
			glm::vec2 size = { sprites[i]->get_size().x + size_x, sprites[i]->get_size().y + size_y };

			glm::mat4 translate = {
				glm::vec4(1.f, 0.f, 0.f, 0.f),
				glm::vec4(0.f, 1.f, 0.f, 0.f),
				glm::vec4(pos.x, pos.y, 1.f, 0.f),
				glm::vec4(0.f, 0.f, 0.f, 1.f)
			};

			float rad = rotate * 3.14159265359f / 180.f;

			glm::mat4 rotate = {
				glm::vec4(cos(rad), sin(rad), 0.f, 0.f),
				glm::vec4(-sin(rad), cos(rad), 0.f, 0.f),
				glm::vec4(0.f, 0.f, 1.f, 0.f),
				glm::vec4(0.f, 0.f, 0.f, 1.f)
			};

			glm::mat4 transform = translate * rotate;

			sprites[i]->set_position(pos);
			sprites[i]->set_size(size);

			renderer.draw_quad(transform, sprites[i]->get_position(), sprites[i]->get_size(), sprites[i]->get_color());
		}

		renderer.end_batch();

		renderer.flush();
	}

	void GraphicsSystem::batch_render()
	{
		// Include a loop of draw calls

		// One draw call
		framebuffer.bind();

		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		draw_world();

		if(debugMode)
			draw_debug_info();

		framebuffer.unbind();
	}
}
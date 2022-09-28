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
#include <framebuffer.h>
#include <renderer.h>
#include <input.h>
#include <windows-system.h>

namespace Copium::Graphics
{
	using Copium::WindowsSystem;

	// Global variables
	int Graphics::sceneWidth;
	int Graphics::sceneHeight;

	GLfloat movement_x = 0.f, movement_y = 0.f;

	void Graphics::init()
	{
		glClearColor(1.f, 1.f, 1.f, 1.f);

		// Initialise Viewport
		//sceneWidth = 1280;
		//sceneHeight = 720;
		//glViewport((windowsSystem.get_window_width() - sceneWidth) / 2, (windowsSystem.get_window_height() - sceneHeight) / 2, sceneWidth, sceneHeight);
		glViewport(0, 0, windowsSystem.get_window_width(), windowsSystem.get_window_height());

		// Setup Shaders
		setup_shader_program();

		// Initialise Renderer
		renderer.init();

		// Bind textures to fragment shader
		shaderProgram.Use();
		GLuint loc = glGetUniformLocation(shaderProgram.GetHandle(), "uTextures");
		GLint samplers[maxTextures];

		for (GLuint i = 0; i < maxTextures; i++)
			samplers[i] = i;

		glUniform1iv(loc, maxTextures, samplers);
	}

	void Graphics::update()
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

		static float time = 0.f;
		static bool hasPressed = false;
		if (Input::isKeyPressed(GLFW_KEY_T) && time < 1.f && !hasPressed)
		{
			//hasPressed = true;
			Copium::Component::SpriteRenderer* sprite = new Copium::Component::SpriteRenderer;
			glm::vec2 pos = { Input::getMousePosition().first - windowsSystem.get_window_width() / 2, Input::getMousePosition().second - windowsSystem.get_window_height() / 2};
			pos.x /= 80.f;
			pos.y /= -45.f;

			sprite->set_position(pos);

			sprite->set_size( glm::vec2(0.1f, 0.1f ));
			sprite->set_color(glm::vec4(0.8f, 0.2f, 0.6f, 1.f));
			sprites.push_back(sprite);
		}
		else if (hasPressed && time < 0.2f)
		{
			time += GLHelper::delta_time;
		}
		else if (time > 0.2f)
		{
			time = 0.f;
			hasPressed = false;
		}

		if (Input::isKeyPressed(GLFW_KEY_Y))
		{
			PRINT("Number of sprites: " << sprites.size());
		}
		
		draw_world();
	}

	void Graphics::exit()
	{
		renderer.shutdown();

		for (Copium::Component::SpriteRenderer * s : sprites)
			delete s;
	}

	void Graphics::init_geometry()
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
	void Graphics::setup_shader_program()
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
		framebuffer.bind();

		glClearColor(0.7f, 0.7f, 0.7f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		renderer.begin_batch();

		// Reference all sprites in the world and draw
		// Overflowing sprites gets pushed to next draw call ( Which means dynamic 0.0 )
		for (int i = 0; i < 10; i++)
		{
			glm::vec4 color = { 0.5f, 0.2f, 0.2f, 1.f };
			renderer.draw_quad({ i + movement_x, i + movement_y }, { 0.045f, 0.08f }, color);
		}

		for (size_t i = 0; i < sprites.size(); i++)
		{
			/*PRINT(i + 1 << " : Sprite Data: " << sprites[i].pos.x << "," << sprites[i].pos.y
				<< "\t Size: " << sprites[i].size.x << "," << sprites[i].size.y);*/

			glm::vec2 pos = { sprites[i]->get_position().x + movement_x, sprites[i]->get_position().y + movement_y};

			renderer.draw_quad(pos, sprites[i]->get_size(), sprites[i]->get_color());
		}

		renderer.end_batch();

		renderer.flush();

		framebuffer.unbind();

		shaderProgram.UnUse();
	}
}
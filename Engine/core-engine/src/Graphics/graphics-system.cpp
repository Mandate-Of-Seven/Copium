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
#include "Windows/windows-system.h"

#include "Graphics/graphics-system.h"
#include "Graphics/spritesheet.h"
#include "Windows/windows-input.h"

#include "Editor/editor-system.h"
#include "Files/assets-system.h"

// Bean: remove this after NewManagerInstance is moved
#include "GameObject/Components/renderer-component.h"
#include "GameObject/Components/ui-components.h"
#include "SceneManager/sm.h"
#include "Math/math-library.h"

namespace
{
	Copium::InputSystem& inputSystem{ *Copium::InputSystem::Instance()};
}

namespace Copium
{
	// Temporary global variables
	GLfloat movement_x = 0.f, movement_y = 0.f;
	GLfloat size_x = 0.f, size_y = 0.f;
	GLfloat rotate = 0.f;
	bool massSpawn = false, toggleAnimation = false;
	int animID = 0;

	void GraphicsSystem::init()
	{
		systemFlags |= FLAG_RUN_ON_EDITOR | FLAG_RUN_ON_PLAY;

		glClearColor(1.f, 1.f, 1.f, 1.f);

		// Initialise Sub systems
		renderer.init();

		glm::vec2 size = EditorSystem::Instance()->get_scene_view()->get_dimension();
		framebuffer.set_size((GLuint)size.x, (GLuint)size.y);
		framebuffer.init();

		draw.init();

		// Bind textures to quad fragment shader
		shaderProgram[QUAD_SHADER].Use();
		GLuint loc = glGetUniformLocation(shaderProgram[QUAD_SHADER].GetHandle(), "uTextures");
		GLint samplers[maxTextures]{};

		for (GLuint i = 0; i < maxTextures; i++)
			samplers[i] = i;

		glUniform1iv(loc, maxTextures, samplers);
		shaderProgram[QUAD_SHADER].UnUse();

		// Bind fonts to text fragment shader
		/*shaderProgram[TEXT_SHADER].Use();
		loc = glGetUniformLocation(shaderProgram[TEXT_SHADER].GetHandle(), "uFont");
		glUniform1iv(loc, maxTextures, samplers);
		shaderProgram[TEXT_SHADER].UnUse();*/

		// Parse all textures loaded into the engine into the graphics
		// Bean: Seems like i dont need to parse the textures during startup because the 
		//		 Renderer.cpp does it for me if the texture has not been stored, enable 
		//		 to improve runtime rendering(no spikes)
		parse_textures();
	}

	void GraphicsSystem::update()
	{
		GLfloat dt = (GLfloat) WindowsSystem::Instance()->get_delta_time();
		movement_x = movement_y = 0;

		glClearColor(1.f, 1.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		/*if (inputSystem.is_key_held(GLFW_KEY_A))
			movement_x -= dt;
		else if (!inputSystem.is_key_held(GLFW_KEY_LEFT_SHIFT) && inputSystem.is_key_held(GLFW_KEY_D))
			movement_x += dt;

		if (inputSystem.is_key_held(GLFW_KEY_W))
			movement_y += dt;
		else if (inputSystem.is_key_held(GLFW_KEY_S))
			movement_y -= dt;*/

		// Create sprites
		//glm::vec2 mousePos{0}, centreOfScene{0}, mouseScenePos{0}, mouseToNDC{0}, worldNDC{0};
		//if (!inputSystem.is_key_held(GLFW_KEY_LEFT_SHIFT) && inputSystem.is_key_pressed(GLFW_KEY_C))
		//{
		//	Sprite* sprite = new Sprite;
		//	EditorSystem* editor = EditorSystem::Instance();
		//	glm::vec2 scenePos = editor->get_scene_view()->get_position();
		//	glm::vec2 sceneDim = editor->get_scene_view()->get_dimension();
		//	glm::vec2 cameraPos = editor->get_camera()->get_position();
		//	float zoom = editor->get_camera()->get_zoom();
		//	// Mouse to scene view conversion
		//	mousePos = inputSystem.get_mouseposition().to_glm();
		//	centreOfScene = { scenePos.x + sceneDim.x / 2, scenePos.y + sceneDim.y / 2 };
		//	mouseScenePos = { mousePos.x - centreOfScene.x, centreOfScene.y - mousePos.y };
		//	mouseToNDC = { mouseScenePos.x / sceneDim.y * 2, mouseScenePos.y / sceneDim.y * 2 + 0.1f };
		//	mouseToNDC *= zoom;
		//	worldNDC = { mouseToNDC.x + cameraPos.x, mouseToNDC.y + cameraPos.y };

		//	glm::vec3 pos = glm::vec3(worldNDC, 0.f);

		//	sprite->set_position(pos);
		//	
		//	sprite->set_size( glm::vec2(1.f, 1.f));
		//	sprite->set_color(glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
		//	sprites.push_back(sprite);
		//	
		//}

		if (inputSystem.is_key_held(GLFW_KEY_LEFT_SHIFT) && inputSystem.is_key_pressed(GLFW_KEY_C))
		{
			massSpawn = !massSpawn;
		}

		// Mass spawning
		if (massSpawn)
		{
			NewSceneManager* sm = NewSceneManager::Instance();
			Scene* scene = sm->get_current_scene();
			if (scene != nullptr)
			{
				for (size_t i = 0; i < 10; i++)
				{
					GameObject* go = sm->get_gof().build_gameobject();
					go->addComponent(ComponentType::SpriteRenderer);
					//go->addComponent(ComponentType::Rigidbody);

					float x = rand() % 2000 * 0.1f - 100.f;
					float y = rand() % 2000 * 0.1f - 100.f;

					go->transform.set_position({ x, y, 0.f });
					SpriteRenderer* rc = reinterpret_cast<SpriteRenderer*>(go->getComponent(ComponentType::SpriteRenderer));
					rc->get_sprite_renderer().set_sprite_id(rand() % 7 + 15);
				}
			}
		}

		/*PRINT("Mouse position: " << mousePos.x << ", " << mousePos.y);
		PRINT("Centre position: " << centreOfScene.x << ", " << centreOfScene.y);
		PRINT("Mouse NDC position: " << mouseToNDC.x << ", " << mouseToNDC.y);
		PRINT("World NDC position: " << worldNDC.x << ", " << worldNDC.y);*/

		if (inputSystem.is_key_pressed(GLFW_KEY_Y))
		{
			NewSceneManager* sm = NewSceneManager::Instance();
			PRINT("Number of Gameobjects: " << sm->get_current_scene()->get_gameobjcount());
		}

		if (inputSystem.is_key_held(GLFW_KEY_LEFT_SHIFT) && inputSystem.is_key_pressed(GLFW_KEY_A))
		{
			toggleAnimation = !toggleAnimation;
		}

		if (inputSystem.is_key_held(GLFW_KEY_LEFT_SHIFT) && inputSystem.is_key_pressed(GLFW_KEY_S))
		{
			animID = (animID == 1) ? 0 : 1;
		}

		static bool debugMode = false;
		if (inputSystem.is_key_held(GLFW_KEY_LEFT_SHIFT) && inputSystem.is_key_pressed(GLFW_KEY_D))
		{
			debugMode = !debugMode;
		}

		if (debugMode)
			draw.enable(DRAW::DEBUG);
		else if (!debugMode)
			draw.disable(DRAW::DEBUG);
		
		if (inputSystem.is_key_held(GLFW_KEY_Z) && inputSystem.is_key_held(GLFW_KEY_LEFT_SHIFT))
		{
			size_x -= dt;
			size_y -= dt;
		}
		else if (inputSystem.is_key_held(GLFW_KEY_Z))
		{
			size_x += dt;
			size_y += dt;
		}

		/*if (inputSystem.is_key_held(GLFW_KEY_R) && inputSystem.is_key_held(GLFW_KEY_LEFT_SHIFT))
		{
			rotate -= dt * 75;
		}
		else if (inputSystem.is_key_held(GLFW_KEY_R))
		{
			rotate += dt * 75;
		}*/

		if (draw.get_draw_mode(DRAW::EDITOR))
			batch_render_editor();
		else
			batch_render_game();
	}

	void GraphicsSystem::exit()
	{
		Font::cleanUp();

		renderer.shutdown();
		framebuffer.exit();
		draw.exit();
	}

	// Setup default shaders for the graphics system
	void GraphicsSystem::setup_shader_program(std::string _vtx_shdr, std::string _frg_shdr)
	{
		std::vector<std::pair<GLenum, std::string>> shdr_files;
		shdr_files.emplace_back(std::make_pair(GL_VERTEX_SHADER, _vtx_shdr));
		shdr_files.emplace_back(std::make_pair(GL_FRAGMENT_SHADER, _frg_shdr));

		for (int i = 0; i < NUM_SHADERS; i++)
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

	// parse all textures into the game
	void GraphicsSystem::parse_textures()
	{
		AssetsSystem* assets = AssetsSystem::Instance();
		// Check for texture slots
		COPIUM_ASSERT(textureSlotIndex == maxTextures, "Max textures reached! Replace old textures!!");

		// Assign the slot to the texture
		textureSlotIndex = 1;
		for (GLuint i = 0; i < assets->get_textures().size(); i++)
		{
			textureSlots[textureSlotIndex++] = assets->get_textures()[i].get_object_id();
		}
	}
	
	// Renders the objects in the engine in batches
	void GraphicsSystem::batch_render_editor()
	{
		// Binds the framebuffer
		framebuffer.bind();
		//glEnable(GL_DEPTH_TEST);
		
		// Clear the screen
		glClearColor(0.278f, 0.278f, 0.278f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		draw.update();

		// Unbind the framebuffer to display renderable
		// onto the image
		framebuffer.unbind();
	}

	void GraphicsSystem::batch_render_game()
	{
		//glEnable(GL_DEPTH_TEST);

		// Clear the screen
		glClearColor(0.278f, 0.278f, 0.278f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		draw.update();
	}
}
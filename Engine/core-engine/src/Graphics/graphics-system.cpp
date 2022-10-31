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

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"
#include "Windows/windows-system.h"

#include "Graphics/graphics-system.h"
#include "Graphics/sprite-renderer.h"
#include "Windows/windows-input.h"

#include "Editor/editor-system.h"
#include "Files/assets-system.h"

// Bean: remove this after NewManagerInstance is moved
#include "GameObject/renderer-component.h"
#include "Graphics/ui-components.h"
#include "SceneManager/sm.h"

namespace Copium::Graphics
{
	// Temporary global variables
	GLfloat movement_x = 0.f, movement_y = 0.f;
	GLfloat size_x = 0.f, size_y = 0.f;
	GLfloat rotate = 0.f;
	bool massSpawn = false;

	void GraphicsSystem::init()
	{
		PRINT("\n---------------- Loading Graphics System ----------------\n");

		glClearColor(1.f, 1.f, 1.f, 1.f);
		
		// Setup Shaders
		setup_shader_program("Assets/shaders/shader-glsl.vert",
			"Assets/shaders/shader-glsl.frag");

		setup_shader_program("Assets/shaders/line-shader-glsl.vert",
			"Assets/shaders/line-shader-glsl.frag");

		setup_shader_program("Assets/shaders/text-shader-glsl.vert",
			"Assets/shaders/text-shader-glsl.frag");

		// Initialise Sub systems
		renderer.init();

		glm::vec2 size = Copium::EditorSystem::Instance()->get_scene_view()->get_dimension();
		framebuffer.set_size((GLuint)size.x, (GLuint)size.y);
		framebuffer.init();

		// Bind textures to quad fragment shader
		shaderProgram[0].Use();
		GLuint loc = glGetUniformLocation(shaderProgram[0].GetHandle(), "uTextures");
		GLint samplers[maxTextures];

		for (GLuint i = 0; i < maxTextures; i++)
			samplers[i] = i;

		glUniform1iv(loc, maxTextures, samplers);
		shaderProgram[0].UnUse();

		// Bind fonts to text fragment shader
		/*shaderProgram[2].Use();
		loc = glGetUniformLocation(shaderProgram[2].GetHandle(), "uFont");
		glUniform1iv(loc, maxTextures, samplers);
		shaderProgram[2].UnUse();*/

		// Parse all textures loaded into the engine into the graphics 
		parse_textures();

		//if (NewSceneManager::Instance())
		//{
		//	NewSceneManager::Instance()->get_gof().add_component_creator(RENDERER_CREATOR, new RendererCreator);
		//}
		PRINT("\n---------------- Graphics System Completed ----------------\n");
	}

	void GraphicsSystem::update()
	{
		GLfloat dt = (GLfloat) WindowsSystem::Instance()->get_delta_time();
		movement_x = movement_y = 0;

		glClearColor(1.f, 1.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		/*if (Input::is_key_held(GLFW_KEY_A))
			movement_x -= dt;
		else if (!Input::is_key_held(GLFW_KEY_LEFT_SHIFT) && Input::is_key_held(GLFW_KEY_D))
			movement_x += dt;

		if (Input::is_key_held(GLFW_KEY_W))
			movement_y += dt;
		else if (Input::is_key_held(GLFW_KEY_S))
			movement_y -= dt;*/

		// Create sprites
		glm::vec2 mousePos{0}, centreOfScene{0}, mouseScenePos{0}, mouseToNDC{0}, worldNDC{0};
		if (!inputSystem.is_key_held(GLFW_KEY_LEFT_SHIFT) && inputSystem.is_key_pressed(GLFW_KEY_C))
		{
			SpriteRenderer* sprite = new SpriteRenderer;
			Copium::Editor::EditorSystem* editor = Copium::Editor::EditorSystem::Instance();
			glm::vec2 scenePos = editor->get_scene_view()->get_position();
			glm::vec2 sceneDim = editor->get_scene_view()->get_dimension();
			glm::vec2 cameraPos = editor->get_camera()->get_position();
			float zoom = editor->get_camera()->get_zoom();
			// Mouse to scene view conversion
			mousePos = inputSystem.get_mouseposition().to_glm();
			centreOfScene = { scenePos.x + sceneDim.x / 2, scenePos.y + sceneDim.y / 2 };
			mouseScenePos = { mousePos.x - centreOfScene.x, centreOfScene.y - mousePos.y };
			mouseToNDC = { mouseScenePos.x / sceneDim.y * 2, mouseScenePos.y / sceneDim.y * 2 + 0.1f };
			mouseToNDC *= zoom;
			worldNDC = { mouseToNDC.x + cameraPos.x, mouseToNDC.y + cameraPos.y };

			glm::vec3 pos = glm::vec3(worldNDC, 0.f);

			sprite->set_position(pos);
			
			sprite->set_size( glm::vec2(1.f, 1.f));
			sprite->set_color(glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
			sprites.push_back(sprite);
			
		}

		if (Input::is_key_held(GLFW_KEY_LEFT_SHIFT) && Input::is_key_pressed(GLFW_KEY_C))
		{
			massSpawn = !massSpawn;
		}

		// Mass spawning
		if (massSpawn)
		{

			for (size_t i = 0; i < 10; i++)
			{
				SpriteRenderer* sprite = new SpriteRenderer;

				glm::vec3 pos = { rand() % 200 * 0.1f - 10.f, rand() % 200 * 0.1f - 10.f , 0.f};

				sprite->set_position(pos);

				sprite->set_size(glm::vec2(1.f, 1.f));
				sprite->set_color(glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
				sprites.push_back(sprite);
			}
		}

		/*PRINT("Mouse position: " << mousePos.x << ", " << mousePos.y);
		PRINT("Centre position: " << centreOfScene.x << ", " << centreOfScene.y);
		PRINT("Mouse NDC position: " << mouseToNDC.x << ", " << mouseToNDC.y);
		PRINT("World NDC position: " << worldNDC.x << ", " << worldNDC.y);*/

		if (Input::is_key_pressed(GLFW_KEY_Y))
		{
			PRINT("Number of sprites: " << sprites.size());
		}

		if (Input::is_key_held(GLFW_KEY_LEFT_SHIFT) && Input::is_key_pressed(GLFW_KEY_D))
		{
			debugMode = !debugMode;
		}
		
		if (inputSystem.is_key_held(GLFW_KEY_Z) && inputSystem.is_key_held(GLFW_KEY_LEFT_SHIFT))
		{
			size_x -= dt;
			size_y -= dt;
		}
		else if (Input::is_key_held(GLFW_KEY_Z))
		{
			size_x += dt;
			size_y += dt;
		}

		/*if (Input::is_key_held(GLFW_KEY_R) && Input::is_key_held(GLFW_KEY_LEFT_SHIFT))
		{
			rotate -= dt * 75;
		}
		else if (Input::is_key_held(GLFW_KEY_R))
		{
			rotate += dt * 75;
		}*/
		
		setup_matrices();

		batch_render();
	}

	void GraphicsSystem::exit()
	{
		Font::cleanUp();

		renderer.shutdown();
		framebuffer.exit();

		for (SpriteRenderer * s : sprites)
			delete s;
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

	// Setup default world, view and projection matrices (May include orthographic)
	void GraphicsSystem::setup_matrices()
	{

	}

	// Load assets into the game
	void load_assets()
	{

	}

	// parse all textures into the game
	void GraphicsSystem::parse_textures()
	{
		Copium::Files::AssetsSystem* assets = Copium::Files::AssetsSystem::Instance();
		// Check for texture slots
		COPIUM_ASSERT(textureSlotIndex == maxTextures, "Max textures reached! Replace old textures!!");

		// Assign the slot to the texture
		for (GLuint i = 0; i < assets->get_textures().size(); i++)
		{
			textureSlots[textureSlotIndex++] = assets->get_textures()[i].get_object_id();
		}
	}
	
	// Renders the objects in the engine in batches
	void GraphicsSystem::batch_render()
	{
		// Binds the framebuffer
		framebuffer.bind();
		//glEnable(GL_DEPTH_TEST);
		
		// Clear the screen
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Draw the world using batch rendering
		draw_world();

		if(debugMode)
			draw_debug_info();

		glm::vec3 position = { 0.f, 0.f, 0.f };
		glm::vec4 color = { 1.f, 1.f, 1.f, 1.f };
		Font::getFont("corbel")->draw_text("Corbel", position, color, 0.4f + size_x, 0);


		float red = 0.f, green = 1.f;
		static float timer = 0.f; 
		timer += (float)WindowsSystem::Instance()->get_delta_time();
		static bool switcher = false;

		if (timer >= 1.f && switcher)
		{
			timer = 0.f;
			switcher = false;
		}
		else if (timer >= 1.f && !switcher)
		{
			timer = 0.f;
			switcher = true;
		}

		if (switcher)

		{
			red = (0.1f * (1.0 - timer)) + (0.9f * timer);
			green = (0.9f * (1.0 - timer)) + (0.1f * timer);
		}
		else
		{
			red = (0.9f * (1.0 - timer)) + (0.1f * timer);
			green = (0.1f * (1.0 - timer)) + (0.9f * timer);
		}

		position = { 0.f, 2.f, 0.f };
		color = { red, green, 1.f, 1.f };
		Font::getFont("Comfortaa-Regular")->draw_text("Hello Comfortaa Here...", position, color, 0.6f + size_x, 0);

		// Unbind the framebuffer to display renderable
		// onto the image
		framebuffer.unbind();
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

			glm::vec3 pos = { sprites[i]->get_position().x + movement_x, sprites[i]->get_position().y + movement_y, 0.f };
			sprites[i]->set_position(pos);

			glm::mat4 translate = {
				glm::vec4(1.f, 0.f, 0.f, 0.f),
				glm::vec4(0.f, 1.f, 0.f, 0.f),
				glm::vec4(pos.x, pos.y, 1.f, 0.f),
				glm::vec4(0.f, 0.f, 0.f, 1.f)
			};

			float rad = glm::radians(rotate);

			glm::mat4 rotation = {
				glm::vec4(cos(rad), sin(rad), 0.f, 0.f),
				glm::vec4(-sin(rad), cos(rad), 0.f, 0.f),
				glm::vec4(0.f, 0.f, 1.f, 0.f),
				glm::vec4(0.f, 0.f, 0.f, 1.f)
			};

			glm::mat4 transform = translate * rotation;

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
		// Bean: Should be under draw editor, along with gizmos ( Maybe replace draw_debug_info with draw_editor)
		// Grid
		renderer.begin_batch();
		glm::vec4 color = { 1.f, 1.f, 1.f, 0.2f };
		float start = -100.f, end = -start;
		float numDivision = 24.f, iteration = (end - start) / numDivision;
		for (float i = start; i < end + iteration; i += iteration)
		{
			renderer.draw_line({ i, start }, { i, end }, color);
			renderer.draw_line({ start, i }, { end, i }, color);
		}

		color = { 1.f, 1.f, 1.f, 0.1f };
		float subDivision = 5.f;
		numDivision *= subDivision;
		iteration = (end - start) / numDivision;
		start = -10.f, end = -start;
		for (float i = start; i <= end; i += iteration)
		{
			renderer.draw_line({ i, start }, { i, end }, color);
			renderer.draw_line({ start, i }, { end, i }, color);
		}

		renderer.end_batch();

		renderer.flush();

		// Spriqtes and Objects
		renderer.begin_batch();

		// Reference all sprites in the world and draw
		// Overflowing sprites gets pushed to next draw call ( Which means dynamic 0.0 )
		
		// Background
		// Bean: scale should be the scale of the object, 
		// texture scale should be separate and derived from the image dimensions
		// Scale = image scale / default scale(1024)
		Copium::AssetsSystem* assets = Copium::AssetsSystem::Instance();
		renderer.draw_quad({ 0.f, 0.f, 0.f }, { 3.84f * 2.5f, 2.16f * 2.5f }, 0.f, assets->get_textures()[0].get_object_id());
		
		color = { 0.1f, 1.f, 0.1f, 1.f };
		glm::vec2 worldNDC{ 0 };
		Copium::Editor::EditorSystem* editor = Copium::Editor::EditorSystem::Instance();
		glm::vec2 cameraPos = editor->get_camera()->get_position();
		float zoom = editor->get_camera()->get_zoom();
		worldNDC = { cameraPos.x, cameraPos.y };
		glm::vec2 scale = { 0.01f, 0.01f };
		scale *= zoom;

		//PRINT("World NDC position: " << worldNDC.x << ", " << worldNDC.y);
		// Bean: Temporary green dot in the centre of the scene
		renderer.draw_quad({ worldNDC.x, worldNDC.y , 1.f}, scale, 0.f, color);

		/*
			Bean Theory:
			For each gameobject, check if it has a renderer component
				If it does:
					Check if it is active
						If not: Continue
					Check if it has a spriteID of not 0 (default)
						If it does:
							Render sprite with color
						Else
							Render default sprite (white texture) with color
				Else
					Continue

			Bean: To replace the following code with the Theory
		*/
		for (size_t i = 0; i < sprites.size(); i++)
		{
			/*PRINT(i + 1 << " : Sprite Data: " << sprites[i]->get_position().x << "," << sprites[i]->get_position().y
				<< "\t Size: " << sprites[i]->get_size().x << "," << sprites[i]->get_size().y);*/

			int textureSelector = i % 5 + 1;

			glm::vec3 pos = { sprites[i]->get_position().x + movement_x, sprites[i]->get_position().y + movement_y, 0.f };
			glm::vec2 size = { sprites[i]->get_size().x + size_x, sprites[i]->get_size().y + size_y};

			sprites[i]->set_position(pos);
			sprites[i]->set_size(size);

			// Bean: Set sprite id should be done in the editor or via deserialization
			sprites[i]->set_sprite_id(assets->get_textures()[i % 4 + 1].get_object_id());
			sprites[i]->set_texture(&assets->get_textures()[i % 4 + 1]);

			if(textureSelector == 5) // Alpha Colored Square
				renderer.draw_quad(pos, { 0.1f, 0.1f }, rotate, sprites[i]->get_color());
			else
				renderer.draw_quad(pos, size, rotate, *sprites[i]);
		}

		// Theory WIP
		Copium::NewSceneManager* sm = Copium::NewSceneManager::Instance();
		Scene* scene = sm->get_current_scene();
		for (GameObject* gameObject : scene->get_gameobjectvector())
		{
			for (Component* component : gameObject->Components())
			{
				if (!component->Enabled())
					continue;

				TransformComponent& t = gameObject->Transform();
				RendererComponent * rc = reinterpret_cast<RendererComponent*>(component);
				SpriteRenderer sr = rc->get_sprite_renderer();
				glm::vec2 size(t.glmScale().x, t.glmScale().y);
				float rotate = t.glmRotation().z;
				// Bean: It should be set in inspector view of the renderer component instead
				unsigned int id = sr.get_sprite_id() - 1;
				if (id == -1)
					id = 0;
				sr.set_texture(&assets->get_textures()[id]);
				renderer.draw_quad(t.glmPosition(), size, rotate, sr);
			}
			for (Component* component : gameObject->getComponents<UITextComponent>())
			{
				if (!component->Enabled())
					continue;
				UITextComponent* textComponent = reinterpret_cast<UITextComponent*>(component);
				textComponent->render();
			}
		}

		// Bean : Testing Animations
		if (!assets->get_spritesheets().empty())
		{
			glm::vec3 position(-3.f, 1.f, 0.f);
			glm::vec2 size(2.f, 2.f);
			static GLuint animationID = 0;
			GLuint indexSize = assets->get_spritesheets()[0].get_size() - 1;

			GLfloat dt = (GLfloat) Windows::WindowsSystem::Instance()->get_delta_time();
			static float timer = 0.f;
			timer += dt;
			if (timer > 0.01f)
			{
				timer = 0.f;
				animationID++;
			}

			if (animationID > indexSize)
			{
				animationID = 0;
			}

			renderer.draw_quad(position, size, 0.f, assets->get_spritesheets()[0], animationID, 10);

		}
		
		// Bean : Testing Text
		/*glm::vec3 position = { 0.f, -1.f, 0.f };
		color = { 1.f, 1.f, 0.f, 1.f };
		renderer.draw_text("Testing Arial", position, color, 0.1f, 0);*/
		
		renderer.end_batch();

		renderer.flush();
	}
}
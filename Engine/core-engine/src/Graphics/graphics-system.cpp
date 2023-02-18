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
#include <Debugging/frame-rate-controller.h>

#include "Graphics/graphics-system.h"
#include "Graphics/spritesheet.h"
#include "Graphics/fonts.h"
#include "Windows/windows-input.h"

#include "Files/assets-system.h"

// Bean: remove this after NewManagerInstance is moved
#include "SceneManager/scene-manager.h"
#include <GameObject/components.h>

namespace Copium
{
	
	namespace
	{
		ComponentsArray<Camera*> pCamerasArray{};
		InputSystem& inputSystem{ *InputSystem::Instance() };

		// Temporary global variables
		GLfloat rotate = 0.f;
		bool massSpawn = false;
	}

	void GraphicsSystem::init()
	{
		systemFlags |= FLAG_RUN_ON_EDITOR | FLAG_RUN_ON_PLAY;

		// Bean: 3D Depth Testing
		glEnable(GL_DEPTH_TEST);
		//glAlphaFunc(GL_GREATER, 0.5);
		//glEnable(GL_ALPHA_TEST);
		//glEnable(GL_STENCIL_TEST);

		glClearColor(1.f, 1.f, 1.f, 1.f);

		//renderer.init();

		// Bind textures to quad fragment shader
		shaderProgram[QUAD_SHADER].Use();
		GLuint loc1 = glGetUniformLocation(shaderProgram[QUAD_SHADER].GetHandle(), "uTexture1");
		/*GLuint loc2 = glGetUniformLocation(shaderProgram[QUAD_SHADER].GetHandle(), "uTexture2");
		GLuint loc3 = glGetUniformLocation(shaderProgram[QUAD_SHADER].GetHandle(), "uTexture3");
		GLuint loc4 = glGetUniformLocation(shaderProgram[QUAD_SHADER].GetHandle(), "uTexture4");
		GLuint loc5 = glGetUniformLocation(shaderProgram[QUAD_SHADER].GetHandle(), "uTexture5");
		GLuint loc6 = glGetUniformLocation(shaderProgram[QUAD_SHADER].GetHandle(), "uTexture6");
		GLuint loc7 = glGetUniformLocation(shaderProgram[QUAD_SHADER].GetHandle(), "uTexture7");
		GLuint loc8 = glGetUniformLocation(shaderProgram[QUAD_SHADER].GetHandle(), "uTexture8");

		GLuint loc9 = glGetUniformLocation(shaderProgram[QUAD_SHADER].GetHandle(), "uTexture9");
		GLuint loc10 = glGetUniformLocation(shaderProgram[QUAD_SHADER].GetHandle(), "uTexture10");
		GLuint loc11 = glGetUniformLocation(shaderProgram[QUAD_SHADER].GetHandle(), "uTexture11");
		GLuint loc12 = glGetUniformLocation(shaderProgram[QUAD_SHADER].GetHandle(), "uTexture12");
		GLuint loc13 = glGetUniformLocation(shaderProgram[QUAD_SHADER].GetHandle(), "uTexture13");
		GLuint loc14 = glGetUniformLocation(shaderProgram[QUAD_SHADER].GetHandle(), "uTexture14");
		GLuint loc15 = glGetUniformLocation(shaderProgram[QUAD_SHADER].GetHandle(), "uTexture15");
		GLuint loc16 = glGetUniformLocation(shaderProgram[QUAD_SHADER].GetHandle(), "uTexture16");

		GLuint loc17 = glGetUniformLocation(shaderProgram[QUAD_SHADER].GetHandle(), "uTexture17");
		GLuint loc18 = glGetUniformLocation(shaderProgram[QUAD_SHADER].GetHandle(), "uTexture18");
		GLuint loc19 = glGetUniformLocation(shaderProgram[QUAD_SHADER].GetHandle(), "uTexture19");
		GLuint loc20 = glGetUniformLocation(shaderProgram[QUAD_SHADER].GetHandle(), "uTexture20");
		GLuint loc21 = glGetUniformLocation(shaderProgram[QUAD_SHADER].GetHandle(), "uTexture21");
		GLuint loc22 = glGetUniformLocation(shaderProgram[QUAD_SHADER].GetHandle(), "uTexture22");
		GLuint loc23 = glGetUniformLocation(shaderProgram[QUAD_SHADER].GetHandle(), "uTexture23");
		GLuint loc24 = glGetUniformLocation(shaderProgram[QUAD_SHADER].GetHandle(), "uTexture24");

		GLuint loc25 = glGetUniformLocation(shaderProgram[QUAD_SHADER].GetHandle(), "uTexture25");
		GLuint loc26 = glGetUniformLocation(shaderProgram[QUAD_SHADER].GetHandle(), "uTexture26");
		GLuint loc27 = glGetUniformLocation(shaderProgram[QUAD_SHADER].GetHandle(), "uTexture27");
		GLuint loc28 = glGetUniformLocation(shaderProgram[QUAD_SHADER].GetHandle(), "uTexture28");
		GLuint loc29 = glGetUniformLocation(shaderProgram[QUAD_SHADER].GetHandle(), "uTexture29");
		GLuint loc30 = glGetUniformLocation(shaderProgram[QUAD_SHADER].GetHandle(), "uTexture30");
		GLuint loc31 = glGetUniformLocation(shaderProgram[QUAD_SHADER].GetHandle(), "uTexture31");
		GLuint loc32 = glGetUniformLocation(shaderProgram[QUAD_SHADER].GetHandle(), "uTexture32");*/
		GLint samplers[maxTextures]{};

		for (GLuint i = 1; i < maxTextures; i++)
			samplers[i] = i;

		/*glUniform1i(loc1, samplers[1]);
		glUniform1i(loc2, samplers[2]);
		glUniform1i(loc3, samplers[3]);
		glUniform1i(loc4, samplers[4]);
		glUniform1i(loc5, samplers[5]);
		glUniform1i(loc6, samplers[6]);
		glUniform1i(loc7, samplers[7]);
		glUniform1i(loc8, samplers[8]);
		glUniform1i(loc9, samplers[9]);
		glUniform1i(loc10, samplers[10]);
		glUniform1i(loc11, samplers[11]);
		glUniform1i(loc12, samplers[12]);
		glUniform1i(loc13, samplers[13]);
		glUniform1i(loc14, samplers[14]);
		glUniform1i(loc15, samplers[15]);
		glUniform1i(loc16, samplers[16]);
		glUniform1i(loc17, samplers[17]);
		glUniform1i(loc18, samplers[18]);
		glUniform1i(loc19, samplers[19]);
		glUniform1i(loc20, samplers[20]);
		glUniform1i(loc21, samplers[21]);
		glUniform1i(loc22, samplers[22]);
		glUniform1i(loc23, samplers[23]);
		glUniform1i(loc24, samplers[24]);
		glUniform1i(loc25, samplers[25]);
		glUniform1i(loc26, samplers[26]);
		glUniform1i(loc27, samplers[27]);
		glUniform1i(loc28, samplers[28]);
		glUniform1i(loc29, samplers[29]);
		glUniform1i(loc30, samplers[30]);
		glUniform1i(loc31, samplers[31]);
		glUniform1i(loc32, samplers[32]);*/
		glUniform1iv(loc1, maxTextures, samplers);
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

		// Bean: Checking for archetypes, to be removed once we found a better implementation
		loaded = true;
		//cameras.pop_front();
	}

	void GraphicsSystem::update()
	{
		//GLfloat dt = (GLfloat) MyFrameRateController.getDt();

		glClearColor(1.f, 1.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		//if (inputSystem.is_key_held(GLFW_KEY_LEFT_SHIFT) && inputSystem.is_key_pressed(GLFW_KEY_C) && inputSystem.is_key_pressed(GLFW_KEY_S))
		//{
		//	massSpawn = !massSpawn;
		//}

		//if (inputSystem.is_key_held(GLFW_KEY_C) && inputSystem.is_key_pressed(GLFW_KEY_V))
		//{
		//	SceneManager* sm = SceneManager::Instance();
		//	Scene* scene = sm->get_current_scene();
		//	if (scene != nullptr)
		//	{
		//		for (size_t i = 0; i < 10; i++)
		//		{
		//			GameObject* go = MyGOF.instantiate();
		//			SpriteRenderer& rc = go->AddComponent<SpriteRenderer>();
		//			//go->addComponent(ComponentType::Rigidbody);

		//			float x = rand() % 2000 * 0.1f - 100.f;
		//			float y = rand() % 2000 * 0.1f - 100.f;

		//			go->transform.position = { x, y, 0.f };
		//			
		//			int numSprites = (int)(AssetsSystem::Instance()->get_textures().size() - 1);
		//			rc.spriteID=(rand() % numSprites + 1);

		//			unsigned int id = (unsigned int)rc.spriteID;
		//			if (id != 0)
		//			{
		//				rc.refTexture = AssetsSystem::Instance()->get_texture(id - 1);
		//				std::string str = AssetsSystem::Instance()->get_texture(id - 1)->get_file_path();
		//				//size_t pos = str.find_last_of('\\');
		//				//std::string spriteName = str.substr(pos + 1, str.length() - pos);
		//			}
		//		}
		//	}
		//}

		// Mass spawning
		//if (massSpawn)
		//{
		//	SceneManager* sm = SceneManager::Instance();
		//	Scene* scene = sm->get_current_scene();
		//	if (scene != nullptr)
		//	{
		//		for (size_t i = 0; i < 10; i++)
		//		{
		//			GameObject* go = MyGOF.instantiate();
		//			SpriteRenderer& rc = go->AddComponent<SpriteRenderer>();
		//			//go->addComponent(ComponentType::Rigidbody);

		//			float x = rand() % 2000 * 0.1f - 100.f;
		//			float y = rand() % 2000 * 0.1f - 100.f;

		//			go->transform.position = { x, y, 0.f };
		//			int numSprites = (int)(AssetsSystem::Instance()->get_textures().size() - 1);
		//			rc.spriteID = rand() % numSprites + 1;
		//			unsigned int id = (unsigned int)rc.spriteID;
		//			if (id != 0)
		//			{
		//				rc.refTexture = AssetsSystem::Instance()->get_texture(id - 1);
		//				//std::string str = AssetsSystem::Instance()->get_texture(id - 1)->get_file_path();
		//				//size_t pos = str.find_last_of('\\');
		//				//std::string spriteName = str.substr(pos + 1, str.length() - pos);
		//				//rc.get_sprite_renderer().set_name(spriteName);
		//			}
		//		}
		//	}
		//}

		/*PRINT("Mouse position: " << mousePos.x << ", " << mousePos.y);
		PRINT("Centre position: " << centreOfScene.x << ", " << centreOfScene.y);
		PRINT("Mouse NDC position: " << mouseToNDC.x << ", " << mouseToNDC.y);
		PRINT("World NDC position: " << worldNDC.x << ", " << worldNDC.y);*/

		//if (inputSystem.is_key_pressed(GLFW_KEY_Y))
		//{
		//	SceneManager* sm = SceneManager::Instance();
		//	PRINT("Number of Gameobjects: " << sm->get_current_scene()->get_gameobjcount());
		//}
	
		batch_render();
	}

	void GraphicsSystem::exit()
	{
		Font::cleanUp();
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

	void GraphicsSystem::batch_render()
	{
		for (Camera& camera : *pCamerasArray)
		{
			PRINT("UPDATING CAMERA");
			camera->update();
		}
	}
}
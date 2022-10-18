/*!***************************************************************************************
\file			graphics.h
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			07/09/2022

\brief
	This file holds the declaration of the Graphics class. The Graphic system handles
	various sub-systems which includes asset loading, matrice calculations and rendering.
	Components and objects which require rendering would refer to this class.

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef GRAPHICS_SYSTEM_H
#define GRAPHICS_SYSTEM_H

#include "CopiumCore/system-interface.h"

#include "Graphics/glslshader.h"
#include "Graphics/textures.h"
#include "Graphics/sprite-renderer.h"
#include "Graphics/framebuffer.h"
#include "Graphics/renderer.h"

#include "SceneManager/sm.h"

namespace Copium::Graphics
{

	// Global variables
	static const GLuint maxQuadCount = 1000; // Number of sprites per batch
	static const GLuint maxLineCount = 1000; // Number of lines per batch
	static const GLuint maxVertexCount = maxQuadCount * 4;
	static const GLuint maxIndexCount = maxQuadCount * 6;
	static const GLuint maxLineVertexCount = maxLineCount * 2;
	static const GLuint maxTextures = 32;

	CLASS_SYSTEM(GraphicsSystem) // Inherits from System
	{
	public:
		// Constructors

		/***************************************************************************/
		/*!
		\brief
			Initialises the graphics system and the viewport
		*/
		/***************************************************************************/
		void init();

		/***************************************************************************/
		/*!
		\brief
			Updates the matrices and rendering of the objects in the engine
		*/
		/***************************************************************************/
		void update();

		/***************************************************************************/
		/*!
		\brief
			Exits the renderer and framebuffer
		*/
		/***************************************************************************/
		void exit();

		// Accessing Properties

		// Scene Properties
		GLuint const get_scene_width() { return sceneWidth; }
		void const set_scene_width(GLuint _width) { sceneWidth = _width; }

		GLuint const get_scene_height() { return sceneHeight; }
		void const set_scene_height(GLuint _height) { sceneHeight = _height; }

		glm::vec2 const get_scene_position() { return scenePosition; }
		void const set_scene_position(glm::vec2 _position) { scenePosition = _position; }

		// Texture Properties
		std::vector<GLuint>& get_texture_slots() { return textureSlots; }

		GLuint const get_texture_slot_index() { return textureSlotIndex; }
		void const set_texture_slot_index(GLuint _index) { textureSlotIndex = _index; }
		
		GLuint& get_white_texture() { return whiteTexture; }
		GLuint const get_white_texture_slot() { return whiteTextureSlot; }

		// Data Members
		GLSLShader* const get_shader_program() { return shaderProgram; }
		void add_sprite(SpriteRenderer * _sprite) { sprites.push_back(_sprite); }
		std::vector<SpriteRenderer*> const get_sprites() { return sprites; }
		Framebuffer get_framebuffer() { return framebuffer; }

#pragma region MemberFunctions
	private:
		// Member functions 
		
		/***************************************************************************/
		/*!
		\brief
			Setup default shaders for the graphics system and binds the vertex and 
			fragment shader to the GLSLshader program
		\param vtx_shdr
			The vertex shader to bind
		\param frg_shdr
			The fragment shader to bind
		*/
		/***************************************************************************/
		void setup_shader_program(std::string _vtx_shdr, std::string _frg_shdr);

		/***************************************************************************/
		/*!
		\brief
			Setup default world, view and projection matrices 
			(May include orthographic)
		*/
		/***************************************************************************/
		void setup_matrices();
		
		/***************************************************************************/
		/*!
		\brief
			Load assets into the engine
		*/
		/***************************************************************************/
		void load_assets();

		/***************************************************************************/
		/*!
		\brief
			Loads a texture into the engine
		\param _filePath
			The file path to access to load the texture
		*/
		/***************************************************************************/
		void load_texture(const std::string& _filePath);

		/***************************************************************************/
		/*!
		\brief
			Renders the objects in the engine in batches
		*/
		/***************************************************************************/
		void batch_render();

		/***************************************************************************/
		/*!
		\brief
			Draw the debug mode of the engine
		*/
		/***************************************************************************/
		void draw_debug_info();

		/***************************************************************************/
		/*!
		\brief
			Draw the "world" of the engine which is the scene view
		*/
		/***************************************************************************/
		void draw_world();

#pragma endregion MemberFunctions
#pragma region DataMembers
	private:
		/* Camera View / Scene View *****************************************************/
		// [Camera Here] (Bean: Should be a component instead?)
		GLuint sceneWidth = 0;
		GLuint sceneHeight = 0;
		glm::vec2 scenePosition{0};

		/* Stored Texture Assets ********************************************************/
		std::vector<GLuint> textureSlots;
		GLuint textureSlotIndex = 1; // Initializes with 1
		GLuint whiteTexture = 0;
		GLuint whiteTextureSlot = 0;

		/* Projections & Matrices *******************************************************/
		glm::mat4 projMatrix{0};
		glm::mat4 viewMatrix{0};
		glm::mat4 viewProjMatrix{0};

		/* Shaders **********************************************************************/
		GLSLShader shaderProgram[2]; // Shader program to use

		/* Stored Information ***********************************************************/
		std::vector<Texture> textures;
		std::vector<SpriteRenderer*> sprites;

		Renderer renderer;
		Framebuffer framebuffer;

		bool debugMode = false;
#pragma endregion DataMembers
	};

}

#endif GRAPHICS_SYSTEM_H
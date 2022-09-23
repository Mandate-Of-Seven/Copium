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

#include <glslshader.h>
#include <GLFW/glfw3.h>

#include "system-interface.h"
#include "vertex-types.h"
#include "sprite-renderer.h"
#include "framebuffer.h"
#include "renderer.h"
#include "textures.h"

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
		void init();

		void update();

		void exit();

		std::vector<GLuint> & get_texture_slots() { return textureSlots; }
		GLuint & get_texture_slot_index() { return textureSlotIndex; }
		GLuint & get_white_texture() { return whiteTexture; }
		GLuint & get_white_texture_slot() { return whiteTextureSlot; }

	private:

		// Setup default shaders for the graphics system
		void setup_shader_program(std::string vtx_shdr, std::string frg_shdr);

		// Setup default world, view and projection matrices (May include orthographic)
		void setup_matrices();

		// Draw the debug data
		void draw_debug_info();

		// Draw the world
		void draw_world();

		// Batch Rendering
		void batch_render();

		// Create a vertex buffer for the sprites
		void init_geometry();

		// Load assets into the game
		void load_assets();

		// Load a texture into the game
		void load_texture(const std::string & _filePath);

	public:
		/* Camera View / Scene View *****************************************************/
		// [Camera Here] (Bean: Should be a component instead?)
		GLuint sceneWidth;
		GLuint sceneHeight;
		glm::vec2 scenePosition;

		/* Stored Texture Assets ********************************************************/
		std::vector<Texture> textures;
		std::vector<GLuint> textureSlots;
		GLuint textureSlotIndex = 1; // Initializes with 1
		GLuint whiteTexture = 0;
		GLuint whiteTextureSlot = 0;

		/* Projections & Matrices *******************************************************/
		glm::mat4 projMatrix;
		glm::mat4 viewMatrix;
		glm::mat4 viewProjMatrix;

		/* Shaders **********************************************************************/
		GLSLShader shaderProgram[2]; // Shader program to use

		/* Stored Information ***********************************************************/
		std::vector<SpriteRenderer*> sprites;

		Renderer renderer;
		Framebuffer framebuffer;

		bool debugMode;
	};

}

#endif GRAPHICS_SYSTEM_H
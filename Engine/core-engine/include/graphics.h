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

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <glslshader.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include "vertex-types.h"
#include <sprite-renderer.h>

namespace Copium::Graphics
{
	// Global variables
	static const GLuint maxQuadCount = 100;
	static const GLuint maxVertexCount = maxQuadCount * 4;
	static const GLuint maxIndexCount = maxQuadCount * 6;
	static const GLuint maxTextures = 32;

	class Graphics // Inherits from System
	{
	public:
		void init();

		void update();

		void exit();

	private:

		// Create a vertex buffer for the sprites
		void init_geometry();

		// Load assets into the game
		void load_assets();

		// Load a texture into the game
		void load_texture(const std::string& filename);

		// Setup default world, view and projection matrices (May include orthographic)
		void setup_matrices();

		// Draw the debug data
		void draw_debug_info();

		// Draw the world
		void draw_world();

		// Batch Rendering
		void batch_render();

		void begin_batch();

		void flush();

		void end_batch();

		// Quad Creation
		void draw_quad(const glm::vec2 & position, const glm::vec2 & size, const glm::vec4 & color);

		void draw_quad(const glm::vec2& position, const glm::vec2& size, int textureID);

	public:

		/* Camera view / Scene View *****************************************************/
		// [Camera Here] (Should be a component instead?)
		int sceneWidth;
		int sceneHeight;

		/* Stored Texture Assets ********************************************************/

		std::vector<GLuint> textureSlots;
		GLuint textureSlotIndex = 1; // Initializes with 1
		GLuint whiteTexture = 0;
		GLuint whiteTextureSlot = 0;

		/* Render Data ******************************************************************/

		GLuint drawCount = 0; // The amount of draw calls
		GLuint quadCount = 0; // The amount of quads drawn

		GLuint vertexArrayID = 0; // Handle to Vertex Array Object
		GLuint vertexBufferID = 0; // Handle to Vertex Buffer Object
		GLuint indexBufferID = 0; // Handle to Index Buffer
		GLuint indexCount = 0; // Number of elements in the object

		//std::vector<Vertex> * quadBuffer;
		Vertex * quadBuffer = nullptr;
		Vertex * quadBufferPtr = nullptr;

		/* Projections & Matrices *******************************************************/
		glm::mat4 projMatrix;
		glm::mat4 viewMatrix;
		glm::mat4 viewProjMatrix;

		/* Shaders **********************************************************************/
		GLSLShader shaderProgram; // Shader program to use

		/* Stored Information ***********************************************************/
		std::vector<Copium::Component::SpriteRenderer*> sprites;
	};

	static Graphics graphics;
}

#endif GRAPHICS_H
/*!***************************************************************************************
\file			renderer.h
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			17/07/2022

\brief
	This file holds the declaration of the Renderer class. The Renderer renders all 
	objects in the game/scene. Use this class to access the renderer of any object,
	sprite or particle system (if any). Renderers can be disabled to make objects
	invisble (see enabled).

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef RENDERER_H
#define RENDERER_H

#include <glslshader.h>

// Global variables
static const GLuint maxQuadCount = 1000;
static const GLuint maxVertexCount = maxQuadCount * 4;
static const GLuint maxIndexCount = maxQuadCount * 6;
static const GLuint maxTextures = 32;

// Renders objects in the game / scene
class Renderer //(Inherits from the Component Class)
{
public:

	// Constructors
	/***************************************************************************/
	/*!
	\brief
		Objects are rendered in batches (Batch rendering). This function creates 
		the vertex array object, vertex buffer object, index buffer and stores 
		the elements of the object to be used for rendering
	*/
	/**************************************************************************/
	Renderer(); // Initializes the renderer by storing a handle to VAO

	/***************************************************************************/
	/*!
	\brief
		Flushes the buffer and complete remaining draw calls and shuts down the 
		rendering system when exiting
	*/
	/**************************************************************************/
	~Renderer();

	// Member Functions

	/***************************************************************************/
	/*!
	\brief
		Begins batching the draw calls of the renderer into the buffer for 
		rendering
	*/
	/**************************************************************************/
	static void begin_batch();

	/***************************************************************************/
	/*!
	\brief
		Flushes the buffer and renders the remaining draw calls
	*/
	/**************************************************************************/
	static void flush();

	/***************************************************************************/
	/*!
	\brief
		Ends the batch by clearing the buffer
	*/
	/**************************************************************************/
	static void end_batch();

	/***************************************************************************/
	/*!
	\brief
		Draws the quad onto the game window
	\param position
		The position of the quad
	\param size
		The size of the quad
	\param color
		The color of the quad
	*/
	/**************************************************************************/
	static void draw_quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
	
	/***************************************************************************/
	/*!
	\brief
		Draws the quad onto the game window
	\param position
		The position of the quad
	\param size
		The size of the quad
	\param textureID
		The index of the texture to be used
	*/
	/**************************************************************************/
	static void draw_quad(const glm::vec2& position, const glm::vec2& size, int textureID);

	// Accessing Properties

	/***************************************************************************/
	/*!
	\brief
		Gets the draw count of the renderer
	\return
		The draw count
	*/
	/**************************************************************************/
	static const GLint& GetDrawCount() { return drawCount; }

	/***************************************************************************/
	/*!
	\brief
		Sets the draw count of the renderer
	\param count
		The number of draw counts
	\return
		The draw count
	*/
	/**************************************************************************/
	static const GLint & SetDrawCount(const GLint& count) { drawCount = count; }

	/***************************************************************************/
	/*!
	\brief
		Gets the quad count of the renderer
	\return
		The quad count
	*/
	/**************************************************************************/
	static const GLint& GetQuadCount() { return quadCount; }

	/***************************************************************************/
	/*!
	\brief
		Sets the quad count of the renderer
	\param count
		The number of quad counts
	\return
		The quad count
	*/
	/**************************************************************************/
	static const GLint & SetQuadCount(const GLint & count) { quadCount = count; }

	/***************************************************************************/
	/*!
	\brief
		Resets the number of draw and quad count
	*/
	/**************************************************************************/
	static void ResetStats() { drawCount = quadCount = 0; }

private:
	// Properties (Variables)
	bool enabled; // Is the renderer enabled

	static GLint drawCount; // The amount of draw calls
	static GLint quadCount; // The amount of quads drawn

	static std::array<GLuint, maxTextures> textureSlots;
	GLuint textureSlotIndex = 1;

	GLSLShader shaderProgram; // Shader program to use
	GLuint vertexArrayID; // Handle to Vertex Array Object
	GLuint vertexBufferID; // Handle to Vertex Buffer Object
	GLuint indexBufferID; // Handle to Index Buffer
	GLuint indexCount; // Number of elements in the object
};

#endif // !RENDERER_H

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

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

/* DEPRECIATED */

#ifndef RENDERER_H
#define RENDERER_H

#include <glslshader.h>
#include <array>
#include "vertex-types.h"

namespace Copium::Graphics 
{

	// Renders objects in the game / scene
	// Checks objects that have the Renderer component 
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
		void init(); // Initializes the renderer by storing a handle to VAO

		/***************************************************************************/
		/*!
		\brief
			Flushes the buffer and complete remaining draw calls and shuts down the
			rendering system when exiting
		*/
		/**************************************************************************/
		void shutdown();

		// Member Functions

		/***************************************************************************/
		/*!
		\brief
			Begins batching the draw calls of the renderer into the buffer for
			rendering
		*/
		/**************************************************************************/
		void begin_batch();

		/***************************************************************************/
		/*!
		\brief
			Flushes the buffer and renders the remaining draw calls
		*/
		/**************************************************************************/
		void flush();

		/***************************************************************************/
		/*!
		\brief
			Ends the batch by clearing the buffer
		*/
		/**************************************************************************/
		void end_batch();

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
		void draw_quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);

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
		void draw_quad(const glm::vec2& position, const glm::vec2& size, int textureID);

		// Accessing Properties

		/***************************************************************************/
		/*!
		\brief
			Gets the draw count of the renderer
		\return
			The draw count
		*/
		/**************************************************************************/
		//const GLint & GetDrawCount() { return drawCount; }

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
		//const GLint & SetDrawCount(const GLint& count) { return drawCount = count; }

		/***************************************************************************/
		/*!
		\brief
			Gets the quad count of the renderer
		\return
			The quad count
		*/
		/**************************************************************************/
		//const GLint & GetQuadCount() { return quadCount; }

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
		//const GLint & SetQuadCount(const GLint & count) { return quadCount = count; }

		/***************************************************************************/
		/*!
		\brief
			Resets the number of draw and quad count
		*/
		/**************************************************************************/
		//void reset_stats() { s_Data.drawCount = s_Data.quadCount = 0; };

		GLuint & get_vertex_array_id() { return vertexArrayID; }

	private:

		/* Render Data ******************************************************************/

		GLuint drawCount = 0; // The amount of draw calls
		GLuint quadCount = 0; // The amount of quads drawn

		GLuint vertexArrayID = 0; // Handle to Vertex Array Object
		GLuint vertexBufferID = 0; // Handle to Vertex Buffer Object
		GLuint indexBufferID = 0; // Handle to Index Buffer
		GLuint indexCount = 0; // Number of elements in the object

		Vertex * quadBuffer = nullptr;
		Vertex * quadBufferPtr = nullptr;
	};

	static Renderer renderer;
}

#endif // !RENDERER_H


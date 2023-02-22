/*!***************************************************************************************
\file			renderer.h
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			21/09/2022

\brief
	This file holds the declaration of the Renderer class. The Renderer renders all
	objects in the game/scene. Use this class to access the renderer of any object,
	sprite or particle system (if any). Renderers can be disabled to make objects
	invisble (see enabled).

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef RENDERER_H
#define RENDERER_H

#include "Graphics/vertex-types.h"
#include "Graphics/glslshader.h"

#include "Graphics/spritesheet.h"
#include "Graphics/sprite.h"
#include "Graphics/fonts.h"

namespace Copium
{
	// Global variables
	static const GLuint maxQuadCount = 1000;	// Number of sprites per batch
	static const GLuint maxLineCount = 1000;	// Number of lines per batch
	static const GLuint maxCircleCount = 200;	// Number of circles per batch
	static const GLuint maxTextCount = 1000;	// Number of text per batch
	static const GLuint maxVertexCount = maxQuadCount * 4;
	static const GLuint maxIndexCount = maxQuadCount * 6;
	static const GLuint maxLineVertexCount = maxLineCount * 2;
	static const GLuint circleVertices = 36;
	static const GLuint maxCircleIndexCount = maxCircleCount * (circleVertices + 1);
	static const GLuint maxCircleVertexCount = maxCircleCount * circleVertices;
	static const GLuint maxTextVertexCount = maxTextCount * 4;
	static const GLuint maxTextures = 256;

	// Forward declare
	class GraphicsSystem;
	class BaseCamera;

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
		/***************************************************************************/
		void init(BaseCamera* _camera); // Initializes the renderer by storing a handle to VAO

		/***************************************************************************/
		/*!
		\brief
			Flushes the buffer and complete remaining draw calls and shuts down the
			rendering system when exiting
		*/
		/***************************************************************************/
		void shutdown();

		// Member Functions

		/***************************************************************************/
		/*!
		\brief
			Setup the quad's vertex array object along with its data
		*/
		/***************************************************************************/
		void setup_quad_vao();

		/***************************************************************************/
		/*!
		\brief
			Setup the line's vertex array object along with its data
		*/
		/***************************************************************************/
		void setup_line_vao();

		/***************************************************************************/
		/*!
		\brief
			Setup the cirlce's vertex array object along with its data
		*/
		/***************************************************************************/
		void setup_circle_vao();

		/***************************************************************************/
		/*!
		\brief
			Setup the text's vertex array object along with its data
		*/
		/***************************************************************************/
		void setup_text_vao();

		/***************************************************************************/
		/*!
		\brief
			Begins batching the draw calls of the renderer into the buffer for
			rendering
		*/
		/***************************************************************************/
		void begin_batch();

		/***************************************************************************/
		/*!
		\brief
			Flushes the buffer and renders the remaining draw calls
		*/
		/***************************************************************************/
		void flush();

		/***************************************************************************/
		/*!
		\brief
			Ends the batch by clearing the buffer
		*/
		/***************************************************************************/
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
		/***************************************************************************/
		void draw_quad(const glm::vec3& _position, const glm::vec2& _scale, const float _rotation, const glm::vec4& _color);

		/***************************************************************************/
		/*!
		\brief
			Draws the quad onto the game window
		\param position
			The position of the quad
		\param size
			The size of the quad
		\param textureID
			The texture to use
		*/
		/***************************************************************************/
		void draw_quad(const glm::vec3& _position, const glm::vec2& _scale, const float _rotation, const GLuint _textureID);

		/***************************************************************************/
		/*!
		\brief
			Draws the quad onto the game window
		\param position
			The position of the quad
		\param size
			The size of the quad
		\param sprite
			The sprite to use to draw
		*/
		/***************************************************************************/
		void draw_quad(const glm::vec3& _position, const glm::vec2& _scale, const float _rotation, const Sprite& _sprite, glm::fvec4* tintColor = nullptr);

		/***************************************************************************/
		/*!
		\brief
			Draws the quad onto the game window
		\param position
			The position of the quad
		\param scale
			The size of the quad
		\param rotation
			The rotation of the quad
		\param spritesheet
			The spritesheet to reference
		\param offset
			The index offset of the spritesheet
		\param textureID
			The index of the texture to be used
		*/
		/***************************************************************************/
		void draw_quad(const glm::vec3& _position, const glm::vec2& _scale, const float _rotation, const Spritesheet& _spritesheet, GLuint _offsetID, int _frames);

		/***************************************************************************/
		/*!
		\brief
			Draws the quad onto the game window
		\param transform
			The transform of the quad
		\param position
			The position of the quad
		\param size
			The size of the quad
		\param color
			The color of the quad
		*/
		/***************************************************************************/
		void draw_quad(const glm::mat4& _transform, const glm::vec4& _color);

		/***************************************************************************/
		/*!
		\brief
			Draws the quad onto the game window
		\param transform
			The transform of the quad
		\param textureID
			The texture to use to draw
		*/
		/***************************************************************************/
		void draw_quad(const glm::mat4& _transform, const GLuint _textureID);

		/***************************************************************************/
		/*!
		\brief
			Draws the quad onto the game window
		\param transform
			The transform of the quad
		\param sprite
			The sprite to use to draw
		*/
		/***************************************************************************/
		void draw_quad(const glm::mat4& _transform, const Sprite& _sprite, glm::fvec4* tint = nullptr);

		/***************************************************************************/
		/*!
		\brief
			Draws the quad onto the game window
		\param transform
			The transform of the quad
		\param spritesheet
			The sprite sheet reference
		\param offsetID
			The index offset of the spritesheet
		\param textureID
			The index of the texture to be used
		*/
		/***************************************************************************/
		void draw_quad(const glm::mat4& _transform, const Spritesheet& _spritesheet, GLuint _offsetID, int _frames);

		/***************************************************************************/
		/*!
		\brief
			Draws a line onto the game window
		\param position0
			The starting position of the line
		\param position1
			The ending position of the line
		\param color
			The color of the line
		*/
		/***************************************************************************/
		void draw_line(const glm::vec3& _position0, const glm::vec3& _position1, const glm::vec4& _color);

		void draw_circle(const glm::vec3& _position, const float& _radius, const float _rotation, const glm::vec4& _color);

		void draw_circle(const glm::mat4& _transform, const glm::vec4& _color);

		/***************************************************************************/
		/*!
		\brief
			Draws a text onto the game window
		\param position
			The position of the text
		\param color
			The color of the text
		\param _scale
			The size of the text
		\param _font
			The font to draw
		*/
		/***************************************************************************/
		void draw_text(const std::string& _text, const glm::vec3& _position, const glm::vec4& _color, const float _scale, Font* _font);

		// Accessing Properties

		void set_line_width(GLfloat _lineWidth) { lineWidth = _lineWidth; }
		GLfloat get_line_width() const { return lineWidth; }

		void set_circle_width(GLfloat _circleWidth) { circleWidth = _circleWidth; }
		GLfloat get_circle_width() const { return circleWidth; }

		const GLuint& getDrawCount() { return drawCount; }
		const GLuint& getQuadCount() { return quadCount; }
		const GLuint& getLineCount() { return lineCount; }
		const GLuint& getCircleCount() { return circleCount; }
		const GLuint& getTextCount() { return textCount; }

		//const GLuint& getVertices() { return (unsigned int)(quadCount * 4 + lineCount * 2 + circleCount * circleVertices + textCount * 4); }

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
		//const GLint& set_quad_count(const GLint& _count) { return quadCount = _count; }

		/***************************************************************************/
		/*!
		\brief
			Resets the number of draw, quad and line count
		*/
		/***************************************************************************/
		void reset_stats() { drawCount = quadCount = lineCount = circleCount = textCount = 0; };

	private:

		/* Render Data ******************************************************************/

		GLuint drawCount = 0;	// The amount of draw calls
		GLuint quadCount = 0;	// The amount of quads drawn
		GLuint lineCount = 0;	// The amount of lines drawn
		GLuint circleCount = 0; // The amount of circle drawn
		GLuint textCount = 0;	// The amount of text drawn

		GLuint quadVertexArrayID = 0;	// Handle to Quad Vertex Array Object
		GLuint lineVertexArrayID = 0;	// Handle to Line Vertex Array Object
		GLuint circleVertexArrayID = 0; // Handle to Circle Vertex Array Object
		GLuint textVertexArrayID = 0;	// Handle to Text Vertex Array Object

		GLuint quadVertexBufferID = 0;	// Handle to Quad Vertex Buffer Object
		GLuint quadIndexBufferID = 0;	// Handle to Quad Index Buffer
		GLuint quadIndexCount = 0;		// Number of elements in the Quad object

		GLuint lineVertexBufferID = 0;	// Handle to Line Vertex Buffer Object
		GLuint lineVertexCount = 0;		// Number of elements in the Line object

		GLuint circleVertexBufferID = 0;// Handle to Circle Vertex Buffer Object
		GLuint circleIndexBufferID = 0;	// Handle to Circle Index Buffer
		GLuint circleIndexCount = 0;	// Number of elements in the Circle object

		GLuint textVertexBufferID = 0;	// Handle to Text Vertex Buffer Object
		GLuint textVertexCount = 0;		// Number of elements in the Text object

		QuadVertex* quadBuffer = nullptr;
		QuadVertex* quadBufferPtr = nullptr;

		LineVertex* lineBuffer = nullptr;
		LineVertex* lineBufferPtr = nullptr;
		GLfloat lineWidth = 1.f;

		CircleVertex* circleBuffer = nullptr;
		CircleVertex* circleBufferPtr = nullptr;
		GLfloat circleWidth = 1.f;

		TextVertex* textBuffer = nullptr;
		TextVertex* textBufferPtr = nullptr;

		glm::vec4 quadVertexPosition[4];
		glm::vec2 quadTextCoord[4];
		glm::vec4 circleVertexPosition[circleVertices];
		glm::vec2 textTextCoord[6];

		GraphicsSystem* graphics = nullptr; // A pointer to the instance of graphics system
		BaseCamera* camera = nullptr;		// A pointer to the camera that holds this renderer class
	};
}

#endif // !RENDERER_H

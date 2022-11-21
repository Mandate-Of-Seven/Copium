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
#ifndef GRAPHICS_SYSTEM_H
#define GRAPHICS_SYSTEM_H

#include "CopiumCore/system-interface.h"

#include "Graphics/glslshader.h"
#include "Graphics/base-camera.h"
#include "Messaging/message-system.h"

namespace Copium
{
	// Global variables
	static const GLuint maxQuadCount = 1000;	// Number of sprites per batch
	static const GLuint maxLineCount = 1000;	// Number of lines per batch
	static const GLuint maxCircleCount = 500;	// Number of circles per batch
	static const GLuint maxTextCount = 1000;	// Number of text per batch
	static const GLuint maxVertexCount = maxQuadCount * 4;
	static const GLuint maxIndexCount = maxQuadCount * 6;
	static const GLuint maxLineVertexCount = maxLineCount * 2;
	static const GLuint maxCircleVertexCount = maxCircleCount * 2;
	static const GLuint maxTextVertexCount = maxTextCount * 4;
	static const GLuint maxTextures = 256;

	enum
	{
		LINE_SHADER,
		QUAD_SHADER,
		TEXT_SHADER,
		NUM_SHADERS
	};

	// Inherits from System
	CLASS_SYSTEM(GraphicsSystem), public IReceiver
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

		/**************************************************************************/
		/*!
			\brief
			Interface function for MessageSystem to call for IReceivers to handle
			a messageType
		*/
		/**************************************************************************/
		void handleMessage(MESSAGE_TYPE mType);

		// Accessing Properties

		// Texture Properties
		std::vector<GLuint>& get_texture_slots() { return textureSlots; }
		void set_texture_slots(GLuint _index, GLuint _value) { textureSlots[_index] = _value; }

		GLuint const get_texture_slot_index() { return textureSlotIndex; }
		void const set_texture_slot_index(GLuint _index) { textureSlotIndex = _index; }
		
		GLuint& get_white_texture() { return whiteTexture; }
		GLuint const get_white_texture_slot() { return whiteTextureSlot; }

		// Data Members
		GLSLShader* const get_shader_program() { return shaderProgram; }

		const bool& is_loaded() const { return loaded; }
		
		std::list<BaseCamera*>& get_cameras() { return cameras; }

#pragma region MemberFunctions
		// Public Member Functions

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

	private:
		// Private Member functions 

		/***************************************************************************/
		/*!
		\brief
			Loads a texture into the engine
		\param _filePath
			The file path to access to load the texture
		*/
		/***************************************************************************/
		void parse_textures();

		/***************************************************************************/
		/*!
		\brief
			Renders all objects in the camera container in batches
		*/
		/***************************************************************************/
		void batch_render();

#pragma endregion MemberFunctions
#pragma region DataMembers
	private:
		/* Stored Texture Assets ********************************************************/
		std::vector<GLuint> textureSlots;
		GLuint textureSlotIndex = 1; // Initializes with 1
		GLuint whiteTexture = 0;
		GLuint whiteTextureSlot = 0;

		/* Shaders **********************************************************************/
		GLSLShader shaderProgram[NUM_SHADERS]; // Shader program to use

		/* Stored Information ***********************************************************/
		bool loaded = false;

		std::list<BaseCamera*> cameras; // Stores the reference to the cameras in the engine

#pragma endregion DataMembers
	};

}

#endif GRAPHICS_SYSTEM_H
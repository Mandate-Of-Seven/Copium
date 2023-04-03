/*!***************************************************************************************
\file			graphics.h
\project
\author			Sean Ngo

\par			Course: GAM250
\par			Section:
\date			07/09/2022

\brief
	This file holds the declaration of the Graphics class. The Graphic system handles
	various sub-systems which includes asset loading, matrice calculations and rendering.
	Components and objects which require rendering would refer to this class.

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef GRAPHICS_SYSTEM_H
#define GRAPHICS_SYSTEM_H

#include "CopiumCore/system-interface.h"

#include "Graphics/glslshader.h"
#include "Graphics/base-camera.h"
#include "Messaging/message-system.h"

#define MyGraphicsSystem (*Copium::GraphicsSystem::Instance())

namespace Copium
{
	enum
	{
		LINE_SHADER,
		QUAD_SHADER,
		TEXT_SHADER,
		NUM_SHADERS
	};

	// Inherits from System
	CLASS_SYSTEM(GraphicsSystem)
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

		// Texture Properties
		std::vector<GLuint>& get_texture_slots() { return textureSlots; }
		void set_texture_slots(GLuint _index, GLuint _value) { textureSlots[_index] = _value; }

		GLuint const get_texture_slot_index() { return textureSlotIndex; }
		void const set_texture_slot_index(GLuint _index) { textureSlotIndex = _index; }
		
		GLuint& get_white_texture() { return whiteTexture; }
		GLuint& GetProjection() { return uProjection; }

		// Data Members
		GLSLShader* const get_shader_program() { return shaderProgram; }

		//const bool& is_loaded() const { return loaded; }
		
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
		void SetupShaderProgram(const std::string& _vtx_shdr, const std::string& _frg_shdr);

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
		GLuint uProjection = 0;

		/* Shaders **********************************************************************/
		GLSLShader shaderProgram[NUM_SHADERS]; // Shader program to use

		/* Stored Information ***********************************************************/
		//bool loaded = false;

#pragma endregion DataMembers
	};

}

#endif GRAPHICS_SYSTEM_H
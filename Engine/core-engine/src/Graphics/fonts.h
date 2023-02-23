/*!***************************************************************************************
\file			fonts.h
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			26/10/2022

\brief
	This file holds the declaration of the fonts.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef FONTS_H
#define FONTS_H

#include <map>
#include <unordered_map>
#include <string>
#include <glm/glm.hpp>

typedef unsigned int GLuint;
typedef float GLfloat;

namespace Copium
{
	struct Character
	{
		unsigned int textureID; // The texture id to bind the glyph texture
		glm::ivec2 size; // Size of glyph
		glm::ivec2 bearing; // Offset from the baseline to left/top of gylph
		unsigned int advance; // Offset to advance to next glyph
	};

	// Bean: Temporary forward declaration of BaseCamera class to render text
	class BaseCamera;

	class Font
	{
	public:
		Font() = delete;

		/***************************************************************************/
		/*!
		\brief
			Setup the vertex array object of the fonts
		*/
		/***************************************************************************/
		void setup_font_vao();

		/***************************************************************************/
		/*!
		\brief
			Draws text onto the screen
		\param _text
			The text to load onto the screen
		\param _position
			The position of the text
		\param _color
			The color of the text
		\param _scale
			The size of the text
		\param _fontID
			The font to use
		*/
		/***************************************************************************/
		void draw_text(const std::string& _text, const glm::vec3& _position, const glm::vec4& _color, GLfloat _scale, const float& _wrapper, BaseCamera* _camera);

		/***************************************************************************/
		/*!
		\brief
			Exits the font system
		\param _text
			The content to draw
		\param _scale
			The scale of the text
		\param _wrapper
			The max length that the text can draw to
		\return
			A vector2 containing the x and y values of the dimension of the text
		*/
		/***************************************************************************/
		glm::vec2 getDimensions(const std::string& _text, GLfloat _scale, const float& _wrapper);

		/***************************************************************************/
		/*!
		\brief
			Exits the font system
		*/
		/***************************************************************************/
		void shutdown();

		const std::map<char, Character>& get_characters() const { return characters; }

		/***************************************************************************/
		/*!
		\brief
			Gets the current font
		\param _text
			The text to load onto the screen
		\return
			The font
		*/
		/***************************************************************************/
		static Font* getFont(const std::string& _name);
		static std::unordered_map<std::string, Font*>& GetFonts() { return mapNameFonts; }

		void SetName(const std::string& _name) { name = _name; }
		const std::string& GetName() { return name; }

		const GLuint& get_VAO() { return fontVertexArrayID; }
		const GLuint& get_VBO() { return fontVertexBufferID; }

		/***************************************************************************/
		/*!
		\brief
			Deletes the fonts in the engine
		*/
		/***************************************************************************/
		static void cleanUp();

	private:
		Font(const std::string& name);
		std::string name;
		static std::unordered_map<std::string, Font*> mapNameFonts;
		std::map<char, Character> characters;
		GLuint fontVertexArrayID = 0; // Handle to Font Vertex Array Object
		GLuint fontVertexBufferID = 0; // Handle to Font Vertex Buffer Object
	};
}
#endif // !FONTS_H
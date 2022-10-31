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
		unsigned int advance; // Offseet to advance to next glyph
	};

	class Font
	{
	public:
		Font() = delete;

		void setup_font_vao();

		void draw_text(std::string _text, const glm::vec3& _position, const glm::vec4& _color, GLfloat _scale, GLuint _fontID);

		void shutdown();

		const std::map<char, Character>& get_characters() const { return characters; }

		static Font* getFont(const std::string& name);

		static void cleanUp();
	private:
		Font(const std::string& name);
		static std::unordered_map<std::string, Font*> mapNameFonts;
		std::map<char, Character> characters;
		GLuint fontVertexArrayID = 0; // Handle to Font Vertex Array Object
		GLuint fontVertexBufferID = 0; // Handle to Font Vertex Buffer Object
	};
}
#endif // !FONTS_H
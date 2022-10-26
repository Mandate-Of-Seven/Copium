/*!***************************************************************************************
\file			fonts.cpp
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			26/10/2022

\brief
	This file holds the definition of the fonts.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"

#include <GL/glew.h>

#include "Graphics/fonts.h"
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Graphics/graphics-system.h"

namespace Copium::Graphics
{
	void Font::load_font(std::string _font)
	{
		FT_Library ft;
		COPIUM_ASSERT(FT_Init_FreeType(&ft), "Could not initialize FreeType Library");

		FT_Face face;
		COPIUM_ASSERT(FT_New_Face(ft, _font.c_str(), 0, &face), "Fail to load font");
		
		FT_Set_Pixel_Sizes(face, 0, 32);

		// Disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		for (unsigned char c = 0; c < 128; c++)
		{
			// Load character glyph
			COPIUM_ASSERT(FT_Load_Char(face, c, FT_LOAD_RENDER), "Fail to load glyph");

			// Generate textures
			GLuint textureID;
			glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, 
				face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

			// Texture options
			glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			Character character = {
				textureID,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<GLuint>(face->glyph->advance.x)
			};
			characters.insert(std::pair<char, Character>(c, character));
		}
		glBindTexture(GL_TEXTURE_2D, 0);

		FT_Done_Face(face);
		FT_Done_FreeType(ft);

		size_t pos = _font.find_last_of('/');
		size_t lastPos = _font.find_last_of('.');
		size_t size = lastPos - pos;
		std::string str = _font.substr(pos + 1, size - 1);
		PRINT("\tFont " << str << " loaded...");
	}

	void Font::setup_font_vao()
	{
		// Vertex Array Object
		glCreateVertexArrays(1, &fontVertexArrayID);

		// Quad Buffer Object
		glCreateBuffers(1, &fontVertexBufferID);
		glNamedBufferStorage(fontVertexArrayID, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_STORAGE_BIT);

		glEnableVertexArrayAttrib(fontVertexArrayID, 0);
		glVertexArrayAttribFormat(fontVertexArrayID, 0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float));
		glVertexArrayAttribBinding(fontVertexArrayID, 0, 0);
	}

	void Font::draw_text(std::string _text, const glm::vec3& _position, const glm::vec4& _color, GLfloat _scale, GLuint _fontID)
	{
		Graphics::GraphicsSystem* graphics = Graphics::GraphicsSystem::Instance();
		graphics->get_shader_program()[2].Use();
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(fontVertexArrayID);

		float x = _position.x;
		float y = _position.y;

		std::string::const_iterator c;
		for (c = _text.begin(); c != _text.end(); c++)
		{
			Character ch = characters[*c];

			float xpos = x + ch.bearing.x * _scale;
			float ypos = y - (ch.size.y - ch.bearing.y) * _scale;

			float w = ch.size.x * _scale;
			float h = ch.size.y * _scale;

			// Update VBO for each character
			float vertices[6][4] = {
				{xpos		, ypos + h	, 0.f, 0.f},
				{xpos		, ypos		, 0.f, 1.f},
				{xpos + w	, ypos		, 1.f, 1.f},

				{xpos		, ypos + h	, 0.f, 0.f},
				{xpos + w	, ypos		, 1.f, 1.f},
				{xpos + w	, ypos + h	, 1.f, 0.f}
			};

			// Render glyph texture over quad
			glBindTexture(GL_TEXTURE_2D, ch.textureID);

			// Update content of VBO memory
			// glBindBuffer(GL_ARRAY_BUFFER, fontVertexBufferID);
			glNamedBufferSubData(fontVertexBufferID, 0, sizeof(vertices), vertices);

			glDrawArrays(GL_TRIANGLES, 0, 6);

			x += (ch.advance >> 6) * _scale; // Bitshift by 6 to get value in pixels

		}

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		graphics->get_shader_program()[2].UnUse();
	}

	void Font::shutdown()
	{
		glDeleteVertexArrays(1, &fontVertexArrayID);
		glDeleteBuffers(1, &fontVertexBufferID);
	}
}
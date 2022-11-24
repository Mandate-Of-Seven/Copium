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
#include "Editor/editor-system.h"
#include "Files/file-system.h"
#include <glm/gtc/type_ptr.hpp>

namespace Copium
{
	std::unordered_map<std::string, Font*> Font::mapNameFonts;

	Font::Font(const std::string& name)
	{
		std::string path = Paths::assetPath + "/Fonts/" + name + ".ttf";
		FT_Library ft;
		COPIUM_ASSERT(FT_Init_FreeType(&ft), "Could not initialize FreeType Library");

		FT_Face face;
		COPIUM_ASSERT(FT_New_Face(ft, path.c_str(), 0, &face), "Fail to load font");

		FT_Set_Pixel_Sizes(face, 0, 256);

		// Disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		int i = 0;
		for (unsigned char c = 0; c < 128; c++, i++)
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

#if defined(DEBUG) | defined(_DEBUG)
		PRINT("Font " << name << " loaded...");
#endif
	}

	Font* Font::getFont(const std::string& name)
	{
		auto pairNameFont = mapNameFonts.find(name);
		if (pairNameFont != mapNameFonts.end())
			return pairNameFont->second;
		Font* pFont = new Font(name);
		pFont->setup_font_vao();
		mapNameFonts.emplace(std::make_pair(name, pFont));
		return pFont;
	}


	void Font::cleanUp()
	{
		for (auto& pairNameFont : mapNameFonts)
		{
			Font* pFont = pairNameFont.second;
			glDeleteVertexArrays(1, &pFont->fontVertexArrayID);
			glDeleteBuffers(1, &pFont->fontVertexBufferID);
			delete pFont;
		}
	}

	void Font::setup_font_vao()
	{
		// Vertex Array Object
		glCreateVertexArrays(1, &fontVertexArrayID);

		// Font Buffer Object
		glCreateBuffers(1, &fontVertexBufferID);
		glNamedBufferStorage(fontVertexBufferID, 6 * sizeof(TextVertex), nullptr, GL_DYNAMIC_STORAGE_BIT);

		glEnableVertexArrayAttrib(fontVertexArrayID, 0);
		glVertexArrayAttribFormat(fontVertexArrayID, 0, 3, GL_FLOAT, GL_FALSE, offsetof(TextVertex, pos));
		glVertexArrayAttribBinding(fontVertexArrayID, 0, 2);

		glEnableVertexArrayAttrib(fontVertexArrayID, 1);
		glVertexArrayAttribFormat(fontVertexArrayID, 1, 4, GL_FLOAT, GL_FALSE, offsetof(TextVertex, color));
		glVertexArrayAttribBinding(fontVertexArrayID, 1, 2);

		glEnableVertexArrayAttrib(fontVertexArrayID, 2);
		glVertexArrayAttribFormat(fontVertexArrayID, 2, 2, GL_FLOAT, GL_FALSE, offsetof(TextVertex, textCoord));
		glVertexArrayAttribBinding(fontVertexArrayID, 2, 2);

		/*glEnableVertexArrayAttrib(fontVertexArrayID, 3);
		glVertexArrayAttribFormat(fontVertexArrayID, 3, 1, GL_FLOAT, GL_FALSE, offsetof(TextVertex, fontID));
		glVertexArrayAttribBinding(fontVertexArrayID, 3, 2);*/
	}

	void Font::draw_text(const std::string& _text, const glm::vec3& _position, const glm::vec4& _color, GLfloat _scale, GLuint _fontID, BaseCamera* _camera)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		GraphicsSystem* graphics = GraphicsSystem::Instance();
		graphics->get_shader_program()[TEXT_SHADER].Use();

		GLuint uProjection = glGetUniformLocation(
			graphics->get_shader_program()[TEXT_SHADER].GetHandle(), "uViewProjection");

		glm::mat4 projection = _camera->get_view_proj_matrix();
		glUniformMatrix4fv(uProjection, 1, GL_FALSE, glm::value_ptr(projection));

		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(fontVertexArrayID);

		float x = _position.x;
		float y = _position.y;
		float z = _position.z;

		glm::vec2 fontTextCoord[6] = {
			glm::vec2(0.f, 0.f),
			glm::vec2(0.f, 1.f),
			glm::vec2(1.f, 1.f),
			glm::vec2(0.f, 0.f),
			glm::vec2(1.f, 1.f),
			glm::vec2(1.f, 0.f)
		};

		/*float w = 0.f, h = 0.f, xpos = 0.f, ypos = 0.f;
		glm::vec3 fontVertexPosition[6] = {
			glm::vec3(xpos, ypos + h, 0.f),
			glm::vec3(xpos, ypos, 0.f),
			glm::vec3(xpos + w, ypos, 0.f),
			glm::vec3(xpos, ypos + h, 0.f),
			glm::vec3(xpos + w, ypos, 0.f),
			glm::vec3(xpos + w, ypos + h, 0.f)
		};*/
		for (char c : _text)
		{
			Character ch = characters[c];

			float xpos = x + ch.bearing.x * (_scale * 0.01f);
			float ypos = y - (ch.size.y - ch.bearing.y) * (_scale * 0.01f);

			float w = ch.size.x * (_scale * 0.01f);
			float h = ch.size.y * (_scale * 0.01f);

			// Update VBO for each character
			TextVertex vertices[6];
			vertices[0].pos = glm::vec3(xpos, ypos + h, z);
			vertices[1].pos = glm::vec3(xpos, ypos, z);
			vertices[2].pos = glm::vec3(xpos + w, ypos, z);
			vertices[3].pos = glm::vec3(xpos, ypos + h, z);
			vertices[4].pos = glm::vec3(xpos + w, ypos, z);
			vertices[5].pos = glm::vec3(xpos + w, ypos + h, z);

			for (int i = 0; i < 6; i++)
			{
				vertices[i].color = _color;
				vertices[i].textCoord = fontTextCoord[i];
			}

			// Render glyph texture over quad
			glBindTexture(GL_TEXTURE_2D, ch.textureID);

			// Update content of VBO memory
			glNamedBufferSubData(fontVertexBufferID, 0, sizeof(vertices), vertices);
			glVertexArrayVertexBuffer(fontVertexArrayID, 2, fontVertexBufferID, 0, sizeof(TextVertex));

			glDrawArrays(GL_TRIANGLES, 0, 6);

			x += (ch.advance >> 6) * (_scale * 0.01f); // Bitshift by 6 to get value in pixels
		}
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		graphics->get_shader_program()[TEXT_SHADER].UnUse();

		glDisable(GL_BLEND);
	}


	glm::vec2 Font::getDimensions(const std::string& _text, GLfloat _scale)
	{
		float x = 0;
		float y = 0;

		for (char c : _text)
		{
			Character& ch = characters[c];
			float scaledY = ch.size.y * (_scale * 0.01f);
			if (scaledY > y)
				y = scaledY;
			x += (ch.advance >> 6) * (_scale * 0.01f); // Bitshift by 6 to get value in pixels
		}
		return {x,y};
	}
}
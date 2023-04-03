/*!***************************************************************************************
\file			fonts.cpp
\project
\author			Sean Ngo

\par			Course: GAM250
\par			Section:
\date			26/10/2022

\brief
	This file holds the definition of the fonts.

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"

#include <GL/glew.h>

#include "Graphics/fonts.h"
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Graphics/graphics-system.h"
#include "Files/file-system.h"
#include <glm/gtc/type_ptr.hpp>

namespace Copium
{
	std::unordered_map<std::string, Font*> Font::mapNameFonts;

	Font::Font(const std::string& _name, bool _hasPath)
	{
		std::string path;

		if (_hasPath)
		{
			path = _name;
			size_t pos = _name.find_last_of('\\');
			name = _name.substr(pos + 1, _name.length() - pos - 5);
		}
		else
		{
			path = Paths::assetPath + "/Fonts/" + _name + ".ttf";
			name = _name;
		}

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
		PRINT("FONT " << _name << " LOADED...");
	}

	Font* Font::getFont(const std::string& _name, bool _hasPath)
	{
		if (!_hasPath)
		{
			auto pairNameFont = mapNameFonts.find(_name);
			if (pairNameFont != mapNameFonts.end())
				return pairNameFont->second;
		}
		
		Font* pFont = new Font(_name, _hasPath);
		mapNameFonts.emplace(std::make_pair(pFont->name, pFont));
		return pFont;
	}

	void Font::cleanUp()
	{
		for (auto& pairNameFont : mapNameFonts)
		{
			Font* pFont = pairNameFont.second;
			delete pFont;
		}
	}

	glm::vec2 Font::getDimensions(const std::string& _text, GLfloat _scale, const float& _wrapper)
	{
		float x = 0.f, y = 0.f, maxX = 0.f, maxY = 0.f, scaledY = 0.f;
		int newLine = 0;
		for (char c : _text)
		{
			if (c == '\n')
			{
				newLine++;
				x = 0.f;
				continue;
			}
			else if (c == ' ' && x > _wrapper && _wrapper != 0.f)
			{
				newLine++;
				x = 0.f;
				continue;
			}

			Character& ch = characters[c];
			scaledY = ch.size.y * (_scale * 0.01f) - newLine * (_scale * 2.5f);
			if (scaledY > y)
				y = scaledY;
			x += (ch.advance >> 6) * (_scale * 0.01f); // Bitshift by 6 to get value in pixels

			maxX = std::max(maxX, x);
			maxY = std::max(maxY, ch.size.y * (_scale * 0.01f));
		}

		maxY *= newLine + 1;
		
		if(newLine)
			maxY = -maxY;
		
		return { maxX,maxY };
	}
}
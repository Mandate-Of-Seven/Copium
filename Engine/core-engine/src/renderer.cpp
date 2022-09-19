/* DEPRECIATED */

#include "pch.h"
#include <graphics.h>
#include <renderer.h>

namespace Copium::Graphics
{
	void Renderer::init()
	{
		quadBuffer = new Vertex[maxVertexCount];

		// Vertex Array Object
		glCreateVertexArrays(1, &vertexArrayID);
		glBindVertexArray(vertexArrayID);

		// Vertex Buffer Object		
		glCreateBuffers(1, &vertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
		glNamedBufferData(vertexBufferID, maxVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

		glEnableVertexArrayAttrib(vertexArrayID, 0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *) offsetof(Vertex, pos));

		glEnableVertexArrayAttrib(vertexArrayID, 1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *) offsetof(Vertex, color));

		glEnableVertexArrayAttrib(vertexArrayID, 2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *) offsetof(Vertex, textCoord));

		glEnableVertexArrayAttrib(vertexArrayID, 3);
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *) offsetof(Vertex, texID));

		// Element Buffer Object
		GLushort indices[maxIndexCount];
		GLuint offset = 0;
		for (GLuint i = 0; i < maxIndexCount; i += 6)
		{
			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;
			indices[i + 3] = 2 + offset;
			indices[i + 4] = 3 + offset;
			indices[i + 5] = 0 + offset;

			offset += 4;
		}

		glCreateBuffers(1, &indexBufferID);
		glNamedBufferStorage(indexBufferID, sizeof(indices), indices, GL_DYNAMIC_STORAGE_BIT);

		glVertexArrayElementBuffer(vertexArrayID, indexBufferID);
		glBindVertexArray(0);

		glCreateTextures(GL_TEXTURE_2D, 1, &graphics.whiteTexture);
		glBindTexture(GL_TEXTURE_2D, graphics.whiteTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		GLuint color = 0xffffffff;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);

		graphics.textureSlots.resize(32);
		graphics.textureSlots[0] = graphics.whiteTexture;

		for (GLuint i = 1; i < maxTextures; i++)
		{
			graphics.textureSlots[i] = 0;
		}
	}

	void Renderer::shutdown()
	{
		glDeleteVertexArrays(1, &vertexArrayID);
		glDeleteBuffers(1, &vertexBufferID);
		glDeleteBuffers(1, &indexBufferID);
		glDeleteTextures(1, &graphics.whiteTexture);

		delete[] quadBuffer;
	}

	void Renderer::begin_batch()
	{
		quadBufferPtr = quadBuffer;
	}

	void Renderer::flush()
	{
		for (GLuint i = 0; i < graphics.textureSlotIndex; i++)
			glBindTextureUnit(i, graphics.textureSlots[i]);

		glBindVertexArray(vertexArrayID);
		glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, NULL);

		drawCount++;

		indexCount = 0;
		graphics.textureSlotIndex = 1;
	}

	void Renderer::end_batch()
	{
		GLsizeiptr size = (GLuint*)quadBufferPtr - (GLuint*)quadBuffer;
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
		glNamedBufferSubData(vertexBufferID, 0, size * 4, quadBuffer);
	}

	void Renderer::draw_quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		if (indexCount >= maxIndexCount)
		{
			end_batch();
			flush();
			begin_batch();
		}

		GLfloat textureIndex = 0.f;

		quadBufferPtr->pos = { position.x, position.y, 0.0f };
		quadBufferPtr->color = color;
		quadBufferPtr->textCoord = { 0.f, 0.f };
		quadBufferPtr->texID = textureIndex;
		quadBufferPtr++;

		quadBufferPtr->pos = { position.x + size.x, position.y, 0.0f };
		quadBufferPtr->color = color;
		quadBufferPtr->textCoord = { 1.f, 0.f };
		quadBufferPtr->texID = textureIndex;
		quadBufferPtr++;

		quadBufferPtr->pos = { position.x + size.x, position.y + size.y, 0.0f };
		quadBufferPtr->color = color;
		quadBufferPtr->textCoord = { 1.f, 1.f };
		quadBufferPtr->texID = textureIndex;
		quadBufferPtr++;

		quadBufferPtr->pos = { position.x, position.y + size.y, 0.0f };
		quadBufferPtr->color = color;
		quadBufferPtr->textCoord = { 0.f, 1.f };
		quadBufferPtr->texID = textureIndex;
		quadBufferPtr++;

		indexCount += 6;
		quadCount++;
	}

	void Renderer::draw_quad(const glm::vec2& position, const glm::vec2& size, int textureID)
	{
		if (indexCount >= maxIndexCount || graphics.textureSlotIndex > maxTextures - 1)
		{
			end_batch();
			flush();
			begin_batch();
		}

		glm::vec4 color = { 1.f, 1.f, 1.f, 1.f };

		GLfloat textureIndex = 0.f;

		for (GLuint i = 1; i < graphics.textureSlotIndex; i++)
		{
			if (graphics.textureSlots[i] == textureID)
			{
				textureIndex = (GLfloat)i;
				break;
			}
		}

		if (textureIndex == 0.f)
		{
			textureIndex = (GLfloat) graphics.textureSlotIndex;
			graphics.textureSlots[graphics.textureSlotIndex] = textureID;
			graphics.textureSlotIndex++;
		}

		quadBufferPtr->pos = { position.x, position.y, 0.0f };
		quadBufferPtr->color = color;
		quadBufferPtr->textCoord = { 0.f, 0.f };
		quadBufferPtr->texID = textureIndex;
		quadBufferPtr++;

		quadBufferPtr->pos = { position.x + size.x, position.y, 0.0f };
		quadBufferPtr->color = color;
		quadBufferPtr->textCoord = { 1.f, 0.f };
		quadBufferPtr->texID = textureIndex;
		quadBufferPtr++;

		quadBufferPtr->pos = { position.x + size.x, position.y + size.y, 0.0f };
		quadBufferPtr->color = color;
		quadBufferPtr->textCoord = { 1.f, 1.f };
		quadBufferPtr->texID = textureIndex;
		quadBufferPtr++;

		quadBufferPtr->pos = { position.x, position.y + size.y, 0.0f };
		quadBufferPtr->color = color;
		quadBufferPtr->textCoord = { 0.f, 1.f };
		quadBufferPtr->texID = textureIndex;
		quadBufferPtr++;

		indexCount += 6;
		quadCount++;
	}
}

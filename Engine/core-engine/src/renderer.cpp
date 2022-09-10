#include <renderer.h>

namespace Copium::Graphics
{
	void Renderer::init()
	{
		s_Data.quadBuffer = new Vertex[maxVertexCount];

		glCreateVertexArrays(1, &s_Data.vertexArrayID);
		glBindVertexArray(s_Data.vertexArrayID);

		glCreateBuffers(1, &s_Data.vertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, s_Data.vertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, maxVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

		glEnableVertexArrayAttrib(s_Data.vertexArrayID, 0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, pos));

		glEnableVertexArrayAttrib(s_Data.vertexArrayID, 1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, color));

		glEnableVertexArrayAttrib(s_Data.vertexArrayID, 2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, textCoord));

		glEnableVertexArrayAttrib(s_Data.vertexArrayID, 3);
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, texID));

		/*glNamedBufferStorage(s_Data.vertexBufferID, maxVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_STORAGE_BIT);

		glEnableVertexArrayAttrib(s_Data.vertexArrayID, 0);
		glVertexArrayVertexBuffer(s_Data.vertexArrayID, 0, s_Data.vertexBufferID, sizeof(Vertex), offsetof(Vertex, pos));
		glVertexArrayAttribFormat(s_Data.vertexArrayID, 0, 2, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(s_Data.vertexArrayID, 0, 0);

		glEnableVertexArrayAttrib(s_Data.vertexArrayID, 1);
		glVertexArrayVertexBuffer(s_Data.vertexArrayID, 1, s_Data.vertexBufferID, sizeof(Vertex), offsetof(Vertex, color));
		glVertexArrayAttribFormat(s_Data.vertexArrayID, 1, 4, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(s_Data.vertexArrayID, 1, 1);

		glEnableVertexArrayAttrib(s_Data.vertexArrayID, 2);
		glVertexArrayVertexBuffer(s_Data.vertexArrayID, 2, s_Data.vertexBufferID, sizeof(Vertex), offsetof(Vertex, textCoord));
		glVertexArrayAttribFormat(s_Data.vertexArrayID, 2, 3, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(s_Data.vertexArrayID, 2, 2);

		glEnableVertexArrayAttrib(s_Data.vertexArrayID, 3);
		glVertexArrayVertexBuffer(s_Data.vertexArrayID, 3, s_Data.vertexBufferID, sizeof(Vertex), offsetof(Vertex, texID));
		glVertexArrayAttribFormat(s_Data.vertexArrayID, 3, 1, GL_FLOAT, GL_FALSE, 0);
		glVertexArrayAttribBinding(s_Data.vertexArrayID, 3, 3);*/

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

		glCreateBuffers(1, &s_Data.indexBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data.indexBufferID);
		glNamedBufferStorage(s_Data.indexBufferID, sizeof(indices), indices, GL_DYNAMIC_STORAGE_BIT);

		glCreateTextures(GL_TEXTURE_2D, 1, &s_Data.whiteTexture);
		glBindTexture(GL_TEXTURE_2D, s_Data.whiteTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		GLuint color = 0xffffffff;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);

		s_Data.textureSlots[0] = s_Data.whiteTexture;

		for (GLuint i = 1; i < maxTextures; i++)
		{
			s_Data.textureSlots[i] = 0;
		}

	}

	void Renderer::shutdown()
	{
		glDeleteVertexArrays(1, &s_Data.vertexArrayID);
		glDeleteBuffers(1, &s_Data.vertexBufferID);
		glDeleteBuffers(1, &s_Data.indexBufferID);
		glDeleteTextures(1, &s_Data.whiteTexture);

		delete[] s_Data.quadBuffer;
	}

	void Renderer::begin_batch()
	{
		s_Data.quadBufferPtr = s_Data.quadBuffer;
	}

	void Renderer::flush()
	{
		for (GLuint i = 0; i < s_Data.textureSlotIndex; i++)
			glBindTextureUnit(i, s_Data.textureSlots[i]);

		glBindVertexArray(s_Data.vertexArrayID);
		glDrawElements(GL_TRIANGLES, s_Data.indexCount, GL_UNSIGNED_SHORT, NULL);

		s_Data.drawCount++;

		s_Data.indexCount = 0;
		s_Data.textureSlotIndex = 1;
	}

	void Renderer::end_batch()
	{
		GLsizeiptr size = (GLuint*)s_Data.quadBufferPtr - (GLuint*)s_Data.quadBuffer;
		glBindBuffer(GL_ARRAY_BUFFER, s_Data.vertexBufferID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_Data.quadBuffer);
	}

	void Renderer::draw_quad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color)
	{
		if (s_Data.indexCount >= maxIndexCount)
		{
			end_batch();
			flush();
			begin_batch();
		}

		GLfloat textureIndex = 0.f;

		s_Data.quadBufferPtr->pos = { position.x, position.y, 0.0f };
		s_Data.quadBufferPtr->color = color;
		s_Data.quadBufferPtr->textCoord = { 0.f, 0.f };
		s_Data.quadBufferPtr->texID = textureIndex;
		s_Data.quadBufferPtr++;

		s_Data.quadBufferPtr->pos = { position.x + size.x, position.y, 0.0f };
		s_Data.quadBufferPtr->color = color;
		s_Data.quadBufferPtr->textCoord = { 1.f, 0.f };
		s_Data.quadBufferPtr->texID = textureIndex;
		s_Data.quadBufferPtr++;

		s_Data.quadBufferPtr->pos = { position.x + size.x, position.y + size.y, 0.0f };
		s_Data.quadBufferPtr->color = color;
		s_Data.quadBufferPtr->textCoord = { 1.f, 1.f };
		s_Data.quadBufferPtr->texID = textureIndex;
		s_Data.quadBufferPtr++;

		s_Data.quadBufferPtr->pos = { position.x, position.y + size.y, 0.0f };
		s_Data.quadBufferPtr->color = color;
		s_Data.quadBufferPtr->textCoord = { 0.f, 1.f };
		s_Data.quadBufferPtr->texID = textureIndex;
		s_Data.quadBufferPtr++;

		s_Data.indexCount += 6;
		s_Data.quadCount++;
	}

	void Renderer::draw_quad(const glm::vec2& position, const glm::vec2& size, int textureID)
	{
		if (s_Data.indexCount >= maxIndexCount || s_Data.textureSlotIndex > maxTextures - 1)
		{
			end_batch();
			flush();
			begin_batch();
		}

		glm::vec4 color = { 1.f, 1.f, 1.f, 1.f };

		GLfloat textureIndex = 0.f;

		for (GLuint i = 1; i < s_Data.textureSlotIndex; i++)
		{
			if (s_Data.textureSlots[i] == textureID)
			{
				textureIndex = (GLfloat)i;
				break;
			}
		}

		if (textureIndex == 0.f)
		{
			textureIndex = (GLfloat)s_Data.textureSlotIndex;
			s_Data.textureSlots[s_Data.textureSlotIndex] = textureID;
			s_Data.textureSlotIndex++;
		}

		s_Data.quadBufferPtr->pos = { position.x, position.y, 0.0f };
		s_Data.quadBufferPtr->color = color;
		s_Data.quadBufferPtr->textCoord = { 0.f, 0.f };
		s_Data.quadBufferPtr->texID = textureIndex;
		s_Data.quadBufferPtr++;

		s_Data.quadBufferPtr->pos = { position.x + size.x, position.y, 0.0f };
		s_Data.quadBufferPtr->color = color;
		s_Data.quadBufferPtr->textCoord = { 1.f, 0.f };
		s_Data.quadBufferPtr->texID = textureIndex;
		s_Data.quadBufferPtr++;

		s_Data.quadBufferPtr->pos = { position.x + size.x, position.y + size.y, 0.0f };
		s_Data.quadBufferPtr->color = color;
		s_Data.quadBufferPtr->textCoord = { 1.f, 1.f };
		s_Data.quadBufferPtr->texID = textureIndex;
		s_Data.quadBufferPtr++;

		s_Data.quadBufferPtr->pos = { position.x, position.y + size.y, 0.0f };
		s_Data.quadBufferPtr->color = color;
		s_Data.quadBufferPtr->textCoord = { 0.f, 1.f };
		s_Data.quadBufferPtr->texID = textureIndex;
		s_Data.quadBufferPtr++;

		s_Data.indexCount += 6;
		s_Data.quadCount++;
	}
}

#include <renderer.h>

Renderer::Renderer()
{
	quadBuffer = new Vertex[maxVertexCount];

	glCreateVertexArrays(1, &vertexArrayID);

	glCreateBuffers(1, &vertexBufferID);
	glNamedBufferData(vertexBufferID, maxVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexArrayAttrib(vertexArrayID, 0);
	glVertexArrayVertexBuffer(vertexArrayID, 0, vertexBufferID, sizeof(Vertex), offsetof(Vertex, pos));
	glVertexArrayAttribFormat(vertexArrayID, 0, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vertexArrayID, 0, 0);

	glEnableVertexArrayAttrib(vertexArrayID, 1);
	glVertexArrayVertexBuffer(vertexArrayID, 1, vertexBufferID, sizeof(Vertex), offsetof(Vertex, color));
	glVertexArrayAttribFormat(vertexArrayID, 1, 4, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vertexArrayID, 1, 1);

	glEnableVertexArrayAttrib(vertexArrayID, 2);
	glVertexArrayVertexBuffer(vertexArrayID, 2, vertexBufferID, sizeof(Vertex), offsetof(Vertex, textCoord));
	glVertexArrayAttribFormat(vertexArrayID, 2, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vertexArrayID, 2, 2);

	glEnableVertexArrayAttrib(vertexArrayID, 3);
	glVertexArrayVertexBuffer(vertexArrayID, 3, vertexBufferID, sizeof(Vertex), offsetof(Vertex, texID));
	glVertexArrayAttribFormat(vertexArrayID, 3, 1, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vertexArrayID, 3, 3);

	GLuint indices[maxIndexCount];
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
	glNamedBufferData(indexBufferID, sizeof(indices), indices, GL_STATIC_DRAW);

	glCreateTextures(GL_TEXTURE_2D, 1, &whiteTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	GLuint color = 0xffffffff;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color);

	textureSlots[0] = whiteTexture;

	for (GLuint i = 1; i < maxTextures; i++)
	{
		textureSlots[i] = 0;
	}

}

Renderer::~Renderer()
{
	glDeleteVertexArrays(1, &vertexArrayID);
	glDeleteBuffers(1, &vertexBufferID);
	glDeleteBuffers(1, &indexBufferID);
	glDeleteTextures(1, &whiteTexture);

	delete[] quadBuffer;
}

void Renderer::begin_batch()
{

}

void Renderer::flush()
{

}

void Renderer::end_batch()
{

}

void Renderer::draw_quad(const glm::vec2 & position, const glm::vec2 & size, const glm::vec4 & color)
{

}

void Renderer::draw_quad(const glm::vec2 & position, const glm::vec2 & size, int textureID)
{

}

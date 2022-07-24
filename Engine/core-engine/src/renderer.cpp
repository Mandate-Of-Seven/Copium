#include <renderer.h>

// A Vertex contains information about a vertex that is used for rendering
struct Vertex
{
	glm::vec3 pos;		// Position of vertex
	glm::vec4 color;	// Color of vertex
	glm::vec2 textCoord;// Texture coordinate of vertex
	float texID;		// Texture index
};

Renderer::Renderer()
{

}

Renderer::~Renderer()
{

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

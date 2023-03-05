/*!***************************************************************************************
\file			vertex-types.h
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			07/09/2022

\brief
	This file holds the vertex types used in the Graphics system.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef VERTEX_TYPES_H
#define VERTEX_TYPES_H

#include <glm/glm.hpp>

namespace Copium
{
	enum ENTITY_TYPE : int
	{
		QUAD,
		TEXT,
		NUM_TYPES
	};

	// A Vertex contains information about a vertex that is used for rendering
	struct QuadVertex
	{
		glm::vec3 pos;		// Position of vertex
		glm::vec4 color;	// Color of vertex
		glm::vec2 textCoord;// Texture coordinate of vertex
		float texID;		// Texture index
		float type;			// Type of the entity
	};

	struct LineVertex
	{
		glm::vec3 pos;
		glm::vec4 color;
	};

	struct CircleVertex
	{
		glm::vec3 pos;
		glm::vec4 color;
	};

}
#endif // !VERTEX_TYPES_H

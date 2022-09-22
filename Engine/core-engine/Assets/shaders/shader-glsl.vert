/*!***************************************************************************************
\file			shader-glsl.vert
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			31/07/2022

\brief
	This file contains the vertex shader of the application.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#version 450 core

layout (location=0) in vec3 aVertexPosition;
layout (location=1) in vec4 aVertexColor;
layout (location=2) in vec2 aTextureCoordinate;
layout (location=3) in float aTextureIndex;
layout (location=4) in vec3 aVertexPosition2;
layout (location=5) in vec4 aVertexColor2;

layout (location=0) out vec4 vColor;
layout (location=1) out vec2 vTextureCoordinate;
layout (location=2) out float vTextureIndex;

uniform int uIndex;

void main()
{
	vColor				= (uIndex == 0) ? aVertexColor : aVertexColor2;
	vTextureCoordinate	= aTextureCoordinate;
	vTextureIndex		= aTextureIndex;
	gl_Position			= vec4((uIndex == 0) ? aVertexPosition : aVertexPosition2, 1.0);
}
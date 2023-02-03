/*!***************************************************************************************
\file			text-shader-glsl.vert
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			26/10/2022

\brief
	This file contains the vertex shader of the application.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#version 450 core

layout (location=0) in vec3 aVertexPosition;
layout (location=1) in vec4 aVertexColor;
layout (location=2) in vec2 aTextureCoordinate;
layout (location=3) in float aFontIndex;

layout (location=0) out vec4 vColor;
layout (location=1) out vec2 vTextureCoordinate;
layout (location=2) out float vFontIndex;

uniform mat4 uViewProjection;

void main()
{
	vColor				= aVertexColor;
	vTextureCoordinate	= aTextureCoordinate;
	vFontIndex			= aFontIndex;
	gl_Position			= uViewProjection * vec4(aVertexPosition, 1.0);
}
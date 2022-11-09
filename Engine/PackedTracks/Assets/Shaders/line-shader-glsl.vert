/*!***************************************************************************************
\file			line-shader-glsl.vert
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			31/07/2022

\brief
	This file contains the line vertex shader of the application.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#version 450 core

layout (location=0) in vec3 aVertexPosition;
layout (location=1) in vec4 aVertexColor;

layout (location=0) out vec4 vColor;

uniform mat4 uViewProjection;

void main()
{
	vColor				= aVertexColor;
	gl_Position			= uViewProjection * vec4(aVertexPosition, 1.0);
	//gl_Position			= vec4(aVertexPosition, 1.0);
}
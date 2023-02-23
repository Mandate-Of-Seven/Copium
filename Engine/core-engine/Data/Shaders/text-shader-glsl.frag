/*!***************************************************************************************
\file			text-shader-glsl.frag
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			26/10/2022

\brief
	This file contains the fragment shader of the application.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#version 450 core

layout (location=0) in vec4 vInterpColor;
layout (location=1) in vec2 vTextureCoordinate;
layout (location=2) in float vFontIndex;

layout (location=0) out vec4 fFragColor;

uniform sampler2D uFont[32];

void main()
{
	int index = int(vFontIndex);
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(uFont[index], vTextureCoordinate).r);
	fFragColor = sampled * vInterpColor;
}
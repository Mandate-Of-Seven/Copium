/*!***************************************************************************************
\file			shader-glsl.frag
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			31/07/2022

\brief
	This file contains the fragment shader of the application.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#version 450 core

layout (location=0) in vec4 vInterpColor;
layout (location=1) in vec2 vTextureCoordinate;
layout (location=2) in float vTextureIndex;

layout (location=0) out vec4 fFragColor;

uniform sampler2D uTextures[32];

void main()
{
	int index = int(vTextureIndex);
	fFragColor = texture(uTextures[index], vTextureCoordinate) * vInterpColor;
}
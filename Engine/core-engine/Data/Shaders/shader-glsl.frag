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

uniform sampler2D uTexture1;
uniform sampler2D uTexture2;
uniform sampler2D uTexture3;
uniform sampler2D uTexture4;
uniform sampler2D uTexture5;
uniform sampler2D uTexture6;
uniform sampler2D uTexture7;
uniform sampler2D uTexture8;

void main()
{
	int index = int(vTextureIndex);
	switch (index)
	{
		case 1:
		fFragColor = texture(uTexture1, vTextureCoordinate) * vInterpColor;
		if(fFragColor.a < 0.5)
		discard;
		break;

		case 2:
		fFragColor = texture(uTexture2, vTextureCoordinate) * vInterpColor;
		if(fFragColor.a < 0.5)
		discard;
		break;

		case 3:
		fFragColor = texture(uTexture3, vTextureCoordinate) * vInterpColor;
		if(fFragColor.a < 0.5)
		discard;
		break;

		case 4:
		fFragColor = texture(uTexture4, vTextureCoordinate) * vInterpColor;
		if(fFragColor.a < 0.5)
		discard;
		break;

		case 5:
		fFragColor = texture(uTexture5, vTextureCoordinate) * vInterpColor;
		if(fFragColor.a < 0.5)
		discard;
		break;

		case 6:
		fFragColor = texture(uTexture6, vTextureCoordinate) * vInterpColor;
		if(fFragColor.a < 0.5)
		discard;
		break;

		case 7:
		fFragColor = texture(uTexture7, vTextureCoordinate) * vInterpColor;
		if(fFragColor.a < 0.5)
		discard;
		break;

		case 8:
		fFragColor = texture(uTexture8, vTextureCoordinate) * vInterpColor;
		if(fFragColor.a < 0.5)
		discard;
		break;

		default:
		fFragColor = vInterpColor;
		break;
	
	}

}
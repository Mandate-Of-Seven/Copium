/*!***************************************************************************************
\file			shader-glsl.frag
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			31/07/2022

\brief
	This file contains the fragment shader of the application.

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#version 450 core

layout (location=0) in vec4 vInterpColor;
layout (location=1) in vec2 vTextureCoordinate;
layout (location=2) in float vTextureIndex;
layout (location=3) in float vEntityType;

layout (location=0) out vec4 fFragColor;

uniform sampler2D uTexture1[32];
//uniform sampler2D uTexture2;
//uniform sampler2D uTexture3;
//uniform sampler2D uTexture4;
//uniform sampler2D uTexture5;
//uniform sampler2D uTexture6;
//uniform sampler2D uTexture7;
//uniform sampler2D uTexture8;
//uniform sampler2D uTexture9;
//uniform sampler2D uTexture10;
//uniform sampler2D uTexture11;
//uniform sampler2D uTexture12;
//uniform sampler2D uTexture13;
//uniform sampler2D uTexture14;
//uniform sampler2D uTexture15;
//uniform sampler2D uTexture16;
//uniform sampler2D uTexture17;
//uniform sampler2D uTexture18;
//uniform sampler2D uTexture19;
//uniform sampler2D uTexture20;
//uniform sampler2D uTexture21;
//uniform sampler2D uTexture22;
//uniform sampler2D uTexture23;
//uniform sampler2D uTexture24;
//uniform sampler2D uTexture25;
//uniform sampler2D uTexture26;
//uniform sampler2D uTexture27;
//uniform sampler2D uTexture28;
//uniform sampler2D uTexture29;
//uniform sampler2D uTexture30;
//uniform sampler2D uTexture31;
//uniform sampler2D uTexture32;

void main()
{
	int index = int(vTextureIndex);
	int type = int(vEntityType);

	if(type == 0)
	{
		fFragColor = texture(uTexture1[index], vTextureCoordinate) * vInterpColor;
	}
	else if(type == 1)
	{
		vec4 sampled = vec4(1.0, 1.0, 1.0, texture(uTexture1[index], vTextureCoordinate).r);
		fFragColor = sampled * vInterpColor;
	}
//	switch (index)
//	{
//		case 1:
//		fFragColor = texture(uTexture1, vTextureCoordinate) * vInterpColor;
//		if(fFragColor.a < 0.5)
//		discard;
//		break;
//
//		case 2:
//		fFragColor = texture(uTexture2, vTextureCoordinate) * vInterpColor;
//		if(fFragColor.a < 0.5)
//		discard;
//		break;
//
//		case 3:
//		fFragColor = texture(uTexture3, vTextureCoordinate) * vInterpColor;
//		if(fFragColor.a < 0.5)
//		discard;
//		break;
//
//		case 4:
//		fFragColor = texture(uTexture4, vTextureCoordinate) * vInterpColor;
//		if(fFragColor.a < 0.5)
//		discard;
//		break;
//
//		case 5:
//		fFragColor = texture(uTexture5, vTextureCoordinate) * vInterpColor;
//		if(fFragColor.a < 0.5)
//		discard;
//		break;
//
//		case 6:
//		fFragColor = texture(uTexture6, vTextureCoordinate) * vInterpColor;
//		if(fFragColor.a < 0.5)
//		discard;
//		break;
//
//		case 7:
//		fFragColor = texture(uTexture7, vTextureCoordinate) * vInterpColor;
//		if(fFragColor.a < 0.5)
//		discard;
//		break;
//
//		case 8:
//		fFragColor = texture(uTexture8, vTextureCoordinate) * vInterpColor;
//		if(fFragColor.a < 0.5)
//		discard;
//		break;
//
//		case 9:
//		fFragColor = texture(uTexture9, vTextureCoordinate) * vInterpColor;
//		if(fFragColor.a < 0.5)
//		discard;
//		break;
//
//		case 10:
//		fFragColor = texture(uTexture10, vTextureCoordinate) * vInterpColor;
//		if(fFragColor.a < 0.5)
//		discard;
//		break;
//
//		case 11:
//		fFragColor = texture(uTexture11, vTextureCoordinate) * vInterpColor;
//		if(fFragColor.a < 0.5)
//		discard;
//		break;
//
//		case 12:
//		fFragColor = texture(uTexture12, vTextureCoordinate) * vInterpColor;
//		if(fFragColor.a < 0.5)
//		discard;
//		break;
//
//		case 13:
//		fFragColor = texture(uTexture13, vTextureCoordinate) * vInterpColor;
//		if(fFragColor.a < 0.5)
//		discard;
//		break;
//
//		case 14:
//		fFragColor = texture(uTexture14, vTextureCoordinate) * vInterpColor;
//		if(fFragColor.a < 0.5)
//		discard;
//		break;
//
//		case 15:
//		fFragColor = texture(uTexture15, vTextureCoordinate) * vInterpColor;
//		if(fFragColor.a < 0.5)
//		discard;
//		break;
//
//		case 16:
//		fFragColor = texture(uTexture16, vTextureCoordinate) * vInterpColor;
//		if(fFragColor.a < 0.5)
//		discard;
//		break;
//
//		case 17:
//		fFragColor = texture(uTexture17, vTextureCoordinate) * vInterpColor;
//		if(fFragColor.a < 0.5)
//		discard;
//		break;
//
//		case 18:
//		fFragColor = texture(uTexture18, vTextureCoordinate) * vInterpColor;
//		if(fFragColor.a < 0.5)
//		discard;
//		break;
//
//		case 19:
//		fFragColor = texture(uTexture19, vTextureCoordinate) * vInterpColor;
//		if(fFragColor.a < 0.5)
//		discard;
//		break;
//
//		case 20:
//		fFragColor = texture(uTexture20, vTextureCoordinate) * vInterpColor;
//		if(fFragColor.a < 0.5)
//		discard;
//		break;
//
//		case 21:
//		fFragColor = texture(uTexture21, vTextureCoordinate) * vInterpColor;
//		if(fFragColor.a < 0.5)
//		discard;
//		break;
//
//		case 22:
//		fFragColor = texture(uTexture22, vTextureCoordinate) * vInterpColor;
//		if(fFragColor.a < 0.5)
//		discard;
//		break;
//
//		case 23:
//		fFragColor = texture(uTexture23, vTextureCoordinate) * vInterpColor;
//		if(fFragColor.a < 0.5)
//		discard;
//		break;
//
//		case 24:
//		fFragColor = texture(uTexture24, vTextureCoordinate) * vInterpColor;
//		if(fFragColor.a < 0.5)
//		discard;
//		break;
//				
//		case 25:
//		fFragColor = texture(uTexture25, vTextureCoordinate) * vInterpColor;
//		if(fFragColor.a < 0.5)
//		discard;
//		break;
//
//		case 26:
//		fFragColor = texture(uTexture26, vTextureCoordinate) * vInterpColor;
//		if(fFragColor.a < 0.5)
//		discard;
//		break;
//
//		case 27:
//		fFragColor = texture(uTexture27, vTextureCoordinate) * vInterpColor;
//		if(fFragColor.a < 0.5)
//		discard;
//		break;
//
//		case 28:
//		fFragColor = texture(uTexture28, vTextureCoordinate) * vInterpColor;
//		if(fFragColor.a < 0.5)
//		discard;
//		break;
//
//		case 29:
//		fFragColor = texture(uTexture29, vTextureCoordinate) * vInterpColor;
//		if(fFragColor.a < 0.5)
//		discard;
//		break;
//
//		case 30:
//		fFragColor = texture(uTexture30, vTextureCoordinate) * vInterpColor;
//		if(fFragColor.a < 0.5)
//		discard;
//		break;
//
//		case 31:
//		fFragColor = texture(uTexture31, vTextureCoordinate) * vInterpColor;
//		if(fFragColor.a < 0.5)
//		discard;
//		break;
//
//		case 32:
//		fFragColor = texture(uTexture32, vTextureCoordinate) * vInterpColor;
//		if(fFragColor.a < 0.5)
//		discard;
//		break;
//
//		default:
//		fFragColor = vInterpColor;
//		break;
//	
//	}
//
}
/*!***************************************************************************************
\file			line-shader-glsl.frag
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			31/07/2022

\brief
	This file contains the line fragment shader of the application.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#version 450 core

layout (location=0) in vec4 vInterpColor;

layout (location=0) out vec4 fFragColor;

void main()
{
	fFragColor = vInterpColor;
}
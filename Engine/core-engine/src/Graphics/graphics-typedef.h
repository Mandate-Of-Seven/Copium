/*!***************************************************************************************
\file			graphics-typedef.h
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			21/11/2022

\brief
	This file holds the type definitions of the enumerations for graphics systems

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef GRAPHICS_TYPEDEF_H
#define GRAPHICS_TYPEDEF_H

namespace Copium
{
	enum CameraType
	{
		NONE,		// Camera not defined
		GAME,		// In game camera / play mode
		SCENEVIEW,	// The editor camera
		PREVIEW		// The preview camera in the editor
	};

	enum DRAW
	{
		EDITOR,
		WORLD,
		DEBUG,
		DEVELOPMENT,
		NUM_DRAW
	};
}

#endif // !GRAPHICS_TYPEDEF_H

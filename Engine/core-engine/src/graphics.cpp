/*!***************************************************************************************
\file			graphics.cpp
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			07/09/2022

\brief
	This file holds the definition of the Graphics class. The Graphic system handles
	various sub-systems which includes asset loading, matrice calculations and rendering.
	Components and objects which require rendering would refer to this class.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <GLFW/glfw3.h>
#include <string>

namespace Copium::Graphics
{
	class Graphics // Inherits from System
	{
	public:
		void init();

		void update();

		void exit();

	private:

		// Create a vertex buffer for the sprites
		void init_geometry();

		// Load assets into the game
		void load_assets();

		// Load a texture into the game
		void load_texture(const std::string& filename);

		// Setup default world, view and projection matrices (May include orthographic)
		void setup_matrices();

		// Draw the debug data
		void draw_debug_info();

		// Draw the world
		void draw_world();
	};
}

#endif GRAPHICS_H
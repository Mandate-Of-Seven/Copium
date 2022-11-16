/*!***************************************************************************************
\file			graphics-draw.h
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			16/11/2022

\brief
	This file holds the declarations of functions which emcompasses the drawing aspect 
	of the graphic's system.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef GRAPHICS_DRAW_H
#define GRAPHICS_DRAW_H

namespace Copium
{
	enum DRAW
	{
		EDITOR,
		WORLD,
		DEBUG,
		DEVELOPMENT,
		NUM_DRAW
	};

	class Draw
	{
	public:
		void init();

		void update();

		void exit();

		/***************************************************************************/
		/*!
		\brief
			Draw the "world" of the engine which is the scene view also when the 
			player plays the game
		*/
		/***************************************************************************/
		void world();

		/***************************************************************************/
		/*!
		\brief
			Draw the editor mode of the engine where the grids, gizmos and 
			information of the gameobjects are shown
		*/
		/***************************************************************************/
		void editor();

		/***************************************************************************/
		/*!
		\brief
			Draw the debug mode of the engine which includes the physics forces and 
			show collision when collided
		*/
		/***************************************************************************/
		void debug();

		/***************************************************************************/
		/*!
		\brief
			Draw the development mode of the engine, includes the testing features
		*/
		/***************************************************************************/
		void development();

		void enable(DRAW _draw) { drawMode[_draw] = true; }
		void disable(DRAW _draw){ drawMode[_draw] = false; }

	private:
		bool drawMode[NUM_DRAW]{false};
	};
}
#endif // !GRAPHICS_DRAW_H

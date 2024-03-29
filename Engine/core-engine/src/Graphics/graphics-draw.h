/*!***************************************************************************************
\file			graphics-draw.h
\project
\author			Sean Ngo

\par			Course: GAM250
\par			Section:
\date			16/11/2022

\brief
	This file holds the declarations of functions which emcompasses the drawing aspect 
	of the graphic's system.

All content � 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef GRAPHICS_DRAW_H
#define GRAPHICS_DRAW_H

#include "Graphics/graphics-typedef.h"
#include "Graphics/renderer.h"

namespace Copium
{
	class Transform;

	class Draw
	{
	public:
		/***************************************************************************/
		/*!
		\brief
			Initializes the systems of the graphics render pipeline
		\param _camera
			The camera to draw to
		*/
		/***************************************************************************/
		void init(BaseCamera* _camera);

		/***************************************************************************/
		/*!
		\brief
			Update the various draw of the engine
		\param _type
			The camera type to check to render certain parts of the view
		*/
		/***************************************************************************/
		void update(CameraType _type);

		/***************************************************************************/
		/*!
		\brief
			Exits the graphics drawing system along with its sub system - renderer
		*/
		/***************************************************************************/
		void exit();

		/***************************************************************************/
		/*!
		\brief
			Resets the batch renderer and all textures loaded into the gpu
		*/
		/***************************************************************************/
		void ResetRenderer();

		/***************************************************************************/
		/*!
		\brief
			Draw the editor mode of the engine where the grids, gizmos and 
			information of the gameobjects are shown
		*/
		/***************************************************************************/
		void editor(int _index = 0);

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

		void UpdateTransform(const Transform& _transform, glm::vec3& _position, float& _rotation, glm::vec3& _scale);

		void enable(DRAW _draw) { drawMode[_draw] = true; }
		void disable(DRAW _draw){ drawMode[_draw] = false; }

		const bool& get_draw_mode(DRAW _draw) const { return drawMode[_draw]; }

		Renderer* get_renderer() { return &renderer; }

	private:
		bool drawMode[NUM_DRAW]{false};

		Renderer renderer;
		BaseCamera* camera;
	};
}
#endif // !GRAPHICS_DRAW_H

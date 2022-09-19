/*!***************************************************************************************
\file			editor-layer.h
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			16/09/2022

\brief
	This file holds the declaration of functions for editor-layer.cpp.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef EDITOR_LAYER_H
#define EDITOR_LAYER_H

namespace Copium::Editor
{
	class Editor
	{
	public:
		void init();

		void update();

		void draw();

		void exit();

	private:
	};

	static Editor editor;

}
#endif // !EDITOR_LAYER_H

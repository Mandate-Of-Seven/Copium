/*!***************************************************************************************
\file			editor-content-browser.h
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			01/11/2022

\brief
	Contains function declarations for the editor content browser

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/

#ifndef EDITOR_CONTENT_BROWSER_H
#define EDITOR_CONTENT_BROWSER_H

#include <string>
#include "Files/file-system.h"
#include "Graphics/textures.h"

namespace Copium
{
	class EditorContentBrowser
	{
	public:
		void init();

		void update();

		void exit();

	private:
		std::filesystem::path currentDirectory;

		std::vector<Texture> icons;
	};
}

#endif // !EDITOR_CONTENT_BROWSER_H


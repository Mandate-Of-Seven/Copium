/*!***************************************************************************************
\file			assets-system.h
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			20/10/2022

\brief
	Contains function declarations for Assets system

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/

#ifndef ASSETS_SYSTEM_H
#define ASSETS_SYSTEM_H

#include "CopiumCore/system-interface.h"
#include "Graphics/textures.h"
#include "Graphics/spritesheet.h"

namespace Copium
{
	CLASS_SYSTEM(AssetsSystem)
	{
	public:
		void init();

		void update();

		void exit();

		void load_all_textures(std::list<std::string>& _path);
		void load_all_audio(std::list<std::string>& _path);
		void load_all_shaders(std::list<std::string>& _path);

		std::vector<Texture>& get_textures() { return textures; }
		std::vector<Spritesheet>& get_spritesheets() { return spritesheets; }

	private:
		std::vector<Texture> textures;
		std::vector<Spritesheet> spritesheets;
	};
}

#endif // !ASSETS_SYSTEM_H

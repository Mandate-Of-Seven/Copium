/*!***************************************************************************************
\file			assets-system.h
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			20/10/2022

\brief
	Contains function declarations for Assets system

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/

#ifndef ASSETS_SYSTEM_H
#define ASSETS_SYSTEM_H

#include "CopiumCore/system-interface.h"

namespace Copium::Assets
{
	CLASS_SYSTEM(AssetsSystem)
	{
	public:
		void init();

		void update();

		void exit();

	private:

	};
}

#endif // !ASSETS_SYSTEM_H

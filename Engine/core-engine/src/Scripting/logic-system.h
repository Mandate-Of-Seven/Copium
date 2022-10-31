﻿/*!***************************************************************************************
\file			
\project		
\author			
\co-authors		// DELETE IF NOT APPLICABLE

\par			Course: GAM200
\par			Section: 
\date			25/10/2022

\brief
	// BRIEF DESCRIPTION OF FILE PURPOSE

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/

#pragma once

#ifndef LOGIC_SYSTEM_H

#define LOGIC_SYSTEM_H

//INCLUDES
#include "CopiumCore/system-interface.h"

//USING

namespace Copium		
{
	class UIButtonComponent;

	CLASS_SYSTEM(LogicSystem)
	{
	public:
		void init();
		void update();
		void exit();
	};
}



#endif // !LOGIC_SYSTEM_H

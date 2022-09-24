/*!***************************************************************************************
\file			physics.cpp
\project
\author			Abdul Hadi

\par			Course: GAM200
\par			Section:
\date			23/09/2022

\brief
	This file contains the function declarations for a physics system. It will perform
	physics and collision on a collider component of an object.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "system-interface.h"
#include "math-library.h"
#include "collision.h"
#include "collider.h"


namespace Copium::Physics
{
	class Physics
	{
	public:
		void init();
		void update();
		void exit();
		
	private:
	/***************************************************************************/
	/*!
    \brief
	   Updates position of object based on dt
    \param dt
	   The delta time to take
   */
   /**************************************************************************/
		void update_pos(float dt);
		
	public:
		std::vector<Copium::Component::Collider*> box;  //store box component to be able to perform physics
	};
}
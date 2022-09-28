/*!***************************************************************************************
\file			physics-system.h
\project
\author			Abdul Hadi

\par			Course: GAM200
\par			Section:
\date			23/09/2022

\brief
	This file contains the function definitions for a physics system. It will perform
	physics and collision on a collider component of an object.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "CopiumCore/system-interface.h"
#include "Math/math-library.h"
#include "Physics/collider.h"


namespace Copium::Physics
{	
	const Math::Vec2 gravity = { 0.0,-0.50 };			//gravity affecting all objects that can be affected
	CLASS_SYSTEM(Physics)
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
   */
   /**************************************************************************/
		void update_pos();
	/***************************************************************************/
	/*!
	\brief
	   Checks for collision between objects
	*/
	/**************************************************************************/
		void check_collision();
		
	public:
		std::vector<Copium::Component::RigidBody*> boxes;  //store box component to be able to perform physics
	};

}

#include "system-interface.h"
#include "math-library.h"


namespace Copium::Physics
{
	class Physics
	{
	public:
		void init();
		void update();
		void exit();
	private:
		void Update_Pos(float dt);

	public:
		Math::Vec2 position;
		Math::Vec2 velocity;
		Math::Vec2 acceleration;
	};

}
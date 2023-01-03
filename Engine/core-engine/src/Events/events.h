#pragma once

#include <GameObject/game-object.h>

namespace Copium
{

	struct IEvent
	{
	protected:
		virtual ~IEvent() {};
	};

	struct CollisionEvent : public IEvent {
		CollisionEvent(GameObject& a, GameObject& b) : gameObjA{ a }, gameObjB{ b } {}
		GameObject& gameObjA;
		GameObject& gameObjB;
	};

}
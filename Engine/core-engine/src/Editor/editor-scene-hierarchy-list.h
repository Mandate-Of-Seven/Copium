#pragma once

#ifndef SCENE_HIERARCHY_LIST_H
#define SCENE_HIERARCHY_LIST_H
#include <imgui.h>
#include "SceneManager/scene-manager.h"
#include "GameObject/game-object.h"

namespace Window::Hierarchy
{


	void init();

	void update();

	void display_gameobject(const GameObject& _go);

}
#endif

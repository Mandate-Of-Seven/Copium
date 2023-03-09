
#ifndef EDITOR_PERFORMANCE_H
#define EDITOR_PERFORMANCE_H
#pragma once

#include <imgui.h>
#include "Windows/windows-utils.h"

namespace Copium
{
	inline bool isPerformanceOpen;

	class EditorPerformance
	{
	public:
		EditorPerformance() : open{ false }{}
		void init();
		void update();
		void exit();
	private:
		std::string performanceText;
		bool open;
	};
}

#endif

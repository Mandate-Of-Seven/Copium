#include "pch.h"
#include "editor-performance.h"
#include <Messaging/message-system.h>
#include "Events/events-system.h"

namespace Copium
{

	namespace
	{
		Copium::MessageSystem& messageSystem{ *Copium::MessageSystem::Instance() };
	}

	void EditorPerformance::init()
	{
		isPerformanceOpen = true;
		performanceText = "testing123";
	}

	void EditorPerformance::update()
	{
		if (!isPerformanceOpen)
			return;

		if (ImGui::Begin("Performance Viewer"))
		{
			MyEventSystem->publish(new ActivatePerformanceViewerEvent(performanceText));
			ImGui::Text(performanceText.c_str());
			
		}else
			MyEventSystem->publish(new OffPerformanceViewerEvent);
		ImGui::End();
	}

	void EditorPerformance::exit()
	{

	}



}

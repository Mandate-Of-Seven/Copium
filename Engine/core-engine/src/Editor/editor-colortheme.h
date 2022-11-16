#pragma once
#include <imgui.h>
#include <rapidjson/document.h>
#include "Windows/windows-utils.h"

namespace Window
{
	namespace ColorTheme
	{
		struct ImVec3
		{
			float x, y, z;
			ImVec3(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f)
			{
				x = _x; y = _y; z = _z;
			}
		};


		static Window::ColorTheme::ImVec3 color_for_text = Window::ColorTheme::ImVec3(255.f / 255.f, 233.f / 255.f, 0.f / 255.f);
		static Window::ColorTheme::ImVec3 color_for_head = Window::ColorTheme::ImVec3(238.f / 255.f, 5.f / 255.f, 43.f / 255.f);
		static Window::ColorTheme::ImVec3 color_for_area = Window::ColorTheme::ImVec3(238.f / 255.f, 75.f / 255.f, 43.f / 255.f);
		static Window::ColorTheme::ImVec3 color_for_body = Window::ColorTheme::ImVec3(238.f / 255.f, 155.f / 255.f, 43.f / 255.f);
		static Window::ColorTheme::ImVec3 color_for_pops = Window::ColorTheme::ImVec3(238.f / 255.f, 105.f / 255.f, 43.f / 255.f);

		struct ColorPreset
		{
			ImVec4 color_for_text;
			ImVec4 color_for_head;
			ImVec4 color_for_area;
			ImVec4 color_for_body;
			ImVec4 color_for_pops;
		};

		

		void init();
		void update();

		void setTheme(ImVec3 color_for_text, ImVec3 color_for_head, ImVec3 color_for_area, ImVec3 color_for_body, ImVec3 color_for_pops);
		void serialize(const std::string& _filepath);
		void deserialize(const std::string& _filepath);
	}
}
#pragma once
#include <imgui.h>
#include <rapidjson/document.h>
#include "Windows/windows-utils.h"
#include "Math/math-library.h"

namespace Window
{
	namespace ColorTheme
	{
		inline bool isColorThemeOpen;

		static Copium::Math::Vec3 color_for_text = Copium::Math::Vec3(255.f / 255.f, 233.f / 255.f, 0.f / 255.f);
		static Copium::Math::Vec3 color_for_head = Copium::Math::Vec3(238.f / 255.f, 5.f / 255.f, 43.f / 255.f);
		static Copium::Math::Vec3 color_for_area = Copium::Math::Vec3(238.f / 255.f, 75.f / 255.f, 43.f / 255.f);
		static Copium::Math::Vec3 color_for_body = Copium::Math::Vec3(238.f / 255.f, 155.f / 255.f, 43.f / 255.f);
		static Copium::Math::Vec3 color_for_pops = Copium::Math::Vec3(238.f / 255.f, 105.f / 255.f, 43.f / 255.f);

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

		void setTheme(Copium::Math::Vec3 color_for_text, Copium::Math::Vec3 color_for_head, 
					  Copium::Math::Vec3 color_for_area, Copium::Math::Vec3 color_for_body, 
					  Copium::Math::Vec3 color_for_pops);

		void serialize(const std::string& _filepath);
		void deserialize(const std::string& _filepath);
	}
}
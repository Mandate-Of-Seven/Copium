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

		struct ImVec3
		{
			float x, y, z;
			ImVec3(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f)
			{
				x = _x; y = _y; z = _z;
			}
		};


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

		/*******************************************************************************
		/*!
		*
		\brief
			Serialize the current 5 main colors in the current theme

		\param _filepath
			read-only reference to the .theme file to serialize the data to

		\return
			void

		*/
		/*******************************************************************************/
		void serialize(const std::string& _filepath);
		/*******************************************************************************
		/*!
		*
		\brief
			Deserialize the 5 main colors

		\param _filepath
			read-only reference to the .theme file to deserialize the data from

		\return
			void

		*/
		/*******************************************************************************/
		void deserialize(const std::string& _filepath);
	}
}
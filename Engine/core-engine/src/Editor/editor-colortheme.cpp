#include "pch.h"
#include "editor-colortheme.h"

namespace Window
{
	namespace ColorTheme
	{
		void init()
		{
            /*
                Window::ColorTheme::setTheme(Window::ColorTheme::color_for_text, Window::ColorTheme::color_for_head,
                                             Window::ColorTheme::color_for_area, Window::ColorTheme::color_for_body,
                                             Window::ColorTheme::color_for_pops);
             */

		}

		void update()
		{
            
            ImGui::Begin("Theme generator");
            ImGui::ColorEdit3("Text Color", (float*)&color_for_text, ImGuiColorEditFlags_PickerHueBar);
            ImGui::ColorEdit3("Head Color", (float*)&color_for_head, ImGuiColorEditFlags_PickerHueBar);
            ImGui::ColorEdit3("Area Color", (float*)&color_for_area, ImGuiColorEditFlags_PickerHueBar);
            ImGui::ColorEdit3("Body Color", (float*)&color_for_body, ImGuiColorEditFlags_PickerHueBar);
            ImGui::ColorEdit3("Pop Color", (float*)&color_for_pops, ImGuiColorEditFlags_PickerHueBar);
            if (ImGui::Button("Apply Theme")) 
            {
                printf("Applied Theme");
                setTheme(color_for_text,color_for_head,color_for_area,color_for_body,color_for_pops);
            }
            ImGui::SameLine();
            if (ImGui::Button("Save Theme"))
            {
                //Serialize
                //setTheme(color_for_text, color_for_head, color_for_area, color_for_body, color_for_pops);
            }
            ImGui::SameLine();
            if (ImGui::Button("Load Theme"))
            {
                //DeSerialize
               // setTheme(color_for_text, color_for_head, color_for_area, color_for_body, color_for_pops);
            }
            ImGui::End();
            
		}

		void setTheme(ImVec3 color_for_text, ImVec3 color_for_head, ImVec3 color_for_area, ImVec3 color_for_body, ImVec3 color_for_pops)
		{
            ImGuiStyle& style = ImGui::GetStyle();

            style.Colors[ImGuiCol_Text] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 1.00f);
            style.Colors[ImGuiCol_TextDisabled] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 0.58f);
            style.Colors[ImGuiCol_WindowBg] = ImVec4(color_for_body.x, color_for_body.y, color_for_body.z, 0.95f);
            style.Colors[ImGuiCol_ChildBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 0.58f);
            style.Colors[ImGuiCol_Border] = ImVec4(color_for_body.x, color_for_body.y, color_for_body.z, 0.00f);
            style.Colors[ImGuiCol_BorderShadow] = ImVec4(color_for_body.x, color_for_body.y, color_for_body.z, 0.00f);
            style.Colors[ImGuiCol_FrameBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 1.00f);
            style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.78f);
            style.Colors[ImGuiCol_FrameBgActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
            style.Colors[ImGuiCol_TitleBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 1.00f);
            style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 0.75f);
            style.Colors[ImGuiCol_TitleBgActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
            style.Colors[ImGuiCol_MenuBarBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 0.47f);
            style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 1.00f);
            style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.21f);
            style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.78f);
            style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
            style.Colors[ImGuiCol_CheckMark] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.80f);
            style.Colors[ImGuiCol_SliderGrab] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.50f);
            style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
            style.Colors[ImGuiCol_Button] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.50f);
            style.Colors[ImGuiCol_ButtonHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.86f);
            style.Colors[ImGuiCol_ButtonActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
            style.Colors[ImGuiCol_Header] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.76f);
            style.Colors[ImGuiCol_HeaderHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.86f);
            style.Colors[ImGuiCol_HeaderActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
            style.Colors[ImGuiCol_Tab] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
            style.Colors[ImGuiCol_TabHovered] = ImVec4(color_for_body.x, color_for_body.y, color_for_body.z, 1.00f);            
            style.Colors[ImGuiCol_TabActive] = ImVec4(color_for_body.x, color_for_body.y, color_for_body.z, 1.00f);
            style.Colors[ImGuiCol_TabUnfocused] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
            style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(color_for_body.x, color_for_body.y, color_for_body.z, 1.00f);
            style.Colors[ImGuiCol_ResizeGrip] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.15f);
            style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.78f);
            style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
            style.Colors[ImGuiCol_PlotLines] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 0.63f);
            style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
            style.Colors[ImGuiCol_PlotHistogram] = ImVec4(color_for_text.x, color_for_text.y, color_for_text.z, 0.63f);
            style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 1.00f);
            style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(color_for_head.x, color_for_head.y, color_for_head.z, 0.43f);
            style.Colors[ImGuiCol_PopupBg] = ImVec4(color_for_pops.x, color_for_pops.y, color_for_pops.z, 0.92f);
            style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(color_for_area.x, color_for_area.y, color_for_area.z, 0.73f);
		}
	}
}
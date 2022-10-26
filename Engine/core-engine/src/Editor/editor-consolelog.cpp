/*!***************************************************************************************
\file			editor-consolelog.cpp
\project
\author			Shawn Tanary

\par			Course: GAM200
\par			Section:
\date			20/09/2022

\brief
    This file contins functions that create the logger using IMGUI for the editor.

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"
#include <Editor/editor-consolelog.h>
#include <CopiumCore/copium-core.h>


namespace Window
{
	namespace EditorConsole
	{
		bool isOpen;

		EditorConsoleLog::EditorConsoleLog()
		{
			keepScrolling = true;
			Clear();
		}

		void EditorConsoleLog::Clear()
		{
			Buf.clear();
			LineOffsets.clear();
			LineOffsets.push_back(0);
		}

		void EditorConsoleLog::AddLog(const char* fmt, ...) IM_FMTARGS(2)
		{
			int old_size = Buf.size();
			va_list args;
			va_start(args, fmt);
			Buf.appendfv(fmt, args);
			va_end(args);
            for (int new_size = Buf.size(); old_size < new_size; old_size++)
            {
                if (Buf[old_size] == '\n')
                {
                    LineOffsets.push_back(old_size + 1);
                }
            }
		}

        void add_logEntry(std::string str)
        {
            str += '\n';
            editorLog.AddLog(str.c_str());
        }

		void init()
		{
            isOpen = true;
		}

		void update()
		{

			if (!isOpen)
			{
				return;
			}

            ImGui::SetNextWindowSize(ImVec2(500, 400), ImGuiCond_FirstUseEver);
			if (!ImGui::Begin("Console Log", &isOpen))//return early if minimised
			{

				ImGui::End();
				return;
			}

            ImGui::SetWindowSize("Console Log", ImVec2(850, 400));
            if (ImGui::BeginPopup("Options"))
            {
                ImGui::Checkbox("Continuous Scrolling", &editorLog.keepScrolling);
                ImGui::EndPopup();
            }

            if (ImGui::SmallButton("[Debug] Add 5 Logs"))
            {
                static int counter = 1;
                const char* words[] = { "Graphics", "Console", "Input", "Scripting", "Physics", "Serialization", "Math" };
                for (int n = 0; n < 5; n++)
                {
                    const char* word = words[counter % IM_ARRAYSIZE(words)];
                    editorLog.AddLog("%d) This is just a test, Last crash caused by: ""%s"" system.\n",
                          counter,word);
                    counter++;
                }
            }
            ImGui::SameLine();
            if (ImGui::SmallButton("[Debug] Testing Win32console"))
            {
                CONSOLE_CRITICAL("This is a critical");
                CONSOLE_ERROR("This is an error");
                CONSOLE_WARN("This is a warning");
                CONSOLE_INFO("This is just info");
                CONSOLE_TRACE("Goodbye");
            }

            if (ImGui::Button("Options"))
            {
                ImGui::OpenPopup("Options");
            }
            ImGui::SameLine();
            bool clear = ImGui::Button("Clear");
            ImGui::SameLine();
            bool copy = ImGui::Button("Copy");
            ImGui::SameLine();
            if (ImGui::Button("Performance Viewer"))
            {
                Copium::CopiumCore::Instance()->toggle_display_peformance();
            }

            ImGui::SameLine();
            editorLog.Search.Draw("Search", 200.0f);

            ImGui::Separator();
            ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

            if (clear)
            {
                editorLog.Clear();
            }
                
            if (copy)
            {
                ImGui::LogToClipboard();
            }

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
            const char* buf = editorLog.Buf.begin();
            const char* buf_end = editorLog.Buf.end();
            if (editorLog.Search.IsActive())
            {
                // In this example we don't use the clipper when Filter is enabled.
                // This is because we don't have a random access on the result on our filter.
                // A real application processing logs with ten of thousands of entries may want to store the result of
                // search/filter.. especially if the filtering function is not trivial (e.g. reg-exp).
                for (int line_no = 0; line_no < editorLog.LineOffsets.Size; line_no++)
                {
                    const char* line_start = buf + editorLog.LineOffsets[line_no];
                    const char* line_end = (line_no + 1 < editorLog.LineOffsets.Size) ? (buf + editorLog.LineOffsets[line_no + 1] - 1) : buf_end;
                    if (editorLog.Search.PassFilter(line_start, line_end))
                        ImGui::TextUnformatted(line_start, line_end);
                }
            }
            else
            {
                ImGuiListClipper clipper;
                clipper.Begin(editorLog.LineOffsets.Size);
                while (clipper.Step())
                {
                    for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
                    {
                        const char* line_start = buf + editorLog.LineOffsets[line_no];
                        const char* line_end = (line_no + 1 < editorLog.LineOffsets.Size) ? (buf + editorLog.LineOffsets[line_no + 1] - 1) : buf_end;
                        ImGui::TextUnformatted(line_start, line_end);
                    }
                }
                clipper.End();
            }
            ImGui::PopStyleVar();

            if (editorLog.keepScrolling && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.0f);

            ImGui::EndChild();
            ImGui::End();
		}
	}
}
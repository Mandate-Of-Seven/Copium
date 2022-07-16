#include "inspector.h"

namespace Window
{

	namespace Inspector
	{
        GameObject* selectedGameObject;

        void init()
        {
            selectedGameObject = nullptr;
        }

        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		void update()
		{
            static float f = 0.0f;
            static int counter = 0;
            ImGui::Begin("Inspector");                      // Create a window called "Hello, world!" and append into it.
            if (selectedGameObject)                             // If there is a selectedGameObject
            {
                Transform trans{ selectedGameObject->Trans() };
                Vector2 position = trans.Position();
                Vector2 rotation = trans.Rotation();
                Vector2 scale = trans.Scale();

                //ImGui::LabelText("x:","   ");
                //ImGui::SameLine();
                if (ImGui::CollapsingHeader("Transform"))
                {
                    ImGui::Text("Position");
                    if (ImGui::BeginTable("split", 2))
                    {
                        ImGui::TableNextColumn();
                        ImGui::PushItemWidth(-1);
                        ImGui::Text("X"); ImGui::SameLine();
                        ImGui::InputDouble("x", &position.x);
                        ImGui::PopItemWidth();

                        ImGui::TableNextColumn();
                        ImGui::PushItemWidth(-1);
                        ImGui::Text("Y"); ImGui::SameLine();
                        ImGui::InputDouble("y", &position.y);
                        ImGui::PopItemWidth();
                        ImGui::EndTable();
                    }
                }
                selectedGameObject->Trans({position, rotation, scale});
                //ImGui::Text("This is some useful text.");   // Display some text (you can use a format strings too)
                //ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                //ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color
                //if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                    //counter++;
                //ImGui::SameLine();
                //ImGui::Text("counter = %d", counter);
                //ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }
            ImGui::End();
		}
	}
}
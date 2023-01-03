
#include <pch.h>
#include <GameObject/Components/collider-components.h>
#include <GameObject/game-object.h>

namespace Copium
{
    BoxCollider2D::BoxCollider2D(ComponentID _entityID):Component(_entityID, ComponentType::BoxCollider2D)
    {
        
    }

	AABB BoxCollider2D::getBounds() 
	{
        GameObject& gameObj = *MyNewSceneManager.findGameObjByID(entityID);
		Math::Vec3 size{ gameObj.transform.scale };
		Math::Vec3 pos{ gameObj.transform.position };

        if (gameObj.transform.hasParent())
        {
            size.x *= (*gameObj.transform.parent).scale.x;
            size.y *= (*gameObj.transform.parent).scale.y;
            size.z *= (*gameObj.transform.parent).scale.z;
            pos += (*gameObj.transform.parent).position;
        }

		float x = (boundingBox.max.x - boundingBox.min.x) * size.x;
		float y = (boundingBox.max.y - boundingBox.min.y) * size.y;
		AABB tmp{ boundingBox };
		tmp.max.x *= x;
		tmp.min.x *= x;
		tmp.max.y *= y;
		tmp.min.y *= y;
		tmp.max.x += pos.x;
		tmp.min.x += pos.x;
		tmp.max.y += pos.y;
		tmp.min.y += pos.y;
		return tmp;
	}

	void BoxCollider2D::inspector_view()
	{
        ImGuiWindowFlags windowFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody
            | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_SizingStretchProp;
        if (ImGui::BeginTable("Component transform", 2, windowFlags))
        {
            ImGui::Indent();

            ImGui::TableSetupColumn("Text", 0, 0.4f);
            ImGui::TableSetupColumn("Input", 0, 0.6f);

            ImGui::TableNextRow();
            ImGui::TableNextColumn();
            ImGui::Text("Bounds");
            ImGui::TableNextColumn();
            if (ImGui::BeginTable("Component AABB", 4, windowFlags))
            {
                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::PushID(0);
                ImGui::Text("minX"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
                ImGui::InputFloat("", &boundingBox.min.x);
                ImGui::PopID();

                ImGui::TableNextColumn();
                ImGui::PushID(1);
                ImGui::Text("minY"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
                ImGui::InputFloat("", &boundingBox.min.y);
                ImGui::PopID();

                ImGui::TableNextColumn();
                ImGui::PushID(2);
                ImGui::Text("maxX"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
                ImGui::InputFloat("", &boundingBox.max.x);
                ImGui::PopID();

                ImGui::TableNextColumn();
                ImGui::PushID(3);
                ImGui::Text("maxY"); ImGui::SameLine(); ImGui::SetNextItemWidth(-FLT_MIN);
                ImGui::InputFloat("", &boundingBox.max.y);
                ImGui::PopID();

                ImGui::EndTable();
            }
        }
        ImGui::Unindent();
        ImGui::EndTable();
	}
}
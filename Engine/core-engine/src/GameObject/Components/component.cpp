/*!***************************************************************************************
\file			component.cpp
\project
\author			Zacharie Hong (50%)
\co-author      Matthew Lau (50%)

\par			Course: GAM200
\par			Section:
\date			17/07/2022

\brief
    Defines Component Class to be added to GameObjects and its member functions

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#include "pch.h"
#include <rapidjson/document.h>
#include "GameObject/Components/component.h"
#include "renderer-component.h"
#include "transform-component.h"

namespace Copium
{
    Component::Component(ComponentID _entityID, ComponentType _componentType) 
        : entityID{ _entityID }, componentType{ _componentType }, enabled{ true }, id{0} {}

    void Component::destroy() {}


    void Component::deserialize(rapidjson::Value& _value)
    {
        std::cout <<  "deserializing CID\n";
        if (_value.HasMember("ID"))
        {
            id = _value["ID"].GetUint64();
        }
    }
    void Component::serialize(rapidjson::Value& _value, rapidjson::Document& _doc) 
    {
        _value.AddMember("ID", id, _doc.GetAllocator());

    }


    bool Component::Enabled() const noexcept{ return enabled;}

    void Component::Enabled(bool _enabled) noexcept { enabled = _enabled; }

    const std::string& Component::Name() const
    {
        return MAP_COMPONENT_TYPE_NAME[componentType];
    }

    void Component::inspector_view()
    {
        ImGui::Checkbox("##Active", &enabled);
        ImGui::SameLine();
    }
}
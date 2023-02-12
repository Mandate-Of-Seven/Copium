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

#include "Animation/animation-system.h"

namespace Copium
{
    Component::Component(GameObject& _gameObj, ComponentType _componentType) 
        : gameObj{ _gameObj }, componentType{ _componentType }, enabled{ true }, id{0} 
    {
        //PRINT("Component Created--\n" << "UID: " << uuid << std::endl);
    }

    void Component::destroy() {}


    void Component::deserialize(rapidjson::Value& _value)
    {
        //PRINT("Deserializing Component ID");
        if (_value.HasMember("ID"))
        {
            id = _value["ID"].GetUint64();
        }

        if (_value.HasMember("UID"))
            uuid.Deserialize(_value["UID"]);
        
    }
    void Component::serialize(rapidjson::Value& _value, rapidjson::Document& _doc) 
    {
        _value.AddMember("ID", id, _doc.GetAllocator());

        uuid.Serialize(_value, _doc);

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
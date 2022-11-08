/*!***************************************************************************************
\file			sprite.cpp
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			14/09/2022

\brief
	This file holds the sprite's data which is used to render using the Graphics system
	on to the scene.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"

#include "Graphics/sprite.h"
#include "Files/assets-system.h"

namespace Copium
{
	bool Sprite::deserialize(rapidjson::Value& _value)
	{
		if (!_value.HasMember("Sprite ID"))
			return false;

		spriteID = _value["Sprite ID"].GetUint();

		if(spriteID != 0)
			refTexture = &AssetsSystem::Instance()->get_textures()[spriteID - 1];

		if (!_value.HasMember("Sprite Name"))
			return false;

		sprite_name = _value["Sprite Name"].GetString();

		if (!_value.HasMember("Color"))
			return false;

		rapidjson::Value& _v = _value["Color"].GetObj();
		color.x = _v["X"].GetFloat();
		color.y = _v["Y"].GetFloat();
		color.z = _v["Z"].GetFloat();
		color.w = _v["W"].GetFloat();

		return true;
	}
	bool Sprite::serialize(rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		_value.AddMember("Sprite ID", spriteID, _doc.GetAllocator());

		rapidjson::Value type(rapidjson::kObjectType);
		type.SetString(sprite_name.c_str(), (rapidjson::SizeType)sprite_name.length(), _doc.GetAllocator());
		_value.AddMember("Sprite Name", type, _doc.GetAllocator());

		rapidjson::Value _color(rapidjson::kObjectType);
		_color.AddMember("X",color.x, _doc.GetAllocator());
		_color.AddMember("Y", color.y, _doc.GetAllocator());
		_color.AddMember("Z", color.z, _doc.GetAllocator());
		_color.AddMember("W", color.w, _doc.GetAllocator());
		_value.AddMember("Color", _color, _doc.GetAllocator());


		return true;
	}
}
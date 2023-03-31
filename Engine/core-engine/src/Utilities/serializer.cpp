/*!***************************************************************************************
\file			serializer.cpp
\project
\author			Matthew Lau
\co-author		Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			27/09/2022

\brief
	This file holds the definitions of functions that are involved in de/serialization in Copium Engine.
	Especially for complex types.

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"
#include <Events/events-system.h>
#include "serializer.h"
#include "Files/assets-system.h"
#include "Audio/sound-system.h"
namespace Copium
{	
	// Serialization --------------------------------------------
	template<>
	void Serializer::Serialize<Math::Vec2>(Math::Vec2& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		//PRINT("Serializing a Vector2");
		rapidjson::Value v2(rapidjson::kObjectType);

		Copium::SerializeBasic(_data.x, v2, _doc, "X");
		Copium::SerializeBasic(_data.y, v2, _doc, "Y");

		rapidjson::Value key;
		CreateJsonString(_key, key, _doc);

		_value.AddMember(key, v2, _doc.GetAllocator());
	}
	template<>
	void Serializer::Serialize<Math::Vec3>(Math::Vec3& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		//PRINT("Serializing a Vector3");

		rapidjson::Value v3(rapidjson::kObjectType);

		Copium::SerializeBasic(_data.x, v3, _doc, "X");
		Copium::SerializeBasic(_data.y, v3, _doc, "Y");
		Copium::SerializeBasic(_data.z, v3, _doc, "Z");

		rapidjson::Value key;
		CreateJsonString(_key, key, _doc);

		_value.AddMember(key, v3, _doc.GetAllocator());
	}
	template<>
	void Serializer::Serialize<Math::Vec4>(Math::Vec4& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		//PRINT("Serializing a Vector4");

		rapidjson::Value v4(rapidjson::kObjectType);

		Copium::SerializeBasic(_data.x, v4, _doc, "X");
		Copium::SerializeBasic(_data.y, v4, _doc, "Y");
		Copium::SerializeBasic(_data.z, v4, _doc, "Z");
		Copium::SerializeBasic(_data.a, v4, _doc, "A");

		rapidjson::Value key;
		CreateJsonString(_key, key, _doc);

		_value.AddMember(key, v4, _doc.GetAllocator());

	}

	template<>
	void Serializer::Serialize<Animation>(Animation& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		Copium::SerializeBasic(_data.spriteSheet.name, _value, _doc, "Name");
		Copium::SerializeBasic(_data.spriteSheet.spriteID, _value, _doc, "Sprite ID");
		Copium::SerializeBasic(_data.timeDelay, _value, _doc, "Time Delay");
		Copium::SerializeBasic(_data.frameCount, _value, _doc, "FrameCount");
		Copium::SerializeBasic(_data.spriteSheet.rows, _value, _doc, "Rows");
		Copium::SerializeBasic(_data.spriteSheet.columns, _value, _doc, "Columns");

		rapidjson::Value flip(rapidjson::kObjectType);
		Copium::SerializeBasic(_data.spriteSheet.flip.x, flip, _doc, "X");
		Copium::SerializeBasic(_data.spriteSheet.flip.y, flip, _doc, "Y");
		_value.AddMember("Flip", flip, _doc.GetAllocator());

	}
	template<>
	void Serializer::Serialize<Animator>(Animator& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		Copium::SerializeBasic(_data.loop, _value, _doc, "Loop");
		Copium::SerializeBasic(_data.reverse, _value, _doc, "Rev");
		Copium::SerializeBasic(_data.animations.size(), _value, _doc, "Count");


		rapidjson::Value anims(rapidjson::kArrayType);
		for (Animation& a : _data.get_animation_vector())
		{
			rapidjson::Value anim(rapidjson::kObjectType);
			Serialize(a, "", anim, _doc);
			anims.PushBack(anim, _doc.GetAllocator());
		}
		_value.AddMember("Animations", anims, _doc.GetAllocator());
	}

	template<typename T, typename... Ts>
	struct SerializeComponentsStruct
	{
		constexpr SerializeComponentsStruct(TemplatePack<T, Ts...> pack) {};
		SerializeComponentsStruct(GameObject& _data, rapidjson::Value& _components, rapidjson::Document& _doc)
		{
			SerializeComponent<T,Ts...>(_data, _components,_doc);
		}

		template <typename T1, typename... T1s>
		void SerializeComponent(GameObject& _data, rapidjson::Value& _components, rapidjson::Document& _doc)
		{
			COPIUM_ASSERT(_components.GetType() != rapidjson::kArrayType, "Value should be of kArrayType!");
			for (T1* component : _data.GetComponents<T1>())
			{
				rapidjson::Value comp(rapidjson::kObjectType);
				Copium::SerializeBasic(component->enabled, comp, _doc, "enabled");
				Copium::SerializeBasic(component->uuid.GetUUID(), comp, _doc, "UID");
				Copium::SerializeBasic(GetComponentType<T1>::name, comp, _doc, "Type");
				Copium::SerializeBasic((int)GetComponentType<T1>::e, comp, _doc, "TypeID");

				Serializer::Serialize(*component, "", comp, _doc);
				_components.PushBack(comp, _doc.GetAllocator());
			}
			if constexpr (sizeof...(T1s) != 0)
			{
				return SerializeComponent<T1s...>(_data, _components, _doc);
			}
		}
	};
	using SerializeAllComponents = decltype(SerializeComponentsStruct(ComponentTypes()));
	void SerializeComponents(GameObject& _data, rapidjson::Value& _components, rapidjson::Document& _doc)
	{
		SerializeAllComponents serialize(_data, _components, _doc);
	}

	template<>
	void Serializer::Serialize<GameObject>(GameObject& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		Copium::SerializeBasic(_data.uuid.GetUUID(), _value, _doc, "UID");
		Copium::SerializeBasic(_data.name, _value, _doc, "Name");
		Copium::SerializeBasic(_data.active, _value, _doc, "Active");

		rapidjson::Value comps(rapidjson::kArrayType);
		rapidjson::Value transformComponent(rapidjson::kObjectType);
		Serialize(_data.transform, "", transformComponent, _doc);
		comps.PushBack(transformComponent, _doc.GetAllocator());
		SerializeComponents(_data, comps,_doc);
		//for (auto iter = _data.components.begin(); iter != _data.components.end(); ++iter)
		//{
		//	rapidjson::Value comp(rapidjson::kObjectType);
		//	ComponentType componentType = (*iter)->nameToType((*iter)->Name());
		//	SerializeComponent(componentType, *iter, comp, _doc);
		//	comps.PushBack(comp, _doc.GetAllocator());
		//}

		_value.AddMember("Components", comps, _doc.GetAllocator());

	}
	template<>
	void Serializer::Serialize<Transform>(Transform& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		uint64_t uid{ 0 };
		if (_data.HasParent())
			uid = _data.parent->gameObject.uuid;
		Copium::SerializeBasic(uid, _value, _doc, "PID");

		Copium::SerializeBasic("Transform", _value, _doc, "Type");

		Serialize(_data.position, "Pos", _value, _doc);
		Serialize(_data.rotation, "Rot", _value, _doc);
		Serialize(_data.scale, "Scale", _value, _doc);

	}
	template<>
	void Serializer::Serialize<SpriteRenderer>(SpriteRenderer& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		// Serialize the sprite
		Serialize(_data.sprite, "", _value, _doc);
	}
	template<>
	void Serializer::Serialize<Sprite>(Sprite& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		Copium::SerializeBasic(_data.spriteID, _value, _doc, "Sprite ID");
		Copium::SerializeBasic(_data.sprite_name, _value, _doc, "Sprite Name");

		rapidjson::Value color(rapidjson::kObjectType);
		Copium::SerializeBasic(_data.color.x, color, _doc, "X");
		Copium::SerializeBasic(_data.color.y, color, _doc, "Y");
		Copium::SerializeBasic(_data.color.z, color, _doc, "Z");
		Copium::SerializeBasic(_data.color.w, color, _doc, "W");
		_value.AddMember("Color", color, _doc.GetAllocator());

		rapidjson::Value flip(rapidjson::kObjectType);
		Copium::SerializeBasic(_data.flip.x, flip, _doc, "X");
		Copium::SerializeBasic(_data.flip.y, flip, _doc, "Y");
		_value.AddMember("Flip", flip, _doc.GetAllocator());
	}

	template<>
	void Serializer::Serialize<AudioSource>(AudioSource& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		Copium::SerializeBasic(_data.alias, _value, _doc, "Alias");
		Copium::SerializeBasic(_data.loop, _value, _doc, "Loop");
		Copium::SerializeBasic(_data.overLap, _value, _doc, "Overlap");
		Copium::SerializeBasic(_data.volume, _value, _doc, "Volume");
		Copium::SerializeBasic(_data.channel, _value, _doc, "Channel");
	}

	template<>
	void Serializer::Serialize<Rigidbody2D>(Rigidbody2D& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		Copium::SerializeBasic(_data.useGravity, _value, _doc, "UseGravity");
		Copium::SerializeBasic(_data.isKinematic, _value, _doc, "IsKinematic");
	}
	template<>
	void Serializer::Serialize<AABB>(AABB& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		Serialize(_data.min, "Min", _value, _doc);
		Serialize(_data.max, "Max", _value, _doc);
	}
	template<>
	void Serializer::Serialize<Camera>(Camera& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		rapidjson::Value color(rapidjson::kObjectType);
		Copium::SerializeBasic(_data.get_bg_color().x, color, _doc, "X");
		Copium::SerializeBasic(_data.get_bg_color().y, color, _doc, "Y");
		Copium::SerializeBasic(_data.get_bg_color().z, color, _doc, "Z");
		Copium::SerializeBasic(_data.get_bg_color().w, color, _doc, "W");
		_value.AddMember("Color", color, _doc.GetAllocator());

		Copium::SerializeBasic(_data.IsOrthographic(), _value, _doc, "Projection");

		rapidjson::Value clippingPlanes(rapidjson::kObjectType);
		Copium::SerializeBasic(_data.GetNearClip(), clippingPlanes, _doc, "Near");
		Copium::SerializeBasic(_data.GetFarClip(), clippingPlanes, _doc, "Far");
		_value.AddMember("Clipping Planes", clippingPlanes, _doc.GetAllocator());


	}
	template<>
	void Serializer::Serialize<BoxCollider2D>(BoxCollider2D& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		rapidjson::Value bb(rapidjson::kObjectType);
		Serialize(_data.bounds, "", bb, _doc);
		_value.AddMember("BoundingBox", bb, _doc.GetAllocator());
	}

	template<>
	void Serializer::Serialize<Image>(Image& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		Copium::SerializeBasic((int)_data.hAlignment, _value, _doc, "H_Align");
		Copium::SerializeBasic((int)_data.vAlignment, _value, _doc, "V_Align");

		// Serialize the sprite
		Serialize(_data.sprite, "", _value, _doc);
	}
	template<>
	void Serializer::Serialize<Button>(Button& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		uint64_t tgID{ 0 };
		if (_data.targetGraphic)
			tgID = _data.targetGraphic->uuid;
		Copium::SerializeBasic(tgID, _value, _doc, "Graphic ID");

		rapidjson::Value bb(rapidjson::kObjectType);
		Serialize(_data.bounds, "", bb, _doc);
		_value.AddMember("BoundingBox", bb, _doc.GetAllocator());

		Copium::SerializeBasic(_data.callbackName, _value, _doc, "Callback");
		Copium::SerializeBasic(_data.fadeDuration, _value, _doc, "FadeDuration");

		Copium::SerializeBasic(_data.clickedColor.r, _value, _doc, "clickedR");
		Copium::SerializeBasic(_data.clickedColor.g, _value, _doc, "clickedG");
		Copium::SerializeBasic(_data.clickedColor.b, _value, _doc, "clickedB");
		Copium::SerializeBasic(_data.clickedColor.a, _value, _doc, "clickedA");

		Copium::SerializeBasic(_data.hoverColor.r, _value, _doc, "hoveredR");
		Copium::SerializeBasic(_data.hoverColor.g, _value, _doc, "hoveredG");
		Copium::SerializeBasic(_data.hoverColor.b, _value, _doc, "hoveredB");
		Copium::SerializeBasic(_data.hoverColor.a, _value, _doc, "hoveredA");

		Copium::SerializeBasic(_data.normalColor.r, _value, _doc, "normalR");
		Copium::SerializeBasic(_data.normalColor.g, _value, _doc, "normalG");
		Copium::SerializeBasic(_data.normalColor.b, _value, _doc, "normalB");
		Copium::SerializeBasic(_data.normalColor.a, _value, _doc, "normalA");

	}
	template<>
	void Serializer::Serialize<Text>(Text& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		Copium::SerializeBasic(_data.fontName, _value, _doc, "FontName");
		Copium::SerializeBasic((int)_data.hAlignment, _value, _doc, "H_Align");
		Copium::SerializeBasic((int)_data.vAlignment, _value, _doc, "V_Align");

		Copium::SerializeBasic(_data.content, _value, _doc, "Content");

		Copium::SerializeBasic(_data.fSize, _value, _doc, "FontSize");
		Copium::SerializeBasic(_data.wrapper, _value, _doc, "Wrapper");

		Copium::SerializeBasic(_data.color.r, _value, _doc, "r");
		Copium::SerializeBasic(_data.color.g, _value, _doc, "g");
		Copium::SerializeBasic(_data.color.b, _value, _doc, "b");
		Copium::SerializeBasic(_data.color.a, _value, _doc, "a");


	}

	template<>
	void Serializer::Serialize<Script>(Script& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		Copium::SerializeBasic(_data.name, _value, _doc, "Name");

		// Field Data References
		auto it = _data.fieldDataReferences.begin();
		static Field buffer( FieldType::None,TEXT_BUFFER_SIZE );
		while (it != _data.fieldDataReferences.end())
		{
			const std::string& fieldName{ it->first };
			MyEventSystem->publish(new ScriptGetFieldEvent(_data, fieldName.c_str(), buffer.data));
			switch (it->second.fType)
			{
			case FieldType::Float:
			{
				Copium::SerializeBasic(buffer.Get<float>(), _value, _doc, fieldName);
				break;
			}
			case FieldType::Double:
			{
				Copium::SerializeBasic(buffer.Get<double>(), _value, _doc, fieldName);
				break;
			}
			case FieldType::Bool:
			{
				Copium::SerializeBasic(buffer.Get<bool>(), _value, _doc, fieldName);
				break;
			}
			case FieldType::Char:
			{
				Copium::SerializeBasic(buffer.Get<char>(), _value, _doc, fieldName);
				break;
			}
			case FieldType::Short:
			{
				Copium::SerializeBasic(buffer.Get<short>(), _value, _doc, fieldName);
				break;
			}
			case FieldType::Int:
			{
				Copium::SerializeBasic(buffer.Get<int>(), _value, _doc, fieldName);
				break;
			}			
			case FieldType::Long:
			{
				Copium::SerializeBasic(buffer.Get<int64_t>(), _value, _doc, fieldName);
				break;
			}
			case FieldType::UShort:
			{
				Copium::SerializeBasic(buffer.Get<uint16_t>(), _value, _doc, fieldName);
				break;
			}
			case FieldType::UInt:
			{
				Copium::SerializeBasic(buffer.Get<uint32_t>(), _value, _doc, fieldName);
				break;
			}
			case FieldType::ULong:
			{
				Copium::SerializeBasic(buffer.Get<uint64_t>(), _value, _doc, fieldName);
				break;
			}
			case FieldType::String:
			{
				Copium::SerializeBasic<C_String>((char*&)buffer.data, _value, _doc, fieldName);
				break;
			}
			case FieldType::Vector2:
			{
				Serialize(buffer.Get<Math::Vec2>(), fieldName, _value, _doc);
				break;
			}
			case FieldType::Vector3:
			{
				Serialize(buffer.Get<Math::Vec3>(), fieldName, _value, _doc);
				break;
			}
			}
			
			++it;
		}

		// Game Object References
		auto gameObjsIt = _data.fieldGameObjReferences.begin();
		while (gameObjsIt != _data.fieldGameObjReferences.end())
		{
			const std::string& fieldName{ gameObjsIt->first };
			//If there was a reference assigned
			if (gameObjsIt->second != nullptr)
				Copium::SerializeBasic((*gameObjsIt).second->uuid, _value, _doc, fieldName);
			++gameObjsIt;
		}

		// Field Component References
		auto componentsIt = _data.fieldComponentReferences.begin();
		while (componentsIt != _data.fieldComponentReferences.end())
		{
			const std::string& fieldName{ componentsIt->first };
			//If there was a reference assigned
			if (componentsIt->second != nullptr)
				Copium::SerializeBasic((*componentsIt).second->uuid, _value, _doc, fieldName);
			++componentsIt;
		}

	}

	template<>
	void Serializer::Serialize<SortingGroup>(SortingGroup& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		Copium::SerializeBasic(_data.GetLayerID(), _value, _doc, "SortingLayer");
		Copium::SerializeBasic(_data.GetOrderInLayer(), _value, _doc, "OrderInLayer");

	}

	void Serializer::WriteToFile(rapidjson::StringBuffer& _sb, rapidjson::Document& _doc, std::ofstream& _os)
	{
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(_sb);
		_doc.Accept(writer);
		_os << _sb.GetString();
	}
	//-----------------------------------------------------------


	// Deserialization ------------------------------------------
	
	// Math Types
	template<>
	void Serializer::Deserialize<Math::Vec2>(Math::Vec2& _data, const std::string& _key, rapidjson::Value& _value)
	{
		if (!Copium::Deserialize(_data.x, _value, "X"))
			_data.x = 0.f;

		if (!Copium::Deserialize(_data.y, _value, "Y"))
			_data.y = 0.f;

	}
	template<>
	void Serializer::Deserialize<Math::Vec3>(Math::Vec3& _data, const std::string& _key, rapidjson::Value& _value)
	{
		rapidjson::Value v3(rapidjson::kObjectType);
		if (!_value.HasMember(_key.c_str()))
		{
			if (!Copium::Deserialize(_data.x, _value, "X"))
				_data.x = 0.f;

			if (!Copium::Deserialize(_data.y, _value, "Y"))
				_data.y = 0.f;

			if (!Copium::Deserialize(_data.z, _value, "Z"))
				_data.z = 0.f;
			return;
		}
		v3 = _value[_key.c_str()].GetObj();

		if (!Copium::Deserialize(_data.x, v3, "X"))
			_data.x = 0.f;

		if (!Copium::Deserialize(_data.y, v3, "Y"))
			_data.y = 0.f;

		if (!Copium::Deserialize(_data.z, v3, "Z"))
			_data.z = 0.f;
	}
	template<>
	void Serializer::Deserialize<Math::Vec4>(Math::Vec4& _data, const std::string& _key, rapidjson::Value& _value)
	{

		if (!Copium::Deserialize(_data.x, _value, "X"))
			_data.x = 0.f;

		if (!Copium::Deserialize(_data.y, _value, "Y"))
			_data.y = 0.f;

		if (!Copium::Deserialize(_data.z, _value, "Z"))
			_data.z = 0.f;

		if (!Copium::Deserialize(_data.z, _value, "Z"))
			_data.z = 0.f;
	}

	template<>
	void Serializer::Deserialize<Transform>(Transform& _data, const std::string& _key, rapidjson::Value& _value)
	{
		Copium::Deserialize((uint64_t&)_data.parent, _value, "PID");
		Deserialize(_data.position, "Pos", _value);
		Deserialize(_data.rotation, "Rot", _value);
		Deserialize(_data.scale, "Scale", _value);

	}

	template<>
	void Serializer::Deserialize<Camera>(Camera& _data, const std::string& _key, rapidjson::Value& _value)
	{
		rapidjson::Value& _v = _value["Color"].GetObj();
		glm::vec4 color;
		Copium::Deserialize(color.x, _v, "X");
		Copium::Deserialize(color.y, _v, "Y");
		Copium::Deserialize(color.z, _v, "Z");
		Copium::Deserialize(color.w, _v, "W");
		_data.set_bg_color(color);

		bool ortho;
		Copium::Deserialize(ortho, _value, "Projection");
		_data.SetOrthographic(ortho);

		_v = _value["Clipping Planes"].GetObj();
		float tmp{ 0.f };
		Copium::Deserialize(tmp, _v, "Near");
		_data.SetNearClip(tmp);
		Copium::Deserialize(tmp, _v, "Far");
		_data.SetFarClip(tmp);

	}
	
	template<>
	void Serializer::Deserialize<SortingGroup>(SortingGroup& _data, const std::string& _key, rapidjson::Value& _value)
	{

		int tmp{ 0 };
		Copium::Deserialize(tmp, _value, "SortingLayer");
		_data.sortingLayer = tmp;
		Copium::Deserialize(tmp, _value, "OrderInLayer");
		_data.orderInLayer = tmp;
	}

	template<>
	void Serializer::Deserialize<AudioSource>(AudioSource& _data, const std::string& _key, rapidjson::Value& _value)
	{
		std::string tmp;
		Copium::Deserialize(tmp, _value, "Alias");
		_data.set_alias(tmp);
		Copium::Deserialize(_data.loop, _value, "Loop");
		Copium::Deserialize(_data.overLap, _value,"Overlap");
		Copium::Deserialize(_data.volume, _value, "Volume");
		Copium::Deserialize(_data.channel, _value, "Channel");
		SoundSystem::Instance()->soundList[_data.alias].first->setVolume(_data.volume);
	}


	template<>
	void Serializer::Deserialize<GameObject>(GameObject& _data, const std::string& _key, rapidjson::Value& _value)
	{
		(void)_key;
		std::string name;
		if (!Copium::Deserialize(name, _value, "Name"))
			name = "New GameObject";
		_data.name = name;

		uint64_t uid = _data.uuid;
		if (!Copium::Deserialize(_data.uuid.GetUUID(), _value, "UID"))
			_data.uuid = uid;

		Copium::Deserialize(_data.active, _value, "Active");

		rapidjson::Value components(rapidjson::kArrayType);
		if (_value.HasMember("Components"))
		{
			components = _value["Components"].GetArray();

			for (rapidjson::Value::ValueIterator iter = components.Begin(); iter != components.End(); ++iter)
			{
				rapidjson::Value& component = *iter;

				//int type{ -1 };
				std::string key;

				UUID uuid{};
				Copium::Deserialize(uuid.GetUUID(), component, "UID");

				if (Copium::Deserialize(key, component, "Type"))
				{

					if (key == "Transform")
					{
						Deserialize(_data.transform, "", component);
					}
					else if (key == "SortingGroup")
					{
						
						SortingGroup* tmp = nullptr;
						MyEventSystem->publish(new ComponentAddEvent(_data, tmp, uuid));
						Deserialize(*tmp, "", component);
					}
					else
					{
						//PRINT("Deserializing " << key);
						switch (NAME_TO_CTYPE[key])
						{
							case ComponentType::Animator:
							{
								Animator* tmp = nullptr;
								MyEventSystem->publish(new ComponentAddEvent(_data,tmp, uuid));
								Deserialize(*tmp, "", component);
								break;
							}
							case ComponentType::BoxCollider2D:
							{
								BoxCollider2D* tmp = nullptr;
								MyEventSystem->publish(new ComponentAddEvent(_data, tmp, uuid));
								Deserialize(*tmp, "", component);
								break;
							}
							case ComponentType::Camera:
							{
								Camera* tmp = nullptr;
								MyEventSystem->publish(new ComponentAddEvent(_data, tmp, uuid));
								Deserialize(*tmp, "", component);
								break;
							}
							case ComponentType::Rigidbody2D:
							{
								Rigidbody2D* tmp = nullptr;
								MyEventSystem->publish(new ComponentAddEvent(_data, tmp, uuid));
								Deserialize(*tmp, "", component);
								break;
							}
							case ComponentType::SpriteRenderer:
							{
								SpriteRenderer* tmp = nullptr;
								MyEventSystem->publish(new ComponentAddEvent(_data, tmp, uuid));
								Deserialize(*tmp, "", component);
								break;
							}
							case ComponentType::Script:
							{
								Script* tmp = nullptr;
								std::string scriptName{};
								Copium::Deserialize(scriptName, component, "Name");
								MyEventSystem->publish(new ComponentAddEvent<Script>(_data, tmp, scriptName.c_str(), uuid));
								Deserialize(*tmp, "", component);
								break;
							}
							case ComponentType::Button:
							{
								Button* tmp = nullptr;
								MyEventSystem->publish(new ComponentAddEvent(_data, tmp, uuid));
								Deserialize(*tmp, "", component);
								break;
							}
							case ComponentType::Image:
							{
								Image* tmp = nullptr;
								MyEventSystem->publish(new ComponentAddEvent(_data, tmp, uuid));
								Deserialize(*tmp, "", component);
								break;
							}
							case ComponentType::Text:
							{
								Text* tmp = nullptr;
								MyEventSystem->publish(new ComponentAddEvent<Text>(_data, tmp, false, uuid));
								Deserialize(*tmp, "", component);
								break;
							}
							case ComponentType::AudioSource:
							{
								AudioSource* tmp = nullptr;
								MyEventSystem->publish(new ComponentAddEvent(_data, tmp, uuid));
								Deserialize(*tmp, "", component);
								break;
							}
						default:
							PRINT("ADDED NOTHING, MAYBE ADDED THE COMPONENT TO THE GAMEOBJECT.CPP");
							break;
						}
					}

				}

			}
		}


	}

	template<>
	void Serializer::Deserialize<Animator>(Animator& _data, const std::string& _key, rapidjson::Value& _value)
	{
		Copium::Deserialize(_data.uuid.GetUUID(), _value, "UID");
		Copium::Deserialize(_data.loop, _value, "Loop");
		Copium::Deserialize(_data.enabled, _value, "enabled");
		//if (_data.loop)
		//	PRINT("looping");
		Copium::Deserialize(_data.reverse, _value, "Rev");

		rapidjson::Value anims(rapidjson::kArrayType);
		if (_value.HasMember("Animations"))
		{
			anims = _value["Animations"].GetArray();

			int i{ 0 };
			for (rapidjson::Value::ValueIterator iter = anims.Begin(); iter != anims.End(); ++iter)
			{
				rapidjson::Value& a = *iter;
				_data.AddAnimation();
				
				Animation& anim = _data.get_animation_vector()[i];
				Copium::Deserialize(anim.spriteSheet.name, a, "Name");
				Copium::Deserialize(anim.spriteSheet.spriteID, a, "Sprite ID");
				Copium::Deserialize(anim.timeDelay, a, "Time Delay");
				Copium::Deserialize(anim.frameCount, a, "FrameCount");
				Copium::Deserialize(anim.spriteSheet.rows, a, "Rows");
				Copium::Deserialize(anim.spriteSheet.columns, a, "Columns");

				if (a.HasMember("Flip"))
				{
					rapidjson::Value flip(rapidjson::kObjectType);
					flip = a["Flip"].GetObj();
					Copium::Deserialize(anim.spriteSheet.flip.x, flip, "X");
					Copium::Deserialize(anim.spriteSheet.flip.y, flip, "Y");
				}

				anim.loop = _data.loop;
				++i;
			}

		}

		_data.currentAnimationIndex=0;
		_data.SetStatus(Animator::AnimatorStatus::idle);

	}
	template<>
	void Serializer::Deserialize<Animation>(Animation& _data, const std::string& _key, rapidjson::Value& _value)
	{

	}

	template<>
	void Serializer::Deserialize<AABB>(AABB& _data, const std::string& _key, rapidjson::Value& _value)
	{
		if (!_value.HasMember(_key.c_str()))
			return;

		

		rapidjson::Value bb(rapidjson::kObjectType);
		bb = _value[_key.c_str()].GetObj();

		if (bb.HasMember("Min"))
		{

			rapidjson::Value min(rapidjson::kObjectType);
			min = bb["Min"].GetObj();
			Deserialize(_data.min, "", min);

		}
		if (bb.HasMember("Max"))
		{

			rapidjson::Value max(rapidjson::kObjectType);
			max = bb["Max"].GetObj();
			Deserialize(_data.max, "", max);

		}

	}
	template<>
	void Serializer::Deserialize<BoxCollider2D>(BoxCollider2D& _data, const std::string& _key, rapidjson::Value& _value)
	{
		Copium::Deserialize(_data.enabled, _value, "enabled");
		Copium::Deserialize(_data.uuid.GetUUID(), _value, "UID");
		Deserialize(_data.bounds, "BoundingBox", _value);
	}
	template<>
	void Serializer::Deserialize<Rigidbody2D>(Rigidbody2D& _data, const std::string& _key, rapidjson::Value& _value)
	{
		Copium::Deserialize(_data.enabled, _value, "enabled");
		Copium::Deserialize(_data.uuid.GetUUID(), _value, "UID");
		bool kinematic, grav;
		Copium::Deserialize(grav, _value, "UseGravity");
		Copium::Deserialize(kinematic, _value, "IsKinematic");

		_data.useGravity = grav;
		_data.isKinematic = kinematic;
	}

	template<>
	void Serializer::Deserialize<Sprite>(Sprite& _data, const std::string& _key, rapidjson::Value& _value)
	{
		Copium::Deserialize(_data.spriteID, _value, "Sprite ID");

		Copium::Deserialize(_data.sprite_name, _value, "Sprite Name");

		if (_value.HasMember("Color"))
		{
			rapidjson::Value color(rapidjson::kObjectType);
			color = _value["Color"].GetObj();
			Copium::Deserialize(_data.color.x, color, "X");
			Copium::Deserialize(_data.color.y, color, "Y");
			Copium::Deserialize(_data.color.z, color, "Z");
			Copium::Deserialize(_data.color.w, color, "W");
		}

		if (_value.HasMember("Flip"))
		{
			rapidjson::Value flip(rapidjson::kObjectType);
			flip = _value["Flip"].GetObj();
			Copium::Deserialize(_data.flip.x, flip, "X");
			Copium::Deserialize(_data.flip.y, flip, "Y");
		}

	}
	template<>
	void Serializer::Deserialize<SpriteRenderer>(SpriteRenderer& _data, const std::string& _key, rapidjson::Value& _value)
	{
		Copium::Deserialize(_data.enabled, _value, "enabled");
		Copium::Deserialize(_data.uuid.GetUUID(), _value, "UID");
		Deserialize(_data.sprite, "", _value);
	}

	template<>
	void Serializer::Deserialize<Button>(Button& _data, const std::string& _key, rapidjson::Value& _value)
	{
		Copium::Deserialize(_data.enabled, _value, "enabled");
		Copium::Deserialize(_data.uuid.GetUUID(), _value, "UID");
		Copium::Deserialize((uint64_t&)_data.targetGraphic, _value, "Graphic ID");

		Deserialize(_data.bounds, "BoundingBox", _value);

		Copium::Deserialize(_data.callbackName, _value, "Callback");
		Copium::Deserialize(_data.fadeDuration, _value, "FadeDuration");

		Copium::Deserialize(_data.clickedColor.r, _value, "clickedR");
		Copium::Deserialize(_data.clickedColor.g, _value, "clickedG");
		Copium::Deserialize(_data.clickedColor.b, _value, "clickedB");
		Copium::Deserialize(_data.clickedColor.a, _value, "clickedA");

		Copium::Deserialize(_data.hoverColor.r, _value, "hoveredR");
		Copium::Deserialize(_data.hoverColor.g, _value, "hoveredG");
		Copium::Deserialize(_data.hoverColor.b, _value, "hoveredB");
		Copium::Deserialize(_data.hoverColor.a, _value, "hoveredA");

		Copium::Deserialize(_data.normalColor.r, _value, "normalR");
		Copium::Deserialize(_data.normalColor.g, _value, "normalG");
		Copium::Deserialize(_data.normalColor.b, _value, "normalB");
		Copium::Deserialize(_data.normalColor.a, _value, "normalA");
	}
	template<>
	void Serializer::Deserialize<Text>(Text& _data, const std::string& _key, rapidjson::Value& _value)
	{
		Copium::Deserialize(_data.enabled, _value, "enabled");
		Copium::Deserialize(_data.fontName, _value, "FontName");
		_data.font = Font::getFont(_data.fontName);
		int va, ha;
		Copium::Deserialize(ha, _value, "H_Align");
		Copium::Deserialize(va, _value, "V_Align");
		_data.hAlignment = (HorizontalAlignment)ha;
		_data.vAlignment = (VerticalAlignment)va;
		
		std::string tmp;
		Copium::Deserialize(tmp, _value, "Content");
		strcpy(_data.content, tmp.c_str());



		Copium::Deserialize(_data.fSize, _value, "FontSize");
		Copium::Deserialize(_data.wrapper, _value, "Wrapper");

		Copium::Deserialize(_data.color.r, _value, "r");
		Copium::Deserialize(_data.color.g, _value, "g");
		Copium::Deserialize(_data.color.b, _value, "b");
		Copium::Deserialize(_data.color.a, _value, "a");


	}
	template<>
	void Serializer::Deserialize<Image>(Image& _data, const std::string& _key, rapidjson::Value& _value)
	{
		Copium::Deserialize(_data.enabled, _value, "enabled");
		int ha, va;
		Copium::Deserialize(ha, _value, "H_Align");
		Copium::Deserialize(va, _value, "V_Align");

		_data.hAlignment = (HorizontalAlignment)ha;
		_data.vAlignment = (VerticalAlignment)va;


		Deserialize(_data.sprite, "", _value);
	}

	template<>
	void Serializer::Deserialize<Script>(Script& _data, const std::string& _key, rapidjson::Value& _value)
	{
		Copium::Deserialize(_data.enabled, _value, "enabled");
		if (_data.fieldDataReferences.empty())
			return;
		for (auto it = _data.fieldDataReferences.begin(); it != _data.fieldDataReferences.end(); ++it)
		{
			const std::string& _name{ it->first };
			if (!_value.HasMember(_name.c_str()))
			{
				continue;
			}

			FieldType fType = it->second.fType;
			//Field& field{ _data.fieldDataReferences[_name] };

			switch (fType)
			{
			case FieldType::Float:
			{
				float buf{ 0.0f };
				Copium::Deserialize(buf, _value, _name);
				memcpy(_data.buffer, &buf, sizeof(float));
				break;
			}
			case FieldType::Double:
			{
				double buf{ 0.0 };
				Copium::Deserialize(buf, _value, _name);
				memcpy(_data.buffer, &buf, sizeof(double));
				break;
			}
			case FieldType::Bool:
			{
				bool buf{false};
				Copium::Deserialize(buf, _value, _name);
				memcpy(_data.buffer, &buf, sizeof(bool));
				break;
			}
			case FieldType::Char:
			{
				const char* buf{nullptr};
				Copium::Deserialize(buf, _value, _name);
				memcpy(_data.buffer, &buf, sizeof(char));
				break;
			}
			case FieldType::Short:
			{
				short buf{ 0 };
				Copium::Deserialize(buf, _value, _name);
				memcpy(_data.buffer, &buf, sizeof(short));
				break;
			}
			case FieldType::Int:
			{
				int buf{ 0 };
				Copium::Deserialize(buf, _value, _name);
				memcpy(_data.buffer, &buf, sizeof(int));
				break;
			}
			case FieldType::Long:
			{
				int64_t buf{ 0 };
				Copium::Deserialize(buf, _value, _name);
				memcpy(_data.buffer, &buf, sizeof(int64_t));
				break;
			}
			case FieldType::UShort:
			{
				uint16_t buf{ 0 };
				Copium::Deserialize(buf, _value, _name);
				memcpy(_data.buffer, &buf, sizeof(uint16_t));
				break;
			}
			case FieldType::UInt:
			{
				uint32_t buf{ 0 };
				Copium::Deserialize(buf, _value, _name);
				memcpy(_data.buffer, &buf, sizeof(uint32_t));
				break;
			}
			case FieldType::ULong:
			{
				uint64_t buf{ 0 };
				Copium::Deserialize(buf, _value, _name);
				memcpy(_data.buffer, &buf, sizeof(uint64_t));
				break;
			}
			case FieldType::String:
			{
				const char* buf = _value[_name.c_str()].GetString();
				strcpy(_data.buffer, buf);
				break;
			}
			case FieldType::Vector2:
			{
				rapidjson::Value& _v = _value[_name.c_str()].GetObj();
				Math::Vec2 tmp;
				Deserialize(tmp, "", _v);
				memcpy(_data.buffer, &tmp, sizeof(Math::Vec2));
				break;
			}
			case FieldType::Vector3:
			{
				rapidjson::Value& _v = _value[_name.c_str()].GetObj();
				Math::Vec3 tmp;
				Deserialize(tmp, "", _v);
				memcpy(_data.buffer, &tmp, sizeof(Math::Vec3));
				break;
			}
			case FieldType::GameObject:
			{
				//PRINT("GAMEOBJECT!");
				Copium::Deserialize((uint64_t&)_data.fieldGameObjReferences[_name], _value, _name);
				continue;
				break;
			}
			default:
			{
				//PRINT("COMPONENT!");
				Copium::Deserialize((uint64_t&)_data.fieldComponentReferences[_name], _value, _name);
				//PRINT("COMPONENT!" << (uint64_t&)_data.fieldComponentReferences[_name]);
				continue;
				break;
			}
			}
			//PRINT("FIELD: " << _name);
			MyEventSystem->publish(new ScriptSetFieldEvent(_data, _name.c_str(), _data.buffer));
		}

	}


	void Serializer::DeserializeComponent(ComponentType _type, Component* _data, rapidjson::Value& _value)
	{
		//int type{ -1 };
		//Copium::Deserialize(type, _value, "TypeID");


		//switch ((Copium::ComponentType)type)
		//{
		//case(ComponentType::Animator):
		//{
		//	Animator* animator = reinterpret_cast<Animator*>(_data);
		//	Deserialize(*animator, "", _value);
		//	break;
		//}
		//case(ComponentType::AudioSource):
		//{
		//	AudioSource* audio = reinterpret_cast<AudioSource*>(_data);
		//	Deserialize(*audio, "", _value);
		//	break;
		//}
		//case(ComponentType::BoxCollider2D):
		//{
		//	BoxCollider2D* bc = reinterpret_cast<BoxCollider2D*>(_data);
		//	Deserialize(*bc, "", _value);
		//	break;
		//}
		//case(ComponentType::Button):
		//{
		//	Button* button = reinterpret_cast<Button*>(_data);
		//	Deserialize(*button, "", _value);
		//	break;
		//}
		//case(ComponentType::Camera):
		//{
		//	Camera* cam = reinterpret_cast<Camera*>(_data);
		//	Deserialize(*cam, "", _value);
		//	break;
		//}
		//case(ComponentType::Image):
		//{
		//	Image* img = reinterpret_cast<Image*>(_data);
		//	Deserialize(*img, "", _value);
		//	break;
		//}
		//case(ComponentType::Rigidbody2D):
		//{
		//	Rigidbody2D* rb = reinterpret_cast<Rigidbody2D*>(_data);
		//	Deserialize(*rb, "", _value);
		//	break;
		//}
		//case(ComponentType::SpriteRenderer):
		//{
		//	SpriteRenderer* spriteRenderer = reinterpret_cast<SpriteRenderer*>(_data);
		//	Deserialize(*spriteRenderer, "", _value);
		//	break;
		//}
		//case(ComponentType::Script):
		//{
		//	Script* script = reinterpret_cast<Script*>(_data);
		//	Deserialize(*script, "", _value);
		//	break;
		//}
		//case(ComponentType::Text):
		//{
		//	Text* txt = reinterpret_cast<Text*>(_data);
		//	Deserialize(*txt, "", _value);
		//	break;
		//}
		//case(ComponentType::SortingGroup):
		//{
		//	SortingGroup* sg = reinterpret_cast<SortingGroup*>(_data);
		//	Deserialize(*sg, "", _value);
		//	break;
		//}
		//default:
		//	PRINT("component serialization failed!");
		//	break;
		//}
	}
	//-----------------------------------------------------------
}


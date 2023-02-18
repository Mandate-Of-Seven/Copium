#include "pch.h"
#include "serializer.h"
#include "Files/assets-system.h"
#include <Events/events-system.h>
namespace Copium
{	
	// Serialization --------------------------------------------
	template<>
	void Serializer::Serialize<Math::Vec2>(Math::Vec2& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		PRINT("Serializing a Vector2");
		rapidjson::Value v2(rapidjson::kObjectType);

		Copium::Serialize(_data.x, v2, _doc, "X");
		Copium::Serialize(_data.y, v2, _doc, "Y");

		rapidjson::Value key;
		CreateJsonString(_key, key, _doc);

		_value.AddMember(key, v2, _doc.GetAllocator());
	}
	template<>
	void Serializer::Serialize<Math::Vec3>(Math::Vec3& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		PRINT("Serializing a Vector3");

		rapidjson::Value v3(rapidjson::kObjectType);

		Copium::Serialize(_data.x, v3, _doc, "X");
		Copium::Serialize(_data.y, v3, _doc, "Y");
		Copium::Serialize(_data.z, v3, _doc, "Z");

		rapidjson::Value key;
		CreateJsonString(_key, key, _doc);

		_value.AddMember(key, v3, _doc.GetAllocator());
	}
	template<>
	void Serializer::Serialize<Math::Vec4>(Math::Vec4& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		PRINT("Serializing a Vector4");

		rapidjson::Value v4(rapidjson::kObjectType);

		Copium::Serialize(_data.x, v4, _doc, "X");
		Copium::Serialize(_data.y, v4, _doc, "Y");
		Copium::Serialize(_data.z, v4, _doc, "Z");
		Copium::Serialize(_data.a, v4, _doc, "A");

		rapidjson::Value key;
		CreateJsonString(_key, key, _doc);

		_value.AddMember(key, v4, _doc.GetAllocator());

	}


	/*void Serializer::SerializeComponent(const ComponentType& _type, Component* _co, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		switch (_type)
		{
		case(ComponentType::Animator):
		{
			Animator* animator = reinterpret_cast<Animator*>(_co);
			Serialize(*animator, "", _value, _doc);
			break;
		}
		case(ComponentType::AudioSource):
		{
			AudioSource* audio = reinterpret_cast<AudioSource*>(_co);
			Serialize(*audio, "", _value, _doc);
			break;
		}
		case(ComponentType::BoxCollider2D):
		{
			BoxCollider2D* bc = reinterpret_cast<BoxCollider2D*>(_co);
			Serialize(*bc, "", _value, _doc);
			break;	
		}	
		case(ComponentType::Button):
		{
			Button* button = reinterpret_cast<Button*>(_co);
			Serialize(*button, "", _value, _doc);
			break;
		}
		case(ComponentType::Camera):
		{
			Camera* cam = reinterpret_cast<Camera*>(_co);
			Serialize(*cam, "", _value, _doc);
			break;
		}
		case(ComponentType::Image):
		{
			Image* img = reinterpret_cast<Image*>(_co);
			Serialize(*img, "", _value, _doc);
			break;
		}
		case(ComponentType::Rigidbody2D):
		{
			Rigidbody2D* rb = reinterpret_cast<Rigidbody2D*>(_co);
			Serialize(*rb, "", _value, _doc);
			break;
		}
		case(ComponentType::SpriteRenderer):
		{
			SpriteRenderer* spriteRenderer = reinterpret_cast<SpriteRenderer*>(_co);
			Serialize(*spriteRenderer, "", _value, _doc);
			break;
		}
		case(ComponentType::Script):
		{
			Script* script = reinterpret_cast<Script*>(_co);
			Serialize(*script, "", _value, _doc);
			break;
		}
		case(ComponentType::Text):
		{
			Text* txt = reinterpret_cast<Text*>(_co);
			Serialize(*txt, "", _value, _doc);
			break;
		}
		case(ComponentType::SortingGroup):
		{
			SortingGroup* sg = reinterpret_cast<SortingGroup*>(_co);
			Serialize(*sg, "", _value, _doc);
			break;
		}
		default:
			PRINT("component serialization failed!");
			break;
		}
	}*/
	template<>
	void Serializer::Serialize<Animation>(Animation& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		Copium::Serialize(_data.spriteSheet.name, _value, _doc, "Name");
		Copium::Serialize(_data.spriteSheet.spriteID, _value, _doc, "Sprite ID");
		Copium::Serialize(_data.timeDelay, _value, _doc, "Time Delay");
		Copium::Serialize(_data.frameCount, _value, _doc, "FrameCount");
		Copium::Serialize(_data.spriteSheet.rows, _value, _doc, "Rows");
		Copium::Serialize(_data.spriteSheet.columns, _value, _doc, "Columns");

	}
	template<>
	void Serializer::Serialize<Animator>(Animator& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		PRINT("Serializing animator component...");
		Copium::Serialize(_data.uuid, _value, _doc, "UID");
		Copium::Serialize(GetComponentType<Animator>::name, _value, _doc, "Type");
		Copium::Serialize(_data.loop, _value, _doc, "Loop");
		Copium::Serialize(_data.reverse, _value, _doc, "Rev");
		Copium::Serialize(_data.animations.size(), _value, _doc, "Count");


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
			SerializeComponent(_data, _components,_doc);
		}

		template <typename T1, typename... T1s>
		void SerializeComponent(GameObject& _data, rapidjson::Value& _components, rapidjson::Document& _doc)
		{
			COPIUM_ASSERT(_components.GetType() != rapidjson::kArrayType, "Value should be of kArrayType!");
			for (T1* component : _data.GetComponents<T1>())
			{
				rapidjson::Value comp(rapidjson::kObjectType);
				//GetComponentType<T1>::name
				//GetComponentType<T1>::e
				Serialize(*component, "", comp, _doc);
				_components.PushBack(comp);
			}
			if constexpr (sizeof...(T1s) == 0)
			{
				return;
			}
			else
			{
				return SerializeComponent<T1s...>SerializeComponent(_data, _components, _doc);
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
		Copium::Serialize(_data.uuid, _value, _doc, "UID");
		Copium::Serialize(_data.name, _value, _doc, "Name");
		Copium::Serialize(_data.active, _value, _doc, "Active");

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
		PRINT("Serializing transform component...");
		uint64_t uid{ 0 };
		if (_data.HasParent())
			uid = _data.parent->gameObject.uuid;
		Copium::Serialize(uid, _value, _doc, "PID");

		Copium::Serialize("Transform", _value, _doc, "Type");

		Serialize(_data.position, "Pos", _value, _doc);
		Serialize(_data.rotation, "Rot", _value, _doc);
		Serialize(_data.scale, "Scale", _value, _doc);

	}
	template<>
	void Serializer::Serialize<SpriteRenderer>(SpriteRenderer& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		PRINT("Serializing sprite renderer...");
		Copium::Serialize(_data.uuid, _value, _doc, "UID");
		Copium::Serialize(GetComponentType<SpriteRenderer>::name, _value, _doc, "Type");
		// Serialize the sprite
		Serialize(_data.sprite, "", _value, _doc);
	}
	template<>
	void Serializer::Serialize<Sprite>(Sprite& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		Copium::Serialize(_data.spriteID, _value, _doc, "Sprite ID");
		Copium::Serialize(_data.sprite_name, _value, _doc, "Sprite Name");

		rapidjson::Value color(rapidjson::kObjectType);
		Copium::Serialize(_data.color.x, color, _doc, "X");
		Copium::Serialize(_data.color.y, color, _doc, "Y");
		Copium::Serialize(_data.color.z, color, _doc, "Z");
		Copium::Serialize(_data.color.w, color, _doc, "W");
		_value.AddMember("Color", color, _doc.GetAllocator());
	}

	template<>
	void Serializer::Serialize<AudioSource>(AudioSource& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		PRINT("Serializing audio source...");
		Copium::Serialize(_data.uuid, _value, _doc, "UID");
		Copium::Serialize(GetComponentType<AudioSource>::name, _value, _doc, "Type");

		Copium::Serialize(_data.alias, _value, _doc, "Alias");

	}

	template<>
	void Serializer::Serialize<Rigidbody2D>(Rigidbody2D& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		PRINT("Serializing rigidbody2d...");
		Copium::Serialize(_data.uuid, _value, _doc, "UID");
		Copium::Serialize(GetComponentType<Rigidbody2D>::name, _value, _doc, "Type");
		Copium::Serialize(_data.useGravity, _value, _doc, "UseGravity");
		Copium::Serialize(_data.isKinematic, _value, _doc, "IsKinematic");
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
		PRINT("Serializing camera...");
		Copium::Serialize(_data.uuid, _value, _doc, "UID");
		Copium::Serialize(GetComponentType<Camera>::name, _value, _doc, "Type");

		rapidjson::Value color(rapidjson::kObjectType);
		Copium::Serialize(_data.get_bg_color().x, color, _doc, "X");
		Copium::Serialize(_data.get_bg_color().y, color, _doc, "Y");
		Copium::Serialize(_data.get_bg_color().z, color, _doc, "Z");
		Copium::Serialize(_data.get_bg_color().w, color, _doc, "W");
		_value.AddMember("Color", color, _doc.GetAllocator());

		Copium::Serialize(_data.IsOrthographic(), _value, _doc, "Projection");

		rapidjson::Value clippingPlanes(rapidjson::kObjectType);
		Copium::Serialize(_data.GetNearClip(), clippingPlanes, _doc, "Near");
		Copium::Serialize(_data.GetFarClip(), clippingPlanes, _doc, "Far");
		_value.AddMember("Clipping Planes", clippingPlanes, _doc.GetAllocator());


	}
	template<>
	void Serializer::Serialize<BoxCollider2D>(BoxCollider2D& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		PRINT("Serializing boxcollider2d...");
		Copium::Serialize(_data.uuid, _value, _doc, "UID");
		Copium::Serialize(GetComponentType<BoxCollider2D>::name, _value, _doc, "Type");

		rapidjson::Value bb(rapidjson::kObjectType);
		Serialize(_data.bounds, "", bb, _doc);
		_value.AddMember("BoundingBox", bb, _doc.GetAllocator());
	}

	template<>
	void Serializer::Serialize<Image>(Image& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		PRINT("Serializing image...");
		Copium::Serialize(_data.uuid, _value, _doc, "UID");
		Copium::Serialize(GetComponentType<Image>::name, _value, _doc, "Type");

		Copium::Serialize((int)_data.hAlignment, _value, _doc, "H_Align");
		Copium::Serialize((int)_data.vAlignment, _value, _doc, "V_Align");

		// Serialize the sprite
		Serialize(_data.sprite, "", _value, _doc);
	}
	template<>
	void Serializer::Serialize<Button>(Button& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		PRINT("Serializing button...");
		Copium::Serialize(_data.uuid, _value, _doc, "UID");
		Copium::Serialize(GetComponentType<Button>::name, _value, _doc, "Type");

		uint64_t tgID{ 0 };
		if (_data.targetGraphic)
			tgID = _data.targetGraphic->uuid;
		Copium::Serialize(tgID, _value, _doc, "Graphic ID");

		rapidjson::Value bb(rapidjson::kObjectType);
		Serialize(_data.bounds, "", bb, _doc);
		_value.AddMember("Bounding Box", bb, _doc.GetAllocator());

		Copium::Serialize(_data.callbackName, _value, _doc, "Callback");

		Copium::Serialize(_data.clickedColor.r, _value, _doc, "clickedR");
		Copium::Serialize(_data.clickedColor.g, _value, _doc, "clickedG");
		Copium::Serialize(_data.clickedColor.b, _value, _doc, "clickedB");
		Copium::Serialize(_data.clickedColor.a, _value, _doc, "clickedA");

		Copium::Serialize(_data.hoverColor.r, _value, _doc, "hoveredR");
		Copium::Serialize(_data.hoverColor.g, _value, _doc, "hoveredG");
		Copium::Serialize(_data.hoverColor.b, _value, _doc, "hoveredB");
		Copium::Serialize(_data.hoverColor.a, _value, _doc, "hoveredA");

		Copium::Serialize(_data.normalColor.r, _value, _doc, "normalR");
		Copium::Serialize(_data.normalColor.g, _value, _doc, "normalG");
		Copium::Serialize(_data.normalColor.b, _value, _doc, "normalB");
		Copium::Serialize(_data.normalColor.a, _value, _doc, "normalA");

	}
	template<>
	void Serializer::Serialize<Text>(Text& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		PRINT("Serializing text...");
		Copium::Serialize(_data.uuid, _value, _doc, "UID");
		Copium::Serialize(GetComponentType<Text>::name, _value, _doc, "Type");

		Copium::Serialize(_data.fontName, _value, _doc, "FontName");
		Copium::Serialize((int)_data.hAlignment, _value, _doc, "H_Align");
		Copium::Serialize((int)_data.vAlignment, _value, _doc, "V_Align");

		Copium::Serialize(_data.content, _value, _doc, "Content");

		Copium::Serialize(_data.fSize, _value, _doc, "Font Size");
		Copium::Serialize(_data.wrapper, _value, _doc, "Wrapper");

		Copium::Serialize(_data.color.r, _value, _doc, "r");
		Copium::Serialize(_data.color.g, _value, _doc, "g");
		Copium::Serialize(_data.color.b, _value, _doc, "b");
		Copium::Serialize(_data.color.a, _value, _doc, "a");


	}

	template<>
	void Serializer::Serialize<Script>(Script& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		PRINT("Serializing script...");
		Copium::Serialize(_data.uuid, _value, _doc, "UID");
		Copium::Serialize(GetComponentType<Script>::name, _value, _doc, "Type");

		// Field Data References
		auto it = _data.fieldDataReferences.begin();
		while (it != _data.fieldDataReferences.end())
		{
			const std::string& fieldName{ it->first };
			_data.GetFieldValue(fieldName, _data.buffer);
			switch (it->second.fType)
			{
			case FieldType::Float:
			{
				Copium::Serialize(*reinterpret_cast<float*>(_data.buffer), _value, _doc, fieldName);
				break;
			}
			case FieldType::Double:
			{
				Copium::Serialize(*reinterpret_cast<double*>(_data.buffer), _value, _doc, fieldName);
				break;
			}
			case FieldType::Bool:
			{
				Copium::Serialize(*reinterpret_cast<bool*>(_data.buffer), _value, _doc, fieldName);
				break;
			}
			case FieldType::Char:
			{
				Copium::Serialize(*reinterpret_cast<char*>(_data.buffer), _value, _doc, fieldName);
				break;
			}
			case FieldType::Short:
			{
				Copium::Serialize(*reinterpret_cast<short*>(_data.buffer), _value, _doc, fieldName);
				break;
			}
			case FieldType::Int:
			{
				Copium::Serialize(*reinterpret_cast<int*>(_data.buffer), _value, _doc, fieldName);
				break;
			}			
			case FieldType::Long:
			{
				Copium::Serialize(*reinterpret_cast<int64_t*>(_data.buffer), _value, _doc, fieldName);
				break;
			}
			case FieldType::UShort:
			{
				Copium::Serialize(*reinterpret_cast<uint16_t*>(_data.buffer), _value, _doc, fieldName);
				break;
			}
			case FieldType::UInt:
			{
				Copium::Serialize(*reinterpret_cast<uint32_t*>(_data.buffer), _value, _doc, fieldName);
				break;
			}
			case FieldType::ULong:
			{
				Copium::Serialize(*reinterpret_cast<uint64_t*>(_data.buffer), _value, _doc, fieldName);
				break;
			}
			case FieldType::String:
			{
				std::string buf(_data.buffer);
				Copium::Serialize(buf, _value, _doc, fieldName);
				break;
			}
			case FieldType::Vector2:
			{
				float* fBuf = reinterpret_cast<float*> (_data.buffer);
				Math::Vec2 tmp{ *(fBuf++),*fBuf };
				Serialize(tmp, fieldName, _value, _doc);
				break;
			}
			case FieldType::Vector3:
			{
				float* fBuf = reinterpret_cast<float*> (_data.buffer);
				Math::Vec3 tmp{ *(fBuf++),*(fBuf++),*fBuf };
				Serialize(tmp, fieldName, _value, _doc);
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
			Copium::Serialize((*gameObjsIt).second->uuid, _value, _doc, fieldName);
			++gameObjsIt;
		}

		// Field Component References
		auto componentsIt = _data.fieldComponentReferences.begin();
		while (componentsIt != _data.fieldComponentReferences.end())
		{
			const std::string& fieldName{ componentsIt->first };
			Copium::Serialize((*componentsIt).second->uuid, _value, _doc, fieldName);
			++componentsIt;
		}

	}

	template<>
	void Serializer::Serialize<SortingGroup>(SortingGroup& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		PRINT("Serializing sorting group...");
		Copium::Serialize(_data.uuid, _value, _doc, "UID");
		Copium::Serialize(GetComponentType<SortingGroup>::name, _value, _doc, "Type");

		Copium::Serialize(_data.GetLayerID(), _value, _doc, "SortingLayer");
		Copium::Serialize(_data.GetOrderInLayer(), _value, _doc, "OrderInLayer");

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
		v3 = _value[_key.c_str()].GetObj();

		if (!Copium::Deserialize(_data.x, v3, "X"))
			_data.x = 0.f;

		if (!Copium::Deserialize(_data.y, v3, "Y"))
			_data.y = 0.f;

		if (!Copium::Deserialize(_data.z, v3, "Z"))
			_data.z = 0.f;

		PRINT(_data);

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
		PRINT("Deserializing a transform...");
		Copium::Deserialize(_data.pid, _value, "PID");
		Deserialize(_data.position, "Pos", _value);
		Deserialize(_data.rotation, "Rot", _value);
		Deserialize(_data.scale, "Scale", _value);

	}

	template<>
	void Serializer::Deserialize<Camera>(Camera& _data, const std::string& _key, rapidjson::Value& _value)
	{
		PRINT("Deserializing camera...");
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
		PRINT("Deserializing sorting group...");
		Copium::Deserialize(_data.uuid.GetUUID(), _value, "UID");

		int tmp{ 0 };
		Copium::Deserialize(tmp, _value, "SortingLayer");
		_data.sortingLayer = tmp;
		Copium::Deserialize(tmp, _value, "OrderInLayer");
		_data.orderInLayer = tmp;
	}

	template<>
	void Serializer::Deserialize<AudioSource>(AudioSource& _data, const std::string& _key, rapidjson::Value& _value)
	{
		PRINT("Deserializing audio source...");
		Copium::Deserialize(_data.uuid.GetUUID(), _value, "UID");
		std::string tmp;
		Copium::Deserialize(tmp, _value, "Alias");
		_data.set_alias(tmp);
	}


	template<>
	void Serializer::Deserialize<GameObject>(GameObject& _data, const std::string& _key, rapidjson::Value& _value)
	{
		PRINT("Deserializing a game object...");
		(void)_key;

		std::string name;
		if (!Copium::Deserialize(name, _value, "Name"))
			name = "New GameObject";
		_data.name = name;

		uint64_t uid = _data.uuid;
		if (!Copium::Deserialize(_data.uuid.GetUUID(), _value, "UID"))
			_data.uuid = uid;

		rapidjson::Value components(rapidjson::kArrayType);
		if (_value.HasMember("Components"))
		{
			components = _value["Components"].GetArray();

			for (rapidjson::Value::ValueIterator iter = components.Begin(); iter != components.End(); ++iter)
			{
				rapidjson::Value& component = *iter;

				std::string key;
				if (Copium::Deserialize(key, component, "Type"))
				{

					if (key == "Transform")
					{
						Deserialize(_data.transform, "", component);
					}
					else
					{
						PRINT("Deserializing " << key);
						Component* component{};
						ComponentType cType;
						switch (NAME_TO_CTYPE[key])
						{
							case ComponentType::Animator:
								return &addComponent<Animator>();
							case ComponentType::BoxCollider2D:
								return &addComponent<BoxCollider2D>();
							case ComponentType::Camera:
								return &addComponent<Camera>();
							case ComponentType::Rigidbody2D:
								return &addComponent<Rigidbody2D>();
							case ComponentType::SpriteRenderer:
								return &addComponent<SpriteRenderer>();
							case ComponentType::Script:
								return &addComponent<Script>();
							case ComponentType::Button:
								return &addComponent<Button>();
							case ComponentType::Image:
								return &addComponent<Image>();
							case ComponentType::Text:
								return _data.ddComponent<Text>();
							case ComponentType::AudioSource:
								return &addComponent<AudioSource>();
							case ComponentType::SortingGroup:
								return &addComponent<SortingGroup>();
						default:
							PRINT("ADDED NOTHING, MAYBE ADDED THE COMPONENT TO THE GAMEOBJECT.CPP");
							break;
						}
						Component* tmp = _data.addComponent(NAME_TO_CTYPE[key]);
						if (tmp)
						{
							// Deserialize stuff
							DeserializeComponent(NAME_TO_CTYPE[key], tmp, component);

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
		if (_data.loop)
			PRINT("looping");
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

		Deserialize(_data.min, "Min", bb);
		Deserialize(_data.max, "Max", bb);

	}
	template<>
	void Serializer::Deserialize<BoxCollider2D>(BoxCollider2D& _data, const std::string& _key, rapidjson::Value& _value)
	{
		Copium::Deserialize(_data.uuid.GetUUID(), _value, "UID");
		Deserialize(_data.bounds, "BoundingBox", _value);
	}
	template<>
	void Serializer::Deserialize<Rigidbody2D>(Rigidbody2D& _data, const std::string& _key, rapidjson::Value& _value)
	{
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


	}
	template<>
	void Serializer::Deserialize<SpriteRenderer>(SpriteRenderer& _data, const std::string& _key, rapidjson::Value& _value)
	{
		Copium::Deserialize(_data.uuid.GetUUID(), _value, "UID");
		Deserialize(_data.sprite, "", _value);
	}

	template<>
	void Serializer::Deserialize<Button>(Button& _data, const std::string& _key, rapidjson::Value& _value)
	{
		Copium::Deserialize(_data.uuid.GetUUID(), _value, "UID");
		uint64_t graphicID{ 0 };
		Copium::Deserialize(graphicID, _value, "Graphic ID");
		_data.graphicID = graphicID;

		Deserialize(_data.bounds, "BoundingBox", _value);

		Copium::Deserialize(_data.callbackName, _value, "Callback");

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
		
		Copium::Deserialize(_data.uuid.GetUUID(), _value, "UID");
		Copium::Deserialize(_data.fontName, _value, "FontName");
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
		Copium::Deserialize(_data.uuid.GetUUID(), _value, "UID");
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
		Copium::Deserialize(_data.uuid.GetUUID(), _value, "UID");
		for (auto it = _data.fieldDataReferences.begin(); it != _data.fieldDataReferences.end(); ++it)
		{
			const std::string& _name{ it->first };
			if (!_value.HasMember(_name.c_str()))
				continue;

			FieldType fType = it->second.fType;
			Field& field{ _data.fieldDataReferences[_name] };

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
				memcpy(_data.buffer, &buf, sizeof(char));
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
			case FieldType::Component:
			case FieldType::GameObject:
			{
				uint64_t buf{ 0 };
				Copium::Deserialize(buf, _value, _name);
				memcpy(_data.buffer, &buf, sizeof(uint64_t));
			}
			}
			_data.SetFieldValue(_name, _data.buffer);
		}

	}


	void Serializer::DeserializeComponent(ComponentType _type, Component* _data, rapidjson::Value& _value)
	{
		switch (_type)
		{
		case(ComponentType::Animator):
		{
			Animator* animator = reinterpret_cast<Animator*>(_data);
			Deserialize(*animator, "", _value);
			break;
		}
		case(ComponentType::AudioSource):
		{
			AudioSource* audio = reinterpret_cast<AudioSource*>(_data);
			Deserialize(*audio, "", _value);
			break;
		}
		case(ComponentType::BoxCollider2D):
		{
			BoxCollider2D* bc = reinterpret_cast<BoxCollider2D*>(_data);
			Deserialize(*bc, "", _value);
			break;
		}
		case(ComponentType::Button):
		{
			Button* button = reinterpret_cast<Button*>(_data);
			Deserialize(*button, "", _value);
			break;
		}
		case(ComponentType::Camera):
		{
			Camera* cam = reinterpret_cast<Camera*>(_data);
			Deserialize(*cam, "", _value);
			break;
		}
		case(ComponentType::Image):
		{
			Image* img = reinterpret_cast<Image*>(_data);
			Deserialize(*img, "", _value);
			break;
		}
		case(ComponentType::Rigidbody2D):
		{
			Rigidbody2D* rb = reinterpret_cast<Rigidbody2D*>(_data);
			Deserialize(*rb, "", _value);
			break;
		}
		case(ComponentType::SpriteRenderer):
		{
			SpriteRenderer* spriteRenderer = reinterpret_cast<SpriteRenderer*>(_data);
			Deserialize(*spriteRenderer, "", _value);
			break;
		}
		case(ComponentType::Script):
		{
			Script* script = reinterpret_cast<Script*>(_data);
			Deserialize(*script, "", _value);
			break;
		}
		case(ComponentType::Text):
		{
			Text* txt = reinterpret_cast<Text*>(_data);
			Deserialize(*txt, "", _value);
			break;
		}
		case(ComponentType::SortingGroup):
		{
			SortingGroup* sg = reinterpret_cast<SortingGroup*>(_data);
			Deserialize(*sg, "", _value);
			break;
		}
		default:
			PRINT("component serialization failed!");
			break;
		}
	}
	//-----------------------------------------------------------
}


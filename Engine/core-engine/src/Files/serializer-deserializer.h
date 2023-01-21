#pragma once

#include <CopiumCore/system-interface.h>
#include <Events/events-system.h>
#include <GameObject/components.h>
#include <GameObject/entity.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>
#include <GameObject/Components/physics-components.h>
#include <Animation/animation-struct.h>

namespace Copium
{
	class Serializer
	{
		template <typename T>
		void Serialize(const char* name,T& value, rapidjson::Value& _rjValue)
		{
			PRINT(typeid(T).name() << " does not have a Serialize function for it!");
		}

		void SerializeScene(const char* sceneName)
		{
			SparseSet<Entity, MAX_ENTITIES>* pEntitiesArray;
			MyEventSystem.publish(new GetEntitiesEvent{ pEntitiesArray });
			for (size_t i = 0; i < pEntitiesArray->GetSize(); ++i)
			{
				Entity& entity{ (*pEntitiesArray)[i] };
				EntityID entityID{ &entity - &pEntitiesArray->DenseGet(0)};
				SerializeComponents<AllComponents>(entityID);
			}
		}

		template <typename Component, typename... Components>
		void SerializeComponents(EntityID entityID)
		{
			bool hasComponent{ false };
			MyEventSystem.publish(new HasComponentEvent<Component>{entityID,hasComponent});
			Serialize(typeid(Component).name() + strlen("struct Copium::"));
			if constexpr (sizeof...(Components) != 0)
			{
				SerializeComponents<Components...>(entity);
			}
			else
			{

			}
		}

		// Other things
		template<>
		void Serialize(const char* name,Math::Vec3& value, rapidjson::Value& _rjValue)
		{

			(void)name;
			_rjValue.AddMember("X", value.x, _doc.GetAllocator());
			_rjValue.AddMember("Y", value.y, _doc.GetAllocator());
			_rjValue.AddMember("Z", value.z, _doc.GetAllocator());


		}
		template<>
		void Serialize(const char* name, Math::Vec2& value, rapidjson::Value& _rjValue)
		{

			(void)name;
			_rjValue.AddMember("X", value.x, _doc.GetAllocator());
			_rjValue.AddMember("Y", value.y, _doc.GetAllocator());

		}
		template<>
		void Serialize(const char* name, Math::Vec4& value, rapidjson::Value& _rjValue)
		{

			(void)name;
			_rjValue.AddMember("X", value.x, _doc.GetAllocator());
			_rjValue.AddMember("Y", value.y, _doc.GetAllocator());
			_rjValue.AddMember("Z", value.z, _doc.GetAllocator());
			_rjValue.AddMember("A", value.a, _doc.GetAllocator());

		}
		template<>
		void Serialize(const char* name, AABB& value, rapidjson::Value& _rjValue)
		{

			rapidjson::Value min(rapidjson::kObjectType);
			rapidjson::Value max(rapidjson::kObjectType);

			Serialize("", value.min, min);
			Serialize("", value.max, max);

			_rjValue.AddMember("Max", max, _doc.GetAllocator());
			_rjValue.AddMember("Min", min, _doc.GetAllocator());

		}
		
		// Components
		template<>
		void Serialize(const char* name, Transform& value, rapidjson::Value& _rjValue)
		{

			rapidjson::Value type;
			type.SetString(name, strlen(name), _doc.GetAllocator());
			_rjValue.AddMember("Type", type, _doc.GetAllocator());

			rapidjson::Value pos(rapidjson::kObjectType);
			rapidjson::Value rot(rapidjson::kObjectType);
			rapidjson::Value scale(rapidjson::kObjectType);

			Serialize("Pos", value.position, pos);
			_rjValue.AddMember("Pos", pos, _doc.GetAllocator());
			Serialize("Rot", value.rotation, rot);
			_rjValue.AddMember("Rot", rot, _doc.GetAllocator());
			Serialize("Scale", value.scale, scale);
			_rjValue.AddMember("Scale", scale, _doc.GetAllocator());

			EntityID pid{ 0 };
			if (value.HasParent())
				pid = value.parentID;
			_rjValue.AddMember("Parent ID:", pid, _doc.GetAllocator());

			rapidjson::Value children(rapidjson::kArrayType);
			for (int i{ 0 }; i < value.childrenIDs.size(); ++i)
			{
				children.PushBack(value.childrenIDs[i], _doc.GetAllocator());
			}
			_rjValue.AddMember("Children IDs", children, _doc.GetAllocator());

		}
		template<>
		void Serialize(const char* name, BoxCollider2D& value, rapidjson::Value& _rjValue)
		{
			rapidjson::Value type;
			type.SetString(name, strlen(name), _doc.GetAllocator());
			_rjValue.AddMember("Type", type, _doc.GetAllocator());

			rapidjson::Value bounds(rapidjson::kObjectType);
			Serialize("", value.bounds, bounds);
			_rjValue.AddMember("Bounds", bounds, _doc.GetAllocator());

		}
		template<>
		void Serialize(const char* name, Rigidbody2D& value, rapidjson::Value& _rjValue)
		{
			rapidjson::Value type;
			type.SetString(name, strlen(name), _doc.GetAllocator());
			_rjValue.AddMember("Type", type, _doc.GetAllocator());

			rapidjson::Value velocity(rapidjson::kObjectType);
			rapidjson::Value acceleration(rapidjson::kObjectType);
			rapidjson::Value force(rapidjson::kObjectType);

			Math::Vec2 tmp{ value.get_vel() };
			Serialize("", tmp, velocity);
			_rjValue.AddMember("Velocity", velocity, _doc.GetAllocator());

			tmp = value.get_acc();
			Serialize("", tmp, acceleration);
			_rjValue.AddMember("Acceleration", acceleration, _doc.GetAllocator());

			tmp = value.get_force();
			Serialize("", tmp, force);
			_rjValue.AddMember("Force", force, _doc.GetAllocator());

			_rjValue.AddMember("Mass", value.get_mass(), _doc.GetAllocator());
			_rjValue.AddMember("Active", value.Active(), _doc.GetAllocator());
			_rjValue.AddMember("Grav", value.get_gravity(), _doc.GetAllocator());

		}
		template<>
		void Serialize(const char* name, Animator& _animator, rapidjson::Value& _rjValue) {
			rapidjson::Value type;
			type.SetString(name, strlen(name), _doc.GetAllocator());
			_rjValue.AddMember("Type", type, _doc.GetAllocator());

			_rjValue.AddMember("Loop", _animator.loop, _doc.GetAllocator());
			_rjValue.AddMember("Count", _animator.animationCount, _doc.GetAllocator());

			rapidjson::Value animations(rapidjson::kArrayType);
			for (Animation& anim : _animator.animations) 
			{
				rapidjson::Value a(rapidjson::kObjectType);
				rapidjson::Value name;
				name.SetString(anim.spriteSheet.name.c_str(),
					(rapidjson::SizeType)anim.spriteSheet.name.length(), _doc.GetAllocator());
				a.AddMember("Name", name, _doc.GetAllocator());
				a.AddMember("Sprite ID", anim.spriteSheet.spriteID, _doc.GetAllocator());
				a.AddMember("Time Delay", anim.timeDelay, _doc.GetAllocator());
				a.AddMember("Frame Count", anim.frameCount, _doc.GetAllocator());

				animations.PushBack(a, _doc.GetAllocator());
			}
			_rjValue.AddMember("Animations", animations, _doc.GetAllocator());
		}
	private:
		rapidjson::Document _doc{};
	};

	class Deserializer
	{

		template <typename T>
		void Deserialize(const char* name, T& value, rapidjson::Value& _rjValue)
		{
			PRINT(typeid(T).name() << " does not have a Serialize function for it!");
		}

		// Math
		template<>
		void Deserialize(const char* name, Math::Vec2& value, rapidjson::Value& _rjValue)
		{
			if(_rjValue.HasMember("X"))
				value.x = _rjValue["X"].GetFloat();
			if (_rjValue.HasMember("Y"))
				value.x = _rjValue["Y"].GetFloat();
		}
		template<>
		void Deserialize(const char* name, Math::Vec3& value, rapidjson::Value& _rjValue)
		{
			if (_rjValue.HasMember("X"))
				value.x = _rjValue["X"].GetFloat();
			if (_rjValue.HasMember("Y"))
				value.x = _rjValue["Y"].GetFloat();
			if (_rjValue.HasMember("Z"))
				value.x = _rjValue["Z"].GetFloat();
		}		
		template<>
		void Deserialize(const char* name, Math::Vec4& value, rapidjson::Value& _rjValue)
		{
			if (_rjValue.HasMember("X"))
				value.x = _rjValue["X"].GetFloat();
			if (_rjValue.HasMember("Y"))
				value.x = _rjValue["Y"].GetFloat();
			if (_rjValue.HasMember("Z"))
				value.x = _rjValue["Z"].GetFloat();
			if (_rjValue.HasMember("A"))
				value.x = _rjValue["A"].GetFloat();
		}

		// Physics
		template<>
		void Deserialize(const char* name, AABB& _boundingBox, rapidjson::Value& _rjValue) 
		{
			if (_rjValue.HasMember("Max"))
			{
				rapidjson::Value max(rapidjson::kObjectType);
				max = _rjValue["Max"].GetObj();
				Deserialize("", _boundingBox.max, max);
			}
			if (_rjValue.HasMember("Min"))
			{
				rapidjson::Value min(rapidjson::kObjectType);
				min = _rjValue["Min"].GetObj();
				Deserialize("", _boundingBox.min, min);
			}

		}
		template<>
		void Deserialize(const char* name, BoxCollider2D& _box, rapidjson::Value& _rjValue)
		{
			if (_rjValue.HasMember("Bounds"))
			{
				rapidjson::Value box(rapidjson::kObjectType);
				box = _rjValue["Bounds"].GetObj();
				Deserialize("", _box.bounds, box);
			}
		}
		template<>
		void Deserialize(const char* name, Rigidbody2D& _rb, rapidjson::Value& _rjValue)
		{
			Math::Vec2 tmp;
			if (_rjValue.HasMember("Velocity"))
			{

				rapidjson::Value vel(rapidjson::kObjectType);
				vel = _rjValue["Velocity"].GetObj();
				Deserialize("", tmp, vel);
				_rb.set_vel(tmp);
			}
			if (_rjValue.HasMember("Acceleration"))
			{
				rapidjson::Value acc(rapidjson::kObjectType);
				acc = _rjValue["Acceleration"].GetObj();
				Deserialize("", tmp, acc);
				_rb.set_acc(tmp);
			}
			if (_rjValue.HasMember("Force"))
			{
				rapidjson::Value force(rapidjson::kObjectType);
				force = _rjValue["Force"].GetObj();
				Deserialize("", tmp, force);
				_rb.set_force(tmp);
			}


			if (_rjValue.HasMember("Mass"))
			{
				float m{ 0.f };
				m = _rjValue["Mass"].GetFloat();

				_rb.set_mass(m);

			}
			if (_rjValue.HasMember("Active"))
			{
				bool b = _rjValue["Active"].GetBool();
				_rb.set_active(b);

			}
			if (_rjValue.HasMember("Grav"))
			{
				bool g = _rjValue["Grav"].GetBool();
				_rb.set_gravity(g);
			}
		}

		// Transform
		template<>
		void Deserialize(const char* name, Transform& _transform, rapidjson::Value& _rjValue)
		{
			if (_rjValue.HasMember("Pos"))
			{
				rapidjson::Value pos;
				pos = _rjValue["Pos"].GetObj();
				Deserialize("", _transform.position, pos);
			}
			if (_rjValue.HasMember("Rot"))
			{
				rapidjson::Value rot;
				rot = _rjValue["Rot"].GetObj();
				Deserialize("", _transform.rotation, rot);
			}
			if (_rjValue.HasMember("Scale"))
			{
				rapidjson::Value scale;
				scale = _rjValue["scale"].GetObj();
				Deserialize("", _transform.scale, scale);
			}

			if (_rjValue.HasMember("Parent ID"))
				_transform.parentID = _rjValue["Parent ID"].GetUint64();

			if (_rjValue.HasMember("Children IDs"))
			{

				rapidjson::Value arr(rapidjson::kArrayType);
				arr = _rjValue["Children IDs"].GetArray();
				for (rapidjson::Value::ValueIterator iter = arr.Begin(); iter != arr.End(); ++iter)
				{
					_transform.childrenIDs.push_back((*iter).GetUint64());
				}

			}

		}

		// Animator
		template<>
		void Deserialize(const char* name, Animator& _animator, rapidjson::Value& _rjValue)
		{
			if (_rjValue.HasMember("Loop"))
				_animator.loop = _rjValue["Loop"].GetBool();
			if (_rjValue.HasMember("Count"))
			{
				_animator.animationCount = _rjValue["Count"].GetUint();
			}

			rapidjson::Value animations(rapidjson::kArrayType);
			if (_rjValue.HasMember("Animations"))
			{

				animations = _rjValue["Animations"].GetArray();
				for (rapidjson::Value::ValueIterator iter = animations.Begin(); iter != animations.End(); ++iter)
				{
					Animation tmp;
					if (iter->HasMember("Name"))
					{
						tmp.spriteSheet.name = (*iter)["Name"].GetString();
					}
					if (iter->HasMember("Sprite ID"))
					{
						tmp.spriteSheet.spriteID = (*iter)["Sprite ID"].GetUint();
					}
					if (iter->HasMember("Time Delay"))
					{
						tmp.timeDelay = (*iter)["Time Delay"].GetFloat();
					}
					if (iter->HasMember("Frame Count"))
					{
						tmp.frameCount = (*iter)["Frame Count"].GetInt();
					}

					_animator.animations.push_back(tmp);
				}
			}
		}

		void init() {}
		void update() {}
		void exit() {}
		rapidjson::Document doc{};
	};
}
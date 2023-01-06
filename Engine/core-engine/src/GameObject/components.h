#pragma once

#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <Physics/collision.h>
#include <imgui.h>
#include <Utilities/sparse-set.h>
#include <config.h>


namespace Copium
{

	using ComponentID = size_t;

	enum class ComponentType : size_t      // Types of Components
	{
		Transform = 0,
		Animator,
		AudioSource,
		BoxCollider2D,
		Button,
		Camera,
		Image,
		Rigidbody2D,
		SpriteRenderer,
		Script,
		Text,
		None
	};

	static std::map<ComponentType, std::string> MAP_COMPONENT_TYPE_NAME
	{
		{ComponentType::Animator,"Animator"},
		{ComponentType::AudioSource,"AudioSource"},
		{ComponentType::BoxCollider2D,"BoxCollider2D"},
		{ComponentType::Button,"Button"},
		{ComponentType::Camera,"Camera"},
		{ComponentType::Image,"ImageComponent"},
		{ComponentType::Rigidbody2D,"Rigidbody2D"},
		{ComponentType::SpriteRenderer,"SpriteRenderer"},
		{ComponentType::Script,"Script"},
		{ComponentType::Button,"Button"},
		{ComponentType::Text,"Text"},
	};

	template<typename T, typename... Ts>
	constexpr bool contains()
	{
		return std::disjunction_v<std::is_same<T, Ts>...>;
	}

	template <typename... Ts>
	struct Pack{};

	template<size_t ID, typename... Components>
	struct ComponentGroup
	{
		using Types = Pack<Components...>;

		template <typename T>
		static constexpr bool Has()
		{
			return contains<T, Components...>();
		}

		template <typename T>
		static SparseSet<T, MAX_ENTITIES>& GetArray()
		{
			static_assert(contains<T, Components...>());
			static SparseSet<T, MAX_ENTITIES> components;
			return components;
		}

		template <typename T>
		static void ComponentEnabled(EntityID entityID, bool* resultOrVal, bool setting = true)
		{
			static_assert(contains<T, Components...>());
			static std::bitset<MAX_ENTITIES> componentsBitset;
			if (setting)
			{
				componentsBitset.set(entityID, *resultOrVal);
			}
			{
				*resultOrVal = componentsBitset.test(entityID);
			}
		}
	};


	class Transform
	{
	public:
		Transform() = default;

		Transform* parent = nullptr;
		std::list<Transform*> children;
		Math::Vec3 position;
		Math::Vec3 rotation;
		Math::Vec3 scale;

		bool hasParent() const
		{
			return parent;
		}

		void setParent(Transform* _parent)
		{
			//Previously had a parent
			if (parent)
				parent->children.remove(this);
			parent = _parent;
			//_parent might be nullptr
			if (_parent)
				_parent->children.push_back(this);
		}

		/***************************************************************************/
		/*!
		\brief
			Constructor that takes in position, rotations,scale to 1, and initializes
			parent to nullptr
		*/
		/**************************************************************************/
		Transform(
			ComponentID _entityID,
			Math::Vec3 _position = Math::Vec3(),
			Math::Vec3 _rotation = Math::Vec3(),
			Math::Vec3 _scale = { 1,1,1 });
	};

	struct AABB;


	struct BoxCollider2D
	{
		/*******************************************************************************
		/*!
		*
		\brief
			Displays the inspector view with its fields

		*/
		/*******************************************************************************/
		void inspector_view()
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
				/*if (ImGui::BeginTable("Component AABB", 4, windowFlags))
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
				}*/
			}
			ImGui::Unindent();
			ImGui::EndTable();
		}
	};

	using MainComponents = ComponentGroup<0, Transform>;
	using BackupComponents = ComponentGroup<1, Transform>;

///#define RegisterComponent(Type)template <> ComponentType GetComponentType<Type>(){return ComponentType::Type;}

	template <typename T>
	ComponentType GetComponentType()
	{
		return ComponentType::None;
	}

	//RegisterComponent(Transform)
}


#endif // !COMPONENTS_H
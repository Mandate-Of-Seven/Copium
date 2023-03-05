

#ifndef COMPONENTS_H
#define COMPONENTS_H
#include <config.h>
#include <Utilities/sparse-set.h>
#include <CopiumCore/uuid.h>
#include <Graphics/base-camera.h>
#include <Physics/collision.h>
#include <Messaging/message-system.h>
#include <Animation/animation-struct.h>
#include <Math/math-library.h>
#include <Graphics/sprite.h>
#include <Scripting/script-fields.h>
#include <vector>
#include <map>

#define RegisterComponent(Type) template <> struct GetComponentType<Type>{static constexpr size_t e{ (size_t)ComponentType::Type }; static constexpr const char* name = #Type;}

namespace Copium
{
	class GameObject;
	class UUID;
	class Font;
	using FieldMap = std::map<std::string, Field>;

	enum class ComponentType : int      // Types of Components
	{
		Animator = (int)FieldType::Component,
		AudioSource,
		BoxCollider2D,
		Button,
		Camera,
		Image,
		Rigidbody2D,
		SpriteRenderer,
		Script,
		Transform,
		Text,
		SortingGroup,
		None
	};

	//REMOVE WHEN SERIALIZATION HAS SWITCHED TO USE ENUMS INSTEAD OF NAMES
	static std::map<std::string,ComponentType> NAME_TO_CTYPE
	{
		{"Animator",ComponentType::Animator},
		{"AudioSource",ComponentType::AudioSource},
		{"BoxCollider2D",ComponentType::BoxCollider2D},
		{"Button",ComponentType::Button},
		{"Camera",ComponentType::Camera},
		{"Image",ComponentType::Image},
		{"Rigidbody2D",ComponentType::Rigidbody2D},
		{"SpriteRenderer",ComponentType::SpriteRenderer},
		{"Script",ComponentType::Script},
		{"Text",ComponentType::Text},
		{"SortingGroup",ComponentType::SortingGroup}
	};

	template <typename T>
	using ComponentsArray = SparseSet<T, MAX_COMPONENTS>;
	template <typename T>
	using ComponentsPtrArray = std::vector<T*>;

	class GameObject;

	class Component
	{
	public:
		Component() = delete;
		~Component() {};

		bool enabled{ true };
		UUID uuid;
		GameObject& gameObj;
	protected:
		/***************************************************************************/
		/*!
		\brief
			Hidden base class constructor for derived classes to implement
			determine the componentType
		\param _gameObj
			Owner of this component
		\param _componentType
			Type of component to be defined by derived classes
		*/
		/**************************************************************************/
		Component(GameObject& _gameObj, UUID _uuid = UUID()) : gameObj{ _gameObj }, uuid{ _uuid } {};
	};

	template<typename T, typename... Ts>
	struct ComponentGroup
	{
		static_assert(std::is_base_of<Component, T>());

		ComponentsArray<T> components;
		ComponentGroup<Ts...> others;

		constexpr ComponentGroup(TemplatePack<T, Ts...> pack) {}
		constexpr ComponentGroup() = default;

		template <typename T1>
		static constexpr bool Has() { return contains<T1, T, Ts...>(); }

		template <typename T1>
		constexpr ComponentsArray<T1>& GetArray()
		{
			if constexpr (std::is_same<T1, T>())
				return components;
			else
				return others.GetArray<T1>();
		}

		Component* FindByUUID(UUID _uuid)
		{
			for (T& component : components)
			{
				if (component.uuid == _uuid)
					return reinterpret_cast<Component*>(&component);
			}
			if constexpr (sizeof...(Ts) != 0)
			{
				return others.FindByUUID(_uuid);
			}
			else
			{
				return nullptr;
			}
		}
	};

	template<typename T>
	struct ComponentGroup <T>
	{
		static_assert(std::is_base_of<Component, T>());
		constexpr ComponentGroup(TemplatePack<T> pack) {}
		constexpr ComponentGroup() = default;
		ComponentsArray<T> components;
		template <typename T1>
		constexpr ComponentsArray<T1>& GetArray()
		{
			return components;
		}

		Component* FindByUUID(UUID _uuid)
		{
			for (T& component : components)
			{
				if (component.uuid == _uuid)
					return reinterpret_cast<Component*>(&component);
			}
			return nullptr;
		}
	};

	template<typename T, typename... Ts>
	struct ComponentPtrGroup
	{
		static_assert(std::is_base_of<Component, T>());

		ComponentsPtrArray<T> components;
		ComponentPtrGroup<Ts...> others;

		constexpr ComponentPtrGroup(TemplatePack<T, Ts...> pack) {}
		constexpr ComponentPtrGroup() = default;

		template <typename T1>
		static constexpr bool Has() { return contains<T1, T, Ts...>(); }

		template <typename T1>
		constexpr ComponentsPtrArray<T1>& GetArray()
		{
			if constexpr (std::is_same<T1, T>())
				return components;
			else
				return others.GetArray<T1>();
		}


		Component* FindByUUID(UUID _uuid)
		{
			for (T* component : components)
			{
				if (component->uuid == _uuid)
					return reinterpret_cast<Component*>(component);
			}
			if constexpr (sizeof...(Ts) != 0)
			{
				return others.FindByUUID();
			}
			return nullptr;
		}
	};

	template<typename T>
	struct ComponentPtrGroup <T>
	{
		static_assert(std::is_base_of<Component, T>());
		constexpr ComponentPtrGroup(TemplatePack<T> pack) {}
		constexpr ComponentPtrGroup() = default;
		ComponentsPtrArray<T> components;
		template <typename T1>
		constexpr ComponentsPtrArray<T1>& GetArray()
		{
			static_assert(std::is_same<T, T1>());
			return components;
		}

		Component* FindByUUID(UUID _uuid)
		{
			for (T* component : components)
			{
				if (component->uuid == _uuid)
					return reinterpret_cast<Component*>(component);
			}
			return nullptr;
		}

	};

	class Transform
	{
	public:
		Transform* parent = nullptr;
		std::list<Transform*> children;
		Math::Vec3 position{};
		Math::Vec3 rotation{};
		Math::Vec3 scale{1,1,1};
		GameObject& gameObject;
		/***************************************************************************/
		/*!
		\brief
			Constructor that takes in position, rotations,scale to 1, and initializes
			parent to nullptr
		*/
		/**************************************************************************/
		Transform(GameObject& _gameObject) : gameObject{ _gameObject }, parent{ nullptr } {}
		Transform(GameObject& _gameObject, const Transform& rhs);
		~Transform() 
		{
		}
		Math::Vec3 GetWorldPosition() const;
		Math::Vec3 GetWorldRotation() const;
		Math::Vec3 GetWorldScale() const;
		bool HasParent() const { return parent; }
		void SetParent(Transform* _parent);
	};

	class Animator : public Component
	{
	public:
		// For handling playing, pausing
		enum class AnimatorStatus : char
		{
			idle = 0,
			playing,
			paused
		};
		/***************************************************************************/
		/*!
		\brief
			Constructor for animator components
		\param _gameObj
			Owner of this component
		*/
		/**************************************************************************/
		Animator(GameObject& _gameObj,UUID _uuid = UUID()) : Component(_gameObj)
		{
		}
		Animator(GameObject& _gameObj, const Animator& rhs, UUID _uuid = UUID()) :
			Component(_gameObj, _uuid), animations{ rhs.animations },
			currentAnimationIndex{rhs.currentAnimationIndex},
			startingAnimationIndex{rhs.startingAnimationIndex},
			loop{ rhs.loop }, reverse{ rhs.reverse }, status{ rhs.status }{}
		/***************************************************************************/
		/*!
		\brief
			Updates the timer which determines whether to move to the next frame or not
		\param _dt
			delta time
		\return
			void
		*/
		/**************************************************************************/
		void Update(double _dt);
		/***************************************************************************/
		/*!
		\brief
			Get the vector of animations attached to this animator
		\return
			reference to this animator's vector of animations
		*/
		/**************************************************************************/
		std::vector<Animation>& get_animation_vector() { return animations; }
		/***************************************************************************/
		/*!
		\brief
			Checks if there are any animations in this animator
		\return
			true if there are animations
			false if there are no animations
		*/
		/**************************************************************************/
		bool IsEmpty() const { return animations.empty(); }
		/***************************************************************************/
		/*!
		\brief
			Add an animation to this animator
		\return
			void
		*/
		/**************************************************************************/
		void AddAnimation();
		/***************************************************************************/
		/*!
		\brief
			Play the current animation
		\return
			void
		*/
		/**************************************************************************/
		void PlayAnimation(){status = AnimatorStatus::playing;}
		/***************************************************************************/
		/*!
		\brief
			Pause the current animation
		\return
			void
		*/
		/**************************************************************************/
		void PauseAnimation(){status = AnimatorStatus::paused;}
		/***************************************************************************/
		/*!
		\brief
			Get the current animation
		\return
			pointer to the current animation
		*/
		/**************************************************************************/
		Animation* GetCurrentAnimation()
		{
			if (IsEmpty())
				return nullptr;
			return &animations[currentAnimationIndex];
		}
		/***************************************************************************/
		/*!
		\brief
			Gets the AnimatorStatus
		\return
			AnimatorStatus
		*/
		/**************************************************************************/
		AnimatorStatus GetStatus() { return status; }
		/***************************************************************************/
		/*!
		\brief
			Set the AnimatorStatus to the specified param
		\param _status
			the new AnimatorStatus of this animator
		\return
			void
		*/
		/**************************************************************************/
		void SetStatus(AnimatorStatus _status) { status = _status; }
		std::vector<Animation> animations;    // The indices of the animations inside the assets-system
		int currentAnimationIndex{0};      // Current playing animation
		int startingAnimationIndex{0};     // The first animation that is playing
		bool loop{ true }, reverse{ false };
		AnimatorStatus status{ AnimatorStatus::idle };
	};

	class AudioSource : public Component
	{
	public:
		std::string alias;
		bool overLap = true;
		bool loop = false;
		int loopCount{ 0 };
		std::string channel;
		float volume = 1.0f;
		const char* channelName[4] = { "Default", "BGM", "SFX", "Voice" };
		/***************************************************************************/
		/*!
		\brief
			Constructor for audio source
		\param  _gameObj
			reference to game object that this belongs to
		*/
		/**************************************************************************/
		AudioSource(GameObject& _gameObj, UUID _uuid = UUID()):Component(_gameObj, _uuid){}
		/***************************************************************************/
		/*!
		\brief
			assignment operator overload
		\param  rhs
			reference to audio source whose values need to be assigned
		*/
		/**************************************************************************/
		AudioSource(GameObject& _gameObj, const AudioSource& rhs, UUID _uuid = UUID()) :
			Component(_gameObj, _uuid), alias{ rhs.alias }, loop{ rhs.loop }, channel{rhs.channel},
			overLap{rhs.overLap}, volume{rhs.volume}
		{}
		/***************************************************************************/
		/*!
		\brief
			Plays the audio file inside the audio source
		*/
		/**************************************************************************/
		void play_sound();
		/***************************************************************************/
		/*!
		\brief
			Stops the audio file inside the audio source if its playing
		*/
		/**************************************************************************/
		void stop_sound();
		void stop_all_sound()
		{
			//soundSystem.StopAll();
		}
		/***************************************************************************/
		/*!
		\brief
			Deserializes transform data from a rapidjson::Value
		\param  s
			string to set for the alias
		*/
		/**************************************************************************/
		void set_alias(std::string s) { alias = s; }
	};

	class BoxCollider2D : public Component
	{
	public:
		/***************************************************************************/
		/*!
		\brief
			Constructor for collider Components
		\param _gameObj
			Owner of this component
		*/
		/**************************************************************************/
		BoxCollider2D(GameObject& _gameObj, UUID _uuid = UUID()) :Component(_gameObj, _uuid) {}
		BoxCollider2D(GameObject& _gameObj, const BoxCollider2D& rhs, UUID _uuid = UUID()):
			Component(_gameObj, _uuid),bounds{rhs.bounds}{}
		AABB bounds{};
	};


	class Camera : public Component, public BaseCamera
	{
	public:
		/***************************************************************************/
		/*!
		\brief
			Constructor for camera component
		\param _gameObj
			Owner of this component
		*/
		/**************************************************************************/
		Camera(GameObject& _gameObj, UUID _uuid = UUID()) : Component{_gameObj,_uuid}
		{
			PRINT("  Camera Component constructed");
			// Bean: Checking for archetypes
			BaseCamera::init(1280.f, 720.f, CameraType::GAME, true);
		}

		Camera(GameObject& _gameObj, const Camera& rhs, UUID _uuid = UUID()) :
			Component(_gameObj, _uuid), BaseCamera(rhs)
		{
			BaseCamera::init(1280.f, 720.f, CameraType::GAME, true);
		}

		~Camera()
		{
			BaseCamera::exit();
		}
	};

	class Rigidbody2D : public Component
	{
	public:
		/***************************************************************************/
		/*!
		\brief
		Constructor for the Rigidbody2D
		\param _gameObj
		Owner of this component
		*/
		/**************************************************************************/
		Rigidbody2D(GameObject& _gameObj, UUID _uuid = UUID()):Component(_gameObj, _uuid) {}
		Math::Vec2 velocity{};					//velocity of object
		Math::Vec2 acceleration{};				//acceleration of object
		Math::Vec2 force{};						//forces acting on object
		float mass{1.f};						//mass of object
		bool isKinematic{true};					//is object active?
		bool useGravity{true};					//is object affected by gravity?


		Rigidbody2D(GameObject& _gameObj, const Rigidbody2D& rhs, UUID _uuid = UUID()) :
			Component(_gameObj, _uuid), mass{rhs.mass}, isKinematic{rhs.isKinematic},
			useGravity{rhs.useGravity}
		{}
	};



	//using ScriptReferenceables = TemplatePack<GameObject, Component>;

	class Script final : public Component
	{
	public:
		/**************************************************************************/
		/*!
			\brief
				Constructs a Script
			\param gameObj
				Owner of this
		*/
		/**************************************************************************/
		Script(GameObject& _gameObj, UUID _uuid = UUID(), const std::string& _name = "New Script") :Component(_gameObj, _uuid), name{ _name }
		{}


		Script(GameObject& _gameObj, const Script& rhs, UUID _uuid = UUID()) :
			Component(_gameObj, _uuid), name{ rhs.name }, fieldDataReferences{ rhs.fieldDataReferences }, 
			fieldComponentReferences{ rhs.fieldComponentReferences }, fieldGameObjReferences{ rhs.fieldGameObjReferences }{}

		Script& operator=(const Script& rhs)
		{
			return *this;
		}
		/**************************************************************************/
		/*!
			\brief
				IReceiver overload to handle a the MT_SCRIPTING_UPDATED message
			\param mType
				Type of message
		*/
		/**************************************************************************/
		void handleMessage(MESSAGE_TYPE mType)
		{
			switch (mType)
			{
			case MESSAGE_TYPE::MT_SCRIPTING_UPDATED:
			{
				//CreateFields
			}
			case MESSAGE_TYPE::MT_SCENE_DESERIALIZED:
				//for (auto pair : fieldComponentReferences)
				//{
				//	MyEventSystem->publish(new ScriptSetFieldReferenceEvent(*this, pair.first.c_str(), pair.second));
				//}
				//for (auto pair : fieldGameObjReferences)
				//{
				//	MyEventSystem->publish(new ScriptSetFieldReferenceEvent(*this, pair.first.c_str(), pair.second));
				//}
				break;
				//CreateFields
			}
		}
		/***************************************************************************/
		/*!
		\brief
			Name function for scripts to return their scriptname as their
			name
		\return
			Const reference to the name of this component
		*/
		/**************************************************************************/
		const std::string& Name() const
		{
			return name;
		}
		/***************************************************************************/
		/*!
		\brief
			Name function for scripts to set a new name, instantiate a different
			kind of class in Mono
		\param _name
			Name to change to
		*/
		/**************************************************************************/
		void Name(const std::string& _name)
		{
			name = _name;
			instantiate();
		}
		void instantiate()
		{
			//MyEventSystem->publish(new ReflectComponentEvent(*this));
		}
		static char buffer[128];
		std::string name;
		std::unordered_map<std::string, GameObject*> fieldGameObjReferences;
		std::unordered_map<std::string, Component*> fieldComponentReferences;
		FieldMap fieldDataReferences;
	};
	
	using ButtonCallback = void (*)();

	enum class ButtonState : char
	{
		OnHover,
		OnClick,
		OnHeld,
		OnRelease,
		None,
	};

	enum class HorizontalAlignment : int
	{
		Left = 0,
		Center,
		Right,
	};

	enum class VerticalAlignment : int
	{
		Top = 0,
		Center,
		Bottom,
	};

	////Runs after InputSystem
	class IUIComponent : public Component
	{
	public:
		const glm::fvec4& get_color() { return color; }
		const HorizontalAlignment& get_hAlign() { return hAlignment; }
		const VerticalAlignment& get_vAlign() { return vAlignment; }
		Math::Vec2 offset;
		HorizontalAlignment hAlignment{ HorizontalAlignment::Center };
		VerticalAlignment vAlignment{ VerticalAlignment::Center };
		glm::fvec4 color{ 1.f };
		glm::fvec4 layeredColor{ 0.f };
	protected:
		IUIComponent(GameObject& _gameObj, UUID _uuid = UUID()) : Component(_gameObj, _uuid) {}
		IUIComponent(GameObject& _gameObj, const IUIComponent& rhs,UUID _uuid = UUID()) : 
			hAlignment{rhs.hAlignment},vAlignment{rhs.vAlignment},offset{rhs.offset},color{rhs.color},
			layeredColor{rhs.layeredColor}, Component(_gameObj, _uuid)
		{}
	};
	class Button final : public Component
	{
	//	//A screen space box collider
	public:
	//	/**************************************************************************/
	//	/*!
	//		\brief
	//			Constructs a Button
	//		\param gameObj
	//			Owner of this
	//		\param _max
	//			Max of bounding box
	//		\param _min
	//			Min of bounding box
	//	*/
	//	/**************************************************************************/
		Button(GameObject& _gameObj, UUID _uuid = UUID(), Math::Vec2 _min = { -0.5,-0.5 }, Math::Vec2 _max = { 0.5,0.5 })
			: Component(_gameObj, _uuid), bounds{ _min,_max },
			normalColor{ 1.f,1.f,1.f,0.5f }, hoverColor{ 0.5f,1.f,1.f,0.5f }, clickedColor{ 0.5f },
			targetGraphic{ nullptr }
		{
			previousColor = normalColor;
			state = ButtonState::None;
		}


		Button(GameObject& _gameObj, const Button& rhs, UUID _uuid = UUID()) :
			Component(_gameObj, _uuid), callbackName{ rhs.callbackName },
			bounds{ rhs.bounds }, normalColor{ rhs.normalColor }, 
			hoverColor{ rhs.hoverColor }, clickedColor{ rhs.clickedColor },
			fadeDuration{ rhs.fadeDuration}, targetGraphic{rhs.targetGraphic}
		{
			PRINT(size_t(rhs.uuid));
			PRINT(_uuid.GetUUID());
			PRINT("BUTTON COPY CONSTRUCTED!");
		}

		Button& operator=(const Button& rhs);

		std::string callbackName;
		AABB bounds;
		ButtonState state{ButtonState::None};
		glm::fvec4 normalColor;
		glm::fvec4 hoverColor;
		glm::fvec4 clickedColor;
		IUIComponent* targetGraphic;
		ButtonState previousState{ ButtonState::None };
		glm::fvec4 previousColor;
		float timer{ 0 };
		float fadeDuration{ 0.1f };
	};


	class Text final : public IUIComponent
	{
	public:
		char content[TEXT_BUFFER_SIZE];
		/**************************************************************************/
		/*!
			\brief
				Constructs a Text
			\param gameObj
				Owner of this
		*/
		/**************************************************************************/
		Text(GameObject& _gameObj, UUID _uuid = UUID(), bool _inspector = false);

		Text(GameObject& _gameObj, const Text& rhs, UUID _uuid = UUID()) :
			IUIComponent(_gameObj, rhs ,_uuid), fontName{ rhs.fontName }, font{ rhs.font },
			fSize{rhs.fSize},wrapper{rhs.wrapper}
		{strcpy(content, rhs.content); }
		/*******************************************************************************
		/*!
		*
		\brief
			Called by graphics to display this
		*/
		/*******************************************************************************/
		void render(BaseCamera* _camera);

		std::string fontName;
		Font* font;
		float fSize;
		float wrapper;
		friend class Button;
	};

	class SpriteRenderer final : public Component
	{
	public:
		SpriteRenderer(GameObject& _gameObj, UUID _uuid = UUID()) : Component(_gameObj, _uuid){}
		SpriteRenderer(GameObject& _gameObj, const SpriteRenderer& rhs, UUID _uuid = UUID()) :
			Component(_gameObj, _uuid), sprite{rhs.sprite}
		{}
		/* Sprite Information ***********************************************************/
		Sprite sprite{};
	};

	class SortingGroup final : public Component
	{
	public:
		/*******************************************************************************
		/*!
		*
		\brief
			Constructs the component of the sorting group for this gameobject
		\param _gameObj
			The gameoject to add into the sorting group
		*/
		/*******************************************************************************/
		//SortingGroup(GameObject& _gameObj, UUID _uuid = UUID()) :
		//	Component(_gameObj, _uuid), sortingLayer{ 0 }, orderInLayer{ 0 }
		//{
		//	//PRINT("Added to default layer");
		//	//MyEditorSystem.getLayers()->SortLayers()->AddGameObject(0, _gameObj);
		//}

		//SortingGroup(GameObject& _gameObj, UUID _uuid = UUID(), bool _add = false);

		/*******************************************************************************
		/*!
		*
		\brief
			Constructs the component of the sorting group for this gameobject
		\param _gameObj
			The gameoject to add into the sorting group
		\param _order
			The order in the specific layer
		\param _sort
			The sorting group to add the gameobject into
		*/
		/*******************************************************************************/
		SortingGroup(GameObject& _gameObj, UUID _uuid = UUID());

		/*******************************************************************************
		/*!
		*
		\brief
			Get the id of the layer this sorting group is in
		\return
			id of the layer this sorting group is in
		*/
		/*******************************************************************************/
		int GetLayerID() const { return sortingLayer; }
		/*******************************************************************************
		/*!
		*
		\brief
			Get the gameobject's order in layer
		\return
			this gameobject's order in layer
		*/
		/*******************************************************************************/
		int GetOrderInLayer() const { return orderInLayer; }
		SortingGroup(GameObject& _gameObj, const SortingGroup& rhs, UUID _uuid = UUID()) :
			Component(_gameObj, _uuid), sortingLayer{ rhs.sortingLayer }, orderInLayer{rhs.orderInLayer}
		{}
		int sortingLayer;
		int orderInLayer;
	};

	class Image final : public IUIComponent
	{
	public:
		/**************************************************************************/
		/*!
			\brief
				Constructs a Image
			\param gameObj
				Owner of this
		*/
		/**************************************************************************/
		Image(GameObject& _gameObj, UUID _uuid = UUID()) : IUIComponent(_gameObj, _uuid){}

		Image(GameObject& _gameObj, const Image& rhs, UUID _uuid = UUID()) :
			IUIComponent(_gameObj, rhs, _uuid), sprite{ rhs.sprite }{};

		Image& operator=(const Image& rhs) { return *this; }
		/*******************************************************************************
		/*!
		*
		\brief
			Called by graphics to display this
		*/
		/*******************************************************************************/
		Math::Vec2 Offset();
		Sprite sprite;
	};

	template <typename T>
	struct GetComponentType {};

	RegisterComponent(Transform);
	RegisterComponent(BoxCollider2D);
	RegisterComponent(SpriteRenderer);
	RegisterComponent(Rigidbody2D);
	RegisterComponent(Camera);
	RegisterComponent(Animator);
	RegisterComponent(SortingGroup);
	RegisterComponent(AudioSource);
	RegisterComponent(Script);
	RegisterComponent(Image);
	RegisterComponent(Text);
	RegisterComponent(Button);
	using ComponentTypes = TemplatePack<SortingGroup, SpriteRenderer,BoxCollider2D, Rigidbody2D, Animator,Camera, AudioSource, Image, Text, Button, Script>;
	using ComponentsArrays = decltype(ComponentGroup(ComponentTypes()));
	using ComponentsPtrArrays = decltype(ComponentPtrGroup(ComponentTypes()));

}


#endif // !COMPONENTS_H
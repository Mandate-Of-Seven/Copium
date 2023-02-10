#pragma once

#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <Physics/collision.h>
#include <Utilities/sparse-set.h>
#include <config.h>
#include <Graphics/textures.h>
#include <glm/gtx/quaternion.hpp>
#include <Graphics/graphics-draw.h>
#include <Graphics/framebuffer.h>
#define MAX_ANIMATION_COUNT 5


namespace Copium
{
	using ComponentID = size_t;
	class Draw;

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
	struct Pack {};

	using ComponentsBitset = std::bitset<MAX_ENTITIES>;

	template <typename Component>
	class ComponentsArray
	{
		ComponentsBitset componentsBitset{};
		SparseSet<Component, MAX_ENTITIES> components;
		friend class EntityComponentSystem;
	public:
		void SetEnabled(EntityID entityID, bool val)
		{
			componentsBitset.set(entityID, val);
		}

		bool GetEnabled(EntityID entityID) const
		{
			return componentsBitset.test(entityID);
		}

		EntityID GetID(const Component& component)
		{
			return(&component - &components.DenseGet(0));
		}

		size_t GetSize() const
		{
			return components.GetSize();
		}

		Component& FindByID(EntityID entityID)
		{
			return components.DenseGet(entityID);
		}

		Component& operator[](size_t index)
		{
			return components[index];
		}
	};

	template<typename Component, typename... Components>
	struct ComponentGroup
	{
		using Types = Pack<Component,Components...>;

		ComponentsArray<Component> components;
		ComponentGroup<Components...> others;

		template <typename T>
		static constexpr bool Has()
		{
			return contains<T,Component, Components...>();
		}

		template <typename T>
		constexpr ComponentsArray<T>& GetArray()
		{
			if constexpr (std::is_same<T, Component>())
			{
				return components;
			}
			else
			{
				return others.GetArray<T>();
			}
		}
	};

	template<typename Component>
	struct ComponentGroup <Component>
	{
		ComponentsArray<Component> components;
		ComponentsBitset componentsBitset;

		template <typename T>
		constexpr ComponentsArray<T>& GetArray()
		{
			return components;
		}
	};


	struct Transform
	{
	public:
		Transform() = default;

		EntityID parentID {MAX_ENTITIES};
		std::vector<EntityID> childrenIDs;
		Math::Vec3 position{};
		Math::Vec3 rotation{};
		Math::Vec3 scale{1,1,1};
		bool HasParent() const{ return parentID < MAX_ENTITIES;}
	};

	struct Rigidbody2D
	{
		Rigidbody2D() = default;
		Math::Vec2 velocity;					//velocity of object
		Math::Vec2 acceleration;				//acceleration of object
		Math::Vec2 force;						//forces acting on object
		float mass{1.f};								//mass of object
		bool useGravity{true};					//is object affected by gravity?
	};

	struct BoxCollider2D
	{
		BoxCollider2D() = default;
		AABB bounds{};
	};

	struct SpriteRenderer
	{
		SpriteRenderer() = default;
		glm::bvec2 flip{ 0 };
		glm::vec3 position{ 0 };	// Temporary variable to access the position
		glm::vec3 rotation{ 0 };	// Temporary variable for the rotation of the sprite
		glm::vec2 size{ 0 };		// The size of the sprite in pixels ( Bean: different from the scale of the gameobject )
		glm::vec4 color{ 1 };		// The blended color of this sprite
		unsigned int spriteID = 0; // The index of the sprite
		const Texture* refTexture; // Pointer to the texture
		bool isAddingSprite;
	};

	// Base camera which contains data
	struct Camera
	{
	public:
		void init(float _width, float _height, bool _orthographic)
		{
			// Setting up data
			viewer = { 0.f, 0.f, -10.f };
			focalPoint = { viewer.x, viewer.y, 0.f };
			/*viewer = { 0.f, 0.f, 0.f };
			focalPoint = { viewer.x, viewer.y, 0.f };*/
			upVector = { 0.f, 1.f, 0.f };

			width = _width;
			height = _height;

			//pitch = 0.f;
			//yaw = 0.f;

			// Setting up properties
			backgroundColor = { 0.278f, 0.278f, 0.278f, 1.f };
			orthographic = _orthographic;

			// Setting up projection data
			aspect = width / height;

			if (orthographic)
			{
				fieldOfView = 0.f;
				orthographicSize = 5.f;
			}
			else
			{
				fieldOfView = 60.f;
				orthographicSize = 0.f;
			}

			// Calculating projection
			viewMatrix = glm::lookAt(viewer, focalPoint, upVector);
			update_ortho_projection();
			//projMatrix = glm::ortho(width, height, nearClip, farClip);
			//viewProjMatrix = projMatrix * viewMatrix;

			// Initialise Sub systems
			framebuffer.set_size((unsigned int)_width, (unsigned int)_height);
			framebuffer.Init();
			draw.init(this);
		}

		void Update()
		{
			update_view_matrix();

			draw_camera();
		}

		void draw_camera()
		{
			//if (cameraType == CameraType::GAME && !editorSystem.is_enabled())
			//{
			//	draw.update(cameraType);
			//}
			//else
			//{
				framebuffer.bind();

				draw.update(SCENEVIEW);

				framebuffer.unbind();
			//}
		}

		void Exit()
		{
			framebuffer.Exit();
			draw.Exit();
		}

		glm::vec3 get_right_direction() const
		{
			return glm::rotate(get_orientation(), glm::vec3(1.f, 0.f, 0.f));
		}

		glm::vec3 get_up_direction() const
		{
			return glm::rotate(get_orientation(), glm::vec3(0.f, 1.f, 0.f));
		}

		glm::vec3 get_forward_direction() const
		{
			return glm::rotate(get_orientation(), glm::vec3(0.f, 0.f, -1.f));
		}

		glm::quat get_orientation() const
		{
			return glm::quat(glm::vec3(-pitch, -yaw, 0.f));
		}

		glm::vec2 get_pan_speed() const
		{
			glm::vec2 speed;
			float x = std::min(width / 1000.f, 2.4f); // Max speed is 2.4
			speed.x = 0.04f * (x * x) - 0.1778f * x + 0.3f;
			float y = std::min(height / 1000.f, 2.4f); // Max speed is 2.4
			speed.y = 0.04f * (y * y) - 0.1778f * y + 0.3f;
			return speed;
		}

		void on_resize(float _width, float _height)
		{
			width = _width;
			height = _height;
			aspect = width / height;

			framebuffer.resize((GLuint)width, (GLuint)height);
			update_ortho_projection();
		}

		void update_ortho_projection()
		{
			float ar = aspect;
			float zl = orthographicSize;
			projMatrix = glm::ortho(-ar * zl, ar * zl, -zl, zl, nearClip, farClip);
			viewProjMatrix = projMatrix * viewMatrix;
		}

		void update_ortho_projection(float _left, float _right, float _bottom, float _top)
		{
			projMatrix = glm::ortho(_left, _right, _bottom, _top, nearClip, farClip);
			viewProjMatrix = projMatrix * viewMatrix;
		}

		void update_view_matrix()
		{
			// Get updated viewer / eye location
			viewer = calculate_position();
			focalPoint = { viewer.x, viewer.y, 0.f };
			upVector = { 0.f, 1.f, 0.f };

			// Update view matrix
			viewMatrix = glm::lookAt(viewer, focalPoint, upVector);

			// Update view projection matrix
			viewProjMatrix = projMatrix * viewMatrix;
			//viewProjMatrix = viewMatrix * projMatrix;
		}

		glm::vec3 calculate_position()
		{
			return focalPoint - get_forward_direction() * orthographicSize;
		}

		/* Camera Data ******************************************************************/
		glm::vec3 viewer;					// The location of the viewer / eye (Center of the screen, 10 units away)
		glm::vec3 focalPoint;				// The look-at point / target point where the viewer is looking (Center of screen)
		glm::vec3 upVector;					// Defines which way is up (Default is Y-direction)
		glm::vec2 dimension;				// The dimension of the camera in width and height defined in pixels

		float width = 0.f;					// Pixel width of the camera
		float height = 0.f;					// Pixel height of the camera
		float pitch = 0.f;					// For rotation
		float yaw = 0.f;					// For rotation

		/* Camera Properties ************************************************************/
		glm::vec4 backgroundColor;			// Default solid color when rendering
		bool cullingMask = false;			// Bean: A culling mask to prevent rendering of specific layers, to be implemented in the future
		bool orthographic = true;			// Orthographic by default until perspective camera has been implemented
		bool useOcclusionCulling = false;	// Bean: A feature to be implemented in the future

		/* Projection Data **************************************************************/
		float aspect = 0.f;					// The aspect ratio of the camera in width/height (Automatically calculated by screen's aspect ratio)
		float nearClip{ 0.3f };				// Distance of near clipping plane from the camera
		float farClip {500.f};				// Distance of far clipping plane from the camera
		float fieldOfView = 0.f;			// The vertical field of view in degrees
		float orthographicSize = 0.f;		// Half-size of camera in orthographic mode

		/* Matrices & Projection ********************************************************/
		glm::mat4 projMatrix{ 0 };			// The projection matrix to use, either orthographic or perspective
		glm::mat4 viewMatrix{ 0 };			// The view matrix -> worldToCamera matrix
		glm::mat4 viewProjMatrix{ 0 };		// The view projection matrix -> viewMatrix * projMatrix

		glm::mat4 cameraToNDC{ 0 };
		glm::mat4 ndcToViewport{ 0 };

		/* Stored Information ***********************************************************/
		Framebuffer framebuffer;
		Draw draw;
	};

	struct Frame
	{
		Math::Vec2 top;		// Top-Left UV of the sprite
		Math::Vec2 bottom;	// Bottom-Right UV of the sprite
		float timeDelay;	// How long the frame is played
	};

	struct Animation
	{
		Spritesheet spriteSheet;
		float timer, timeDelay;
		int frameCount, currentFrameIndex;

		bool UpdateFrame(float _dt)
		{
			timer -= _dt;
			std::cout << "frame updated ";
			if (timer <= 0.f)
			{
				timer = 0.f;
				return true;
			}
			else
			{
				return false;
			}
		}
		void IncrementFrame()
		{
			if (frameCount <= 0)
				return;

			// Perform wrap-around if incrementing from the last frame
			if (currentFrameIndex == frameCount - 1)
				currentFrameIndex = 0;
			else
				++currentFrameIndex;

			std::cout << "frame incremented \n";
			timer = timeDelay;
		}
		void ResetFrame()
		{
			currentFrameIndex = 0;
		}

	};

	struct Animator
	{
	public:
		enum class AnimatorStatus : char
		{
			idle = 0,
			playing
		};

		bool IsEmpty() const { return animations.empty(); }

		void AddAnimation()
		{
			if (animationCount == MAX_ANIMATION_COUNT)
			{
				PRINT("Maximum number of animations for this Animator has been reached!");
				return;
			}

			animations.push_back(Animation());
			++animationCount;

			currentAnimationIndex = animations.size() - 1;

		}

		void PlayAnimation()
		{

		}

		Animation* GetCurrentAnimation()
		{
			if (IsEmpty())
				return nullptr;
			return &animations[currentAnimationIndex];
		}
		std::vector<Animation> animations;    // The indices of the animations inside the assets-system
		int currentAnimationIndex;      // Current playing animation
		int startingAnimationIndex;     // The first animation that is playing
		unsigned int animationCount;
		bool loop;
		AnimatorStatus status;
	};

	using AllComponents = ComponentGroup<Transform, BoxCollider2D, Rigidbody2D, SpriteRenderer, Camera, Animator>;

	#define RegisterComponent(Type)template <> struct GetComponentType<Type>{static constexpr size_t e{ (size_t)ComponentType::Type }; };

	template <typename T>
	struct GetComponentType{};

	RegisterComponent(Transform);
	RegisterComponent(BoxCollider2D);
	RegisterComponent(SpriteRenderer);
	RegisterComponent(Rigidbody2D);
	RegisterComponent(Camera);
	RegisterComponent(Animator);
}


#endif // !COMPONENTS_H
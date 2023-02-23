#include <pch.h>
#include <Events/events-system.h>
#include <GameObject/components.h>
#include <Editor/editor-system.h>
#include <Graphics/fonts.h>
#include "glm/gtc/matrix_transform.hpp"

namespace Copium
{
	void Transform::SetParent(Transform* _parent)
	{
		//Set Parent;
		//Previously had a parent
		if (parent)
			parent->children.remove(this);
		parent = _parent;
		//_parent might be nullptr
		if (_parent)
			_parent->children.push_back(this);
	}

	Transform::Transform(GameObject& _gameObject, const Transform& rhs) : gameObject{_gameObject}
	{
		position = rhs.position;
		rotation = rhs.rotation;
		scale = rhs.scale;
	}

	Math::Vec3 Transform::GetWorldPosition() const
	{
		Math::Vec3 _position = position;
		Math::Vec3 _rotation = rotation;
		Math::Vec3 _scale = scale;

		Transform* _parent = parent;
		while (_parent)
		{
			Math::Vec3 parentRot = _parent->rotation;
			Math::Vec3 parentScale = _parent->scale;
			_position = _parent->position + parentRot * (_position * parentScale);
			_rotation += parentRot;
			_scale *= parentScale;
			_parent = _parent->parent;
		}

		return position;
	}
	Math::Vec3 Transform::GetWorldRotation() const
	{
		Math::Vec3 _rotation = rotation;

		Transform* _parent = parent;
		while (_parent)
		{
			_rotation += _parent->rotation;
			_parent = _parent->parent;
		}
		return _rotation;
	}
	Math::Vec3 Transform::GetWorldScale() const
	{
		Math::Vec3 _scale = scale;
		Transform* _parent = parent;
		while (_parent)
		{
			_scale *= _parent->scale;
			_parent = _parent->parent;
		}
		return _scale;
	}

	char Script::buffer[128];

	void Animator::Update(double _dt)
	{
		if (animations.empty())
			return;
		if (status != AnimatorStatus::playing)
			return;
		if (!loop && animations[currentAnimationIndex].status == Animation::AnimationStatus::completed)
		{
			PRINT("anim completed");
			status = AnimatorStatus::idle;
			return;
		}
		if (animations[currentAnimationIndex].UpdateFrame(_dt))
		{
			if (reverse)
				animations[currentAnimationIndex].DecrementFrame();
			else
				animations[currentAnimationIndex].IncrementFrame();
		}
	}

	void Animator::AddAnimation()
	{
		if (animations.size() == MAX_ANIMATION_COUNT)
		{
			PRINT("Maximum number of animations for this Animator has been reached!");
			return;
		}
		animations.push_back(Animation());
		currentAnimationIndex = (int)(animations.size() - 1);
	}

	Math::Vec2 Image::Offset()
	{
		Transform& trans{ gameObj.transform };
		Math::Vec2 pos{ trans.position };
		Math::Vec2 dimensions{ (float)sprite.refTexture->get_width(),(float)sprite.refTexture->get_height() };
		if (dimensions.x == 0)
			dimensions.x = 1.f;
		if (dimensions.y == 0)
			dimensions.y = 1.f;
		dimensions.x *= trans.scale.x;
		dimensions.y *= trans.scale.y;
		switch (hAlignment)
		{
		case HorizontalAlignment::Left:
		{
			pos.x += dimensions.x / 2.f;
			break;
		}
		case HorizontalAlignment::Right:
		{
			pos.x -= dimensions.x / 2.f;
			break;
		}
		}
		switch (vAlignment)
		{
		case VerticalAlignment::Top:
		{
			pos.y -= dimensions.y / 2.f;
			break;
		}
		case VerticalAlignment::Bottom:
		{
			pos.y += dimensions.y / 2.f;
			break;
		}
		}
		return pos;
	}

	Button& Button::operator=(const Button& rhs)
	{
		bounds = rhs.bounds;
		normalColor = rhs.normalColor;
		hoverColor = rhs.hoverColor;
		clickedColor = rhs.clickedColor;
		float timer{ 0 };
		float fadeDuration{ 0.1f };
		return *this;
	}

	//SortingGroup::SortingGroup(GameObject& _gameObj, UUID _uuid, bool _add) :Component(_gameObj, _uuid)
	//{
	//	PRINT("default sorting group ctor called");
	//}

	
	SortingGroup::SortingGroup(GameObject& _gameObj, UUID _uuid) :Component(_gameObj, _uuid)
	{
	}

	Text::Text(GameObject& _gameObj, UUID _uuid) : IUIComponent(_gameObj, _uuid), fSize{ 1.f }, wrapper{ 0.f }, content{ "New Text" }, fontName{"corbel"}
	{
		font = Font::getFont(fontName);
	}

	void Text::render(BaseCamera* _camera)
	{
		if (!font)
			return;
		Transform& trans{ gameObj.transform };
		Math::Vec3 globalPos{ trans.GetWorldPosition()};
		Math::Vec3 globalScale{trans.GetWorldScale()};
		float scale = globalScale.x * 0.1f;
		if (scale > globalScale.y * 0.1f)
			scale = globalScale.y * 0.1f;
		scale *= fSize;

		glm::fvec4 mixedColor{ 0 };
		mixedColor.a = 1 - (1 - layeredColor.a) * (1 - color.a); // 0.75
		if (mixedColor.a < 0.01f)
			return;
		mixedColor.r = layeredColor.r * layeredColor.a / mixedColor.a + color.r * color.a * (1 - layeredColor.a) / mixedColor.a; // 0.67
		mixedColor.g = layeredColor.g * layeredColor.a / mixedColor.a + color.g * color.a * (1 - layeredColor.a) / mixedColor.a; // 0.33
		mixedColor.b = layeredColor.b * layeredColor.a / mixedColor.a + color.b * color.a * (1 - layeredColor.a) / mixedColor.a; // 0.00

		/*PRINT("Color: " << color.r << " " << color.g << " " << color.b << " " << color.a);
		PRINT("Mixed Color: " << mixedColor.r << " " << mixedColor.g << " " << mixedColor.b << " " << mixedColor.a);
		*/

		glm::vec2 dimensions{ font->getDimensions(content, scale, wrapper) };

		if (trans.HasParent())
		{
			glm::vec3 updatedPos = trans.position;
			glm::vec3 updatedScale = trans.scale;

			Transform* tempObj = trans.parent;
			while (tempObj)
			{
				glm::vec3 tempPos = tempObj->position.glmVec3;
				glm::mat4 translate = glm::translate(glm::mat4(1.f), tempPos);

				float rot = glm::radians(tempObj->rotation.z);
				glm::mat4 lRotate = {
				glm::vec4(cos(rot), sin(rot), 0.f, 0.f),
				glm::vec4(-sin(rot), cos(rot), 0.f, 0.f),
				glm::vec4(0.f, 0.f, 1.f, 0.f),
				glm::vec4(0.f, 0.f, 0.f, 1.f)
				};

				glm::vec3 size = tempObj->scale.glmVec3;
				glm::mat4 lScale = {
					glm::vec4(size.x, 0.f, 0.f, 0.f),
					glm::vec4(0.f, size.y, 0.f, 0.f),
					glm::vec4(0.f, 0.f, 1.f, 0.f),
					glm::vec4(0.f, 0.f, 0.f, 1.f)
				};

				glm::mat4 transform = translate * lRotate * lScale;

				updatedPos = glm::vec3(transform * glm::vec4(updatedPos, 1.f));
				updatedScale *= tempObj->scale.glmVec3;
				tempObj = tempObj->parent;
			}

			float updatedSize = updatedScale.x * fSize * 0.1f;

			glm::vec2 dimensions{ font->getDimensions(content, updatedSize, wrapper) };

			switch (hAlignment)
			{
			case HorizontalAlignment::Center:
				updatedPos.x -= dimensions.x / 2.f;
				break;
			case HorizontalAlignment::Right:
				updatedPos.x -= dimensions.x;
				break;
			}
			switch (vAlignment)
			{
			case VerticalAlignment::Top:
				updatedPos.y -= dimensions.y;
				break;
			case VerticalAlignment::Center:
				updatedPos.y -= dimensions.y / 2.f;
				break;
			}

			font->draw_text(content, updatedPos, mixedColor, updatedSize, wrapper, _camera);
		}
		else
		{
			glm::vec2 dimensions{ font->getDimensions(content, scale, wrapper) };

			switch (hAlignment)
			{
			case HorizontalAlignment::Center:
				globalPos.x -= dimensions.x / 2.f;
				break;
			case HorizontalAlignment::Right:
				globalPos.x -= dimensions.x;
				break;
			}
			switch (vAlignment)
			{
			case VerticalAlignment::Top:
				globalPos.y -= dimensions.y;
				break;
			case VerticalAlignment::Center:
				globalPos.y -= dimensions.y / 2.f;
				break;
			}

			font->draw_text(content, globalPos, mixedColor, scale, wrapper, _camera);
		}
	}
}
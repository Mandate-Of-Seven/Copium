#include <pch.h>
#include <Events/events-system.h>
#include <GameObject/components.h>
#include <Editor/editor-system.h>

namespace Copium
{
	void Transform::SetParent(Transform* _parent)
	{
		//Previously had a parent
		if (parent)
			parent->children.remove(this);
		parent = _parent;
		//_parent might be nullptr
		if (_parent)
			_parent->children.push_back(this);
	}

	Transform& Transform::operator=(const Transform& rhs)
	{
		parent = rhs.parent;
		position = rhs.position;
		rotation = rhs.rotation;
		scale = rhs.scale;
		for (Transform* pTransform : rhs.children)
		{
			//Create Child GameObject
		}
		return *this;
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
	std::pair<const std::string, Field>* Script::editedField;
	bool Script::isAddingReference{ nullptr };

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
	
	SortingGroup::SortingGroup(GameObject& _gameObj, int _order, int _sort, bool _replace) :Component(_gameObj), sortingLayer{ _sort }, orderInLayer{ _order }
	{
		PRINT("Added to specific layer");
		if (_replace)
			MyEditorSystem.getLayers()->SortLayers()->ReplaceGameObject(sortingLayer, _gameObj);
		else
			MyEditorSystem.getLayers()->SortLayers()->AddGameObject(sortingLayer, _gameObj);
	}

}
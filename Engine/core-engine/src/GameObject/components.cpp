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
			_position += _parent->position + parentRot * (_position * parentScale);
			_rotation += parentRot;
			_scale *= parentScale;
			_parent = _parent->parent;
		}

		return _position;
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

	Text::Text(GameObject& _gameObj, UUID _uuid, bool _inspector) : IUIComponent(_gameObj, _uuid), fSize{ 1.f }, wrapper{ 0.f }, content{ "New Text" }
	{
		if (_inspector)
		{
			fontName = "corbel";
			font = Font::getFont(fontName);
		}
	}

	void AudioSource::play_sound()
	{
		if (channel == "Default")
		{
			MySoundSystem.Play(alias, MySoundSystem.channelDefault, overLap, loop);
		}
		else if (channel == "BGM")
		{
			MySoundSystem.Play(alias, MySoundSystem.channelBGM, overLap, loop);
		}
		else if (channel == "SFX")
		{
			MySoundSystem.Play(alias, MySoundSystem.channelSFX, overLap, loop);
		}
		else if (channel == "Voice")
		{
			MySoundSystem.Play(alias, MySoundSystem.channelVoice, overLap, loop);
		}
		else if (true)
		{
			PRINT("No channel detected, Playing on default");
			MySoundSystem.Play(alias, MySoundSystem.channelDefault, overLap, loop);
		}
	}
	void AudioSource::stop_sound()
	{
		MySoundSystem.Stop(this->alias);
	}
}
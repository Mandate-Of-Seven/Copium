/*!***************************************************************************************
\file			animation-system.cpp
\project
\author			Sean Ngo
\co-authors		Matthew Lau
				Shawn Tanary

\par			Course: GAM200
\par			Section:
\date			05/01/2023

\brief
	This file holds the definition of the Animation system. The Animation system animates
	the spritesheets in the engine, where the user has to create the animation in the
	animator in the editor.

	The definitions of functions from the Animator Component are here as well.

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"
#include "Animation/animation-system.h"
#include "SceneManager/scene-manager.h"
#include "Files/assets-system.h"
#include "Debugging/frame-rate-controller.h"
#include "Files/assets-system.h"

#define MAX_ANIMATION_COUNT 5

namespace Copium
{

	Animator::Animator(GameObject& _gameObj) 
		: Component(_gameObj, ComponentType::Animator), currentAnimationIndex{ 0 }, startingAnimationIndex{ 0 }, animationCount{ 0 }, loop{ true }, reverse{ false }, status{ AnimatorStatus::idle }
	{

	}
	void Animator::inspector_view()
	{
		std::string toggleAnimation = "Play";
		if (status == AnimatorStatus::playing)
			toggleAnimation = "Pause";
		else if (status == AnimatorStatus::paused)
			toggleAnimation = "Resume";

		//ImGuiColorEditFlags miscFlags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip
		//	| ImGuiColorEditFlags_NoLabel;

		ImGuiTableFlags tableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody
			| ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_SizingStretchProp;
		if (ImGui::BeginTable("Component Animator", 2, tableFlags))
		{
			ImGui::Indent();

			ImGui::TableSetupColumn("Text", 0, 0.4f);
			ImGui::TableSetupColumn("Input", 0, 0.6f);

			// Animation Looping
			ImGui::PushID(0);
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Loop");
			ImGui::TableNextColumn();
			//ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			if (ImGui::Checkbox("", &loop))
			{
				for (Animation& anim : animations)
				{
					anim.loop = loop;
				}
			}
			ImGui::PopID();
			//ImGui::PopItemFlag();

			ImGui::PushID(1);
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Reverse");
			ImGui::TableNextColumn();
			//ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
			if (ImGui::Checkbox("", &reverse))
				PRINT("clicking reverse");
			ImGui::PopID();
			//ImGui::PopItemFlag();


			// Play / Pause the animation
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Play Animation");

			ImGui::TableNextColumn();
			if (ImGui::Button(toggleAnimation.c_str(), ImVec2(ImGui::GetColumnWidth() * 0.3f, 0.f)))
			{

				if (status == AnimatorStatus::idle)
				{
					status = AnimatorStatus::playing;
					Animation* currAnim = GetCurrentAnimation();
					if (currAnim)
						currAnim->status = Animation::AnimationStatus::playing;

					PRINT("playing");
				}
				else if (status == AnimatorStatus::paused)
				{
					status = AnimatorStatus::playing;
					Animation* currAnim = GetCurrentAnimation();
					if (currAnim)
						currAnim->status = Animation::AnimationStatus::playing;
				}
				else
				{
					PauseAnimation();
				}

			}

			if (status == AnimatorStatus::playing || status == AnimatorStatus::paused)
			{
				if (ImGui::Button("Stop Animation"))
				{
					Animation* anim{ nullptr };
					if (GetCurrentAnimation())
						anim = GetCurrentAnimation();

					PauseAnimation();
					if (reverse)
					{
						anim = GetCurrentAnimation();
						if (anim)
						{
							anim->currentFrameIndex = anim->frameCount - 1;
							anim->columnIndex = anim->columns - 1;
							anim->rowIndex = anim->rows - 1;
							if (!anim->loop)
							{
								anim->status = Animation::AnimationStatus::completed;
							}
						}

					}
					else
					{
						anim->ResetFrame();
					}
					status = AnimatorStatus::idle;
				}
			}

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Number of Animations:");
			ImGui::TableNextColumn();
			ImGui::Text("%d", animationCount);

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			if (ImGui::Button("Add Animation"))
			{
				AddAnimation();
			}

			// For each animation display appropriate things
			for (int i{ 0 }; i < animations.size(); ++i)
			{
				ImGui::PushID(i);
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Animation %d", i + 1);

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Number of Frames:");
				ImGui::TableNextColumn();
				if (ImGui::DragInt("", &animations[i].frameCount, 1))
				{
					animations[i].frameCount = animations[i].frameCount < 0 ? 0 : animations[i].frameCount;
				}
				ImGui::PopID();

				ImGui::PushID(i+1);
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Number of Columns:");
				ImGui::TableNextColumn();
				if (ImGui::DragInt("", &animations[i].spriteSheet.columns, 1))
				{
					animations[i].spriteSheet.columns = animations[i].spriteSheet.columns < 0 ? 0 : animations[i].spriteSheet.columns;
				}
				ImGui::PopID();

				ImGui::PushID(i+2);
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Number of Rows:");
				ImGui::TableNextColumn();
				if (ImGui::DragInt("", &animations[i].spriteSheet.rows, 1))
				{
					animations[i].spriteSheet.rows = animations[i].spriteSheet.rows < 0 ? 0 : animations[i].spriteSheet.rows;
				}
				ImGui::PopID();

				ImGui::PushID(i + 3);
				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Time Delay:");
				ImGui::TableNextColumn();
				if(ImGui::InputDouble("", &animations[i].timeDelay, 0.001f, 0.01f, "%.3f"))
				{
					if (animations[i].timeDelay <= 0.0)
					{
						animations[i].timeDelay = 0.0;
					}
				}
				ImGui::PopID();
				

				ImGui::TableNextRow();
				ImGui::TableNextColumn();
				ImGui::Text("Sprite");
				ImGui::TableNextColumn();
				ImGui::Button(animations[i].spriteSheet.name.c_str(), ImVec2(-FLT_MIN, 0.f));
				if (ImGui::BeginDragDropTarget())
				{
					uint64_t spriteID{0};
					std::string spriteName;
					if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentBrowserItem"))
					{
						std::string str = (const char*)(payload->Data);
						for (int j = 0; j < MyAssetSystem.GetTextures().size(); j++)
						{
							if (!MyAssetSystem.GetTexture(j)->get_file_path().compare(str))
							{
								uint64_t pathID = std::hash<std::string>{}(MyAssetSystem.GetTexture(j)->get_file_path());
								MetaID metaID = MyAssetSystem.GetMetaID(pathID);
								spriteID = metaID.uuid;

								// Attach Reference
								animations[i].spriteSheet.texture = MyAssetSystem.GetTexture(j);
							}
						}
						size_t pos = str.find_last_of('\\');
						spriteName = str.substr(pos + 1, str.length() - pos);
					}
					ImGui::EndDragDropTarget();

					if (spriteID >= 0)
					{
						animations[i].spriteSheet.spriteID = spriteID;
						animations[i].spriteSheet.name = spriteName;
					}
				}
			}

			ImGui::Unindent();
			ImGui::EndTable();
		}
	}
	void Animator::AddAnimation()
	{
		if (animationCount == MAX_ANIMATION_COUNT)
		{
			PRINT("Maximum number of animations for this Animator has been reached!");
			return;
		}

		animations.push_back(Animation());
		++animationCount;

		currentAnimationIndex = (int)(animations.size() - 1);

	}
	void Animator::PlayAnimation()
	{
		status = AnimatorStatus::playing;
	}
	void Animator::PauseAnimation()
	{
		status = AnimatorStatus::paused;		
	}
	

	void Animator::Update(double _dt)
	{

		if (!animationCount)
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
			if(reverse)
				animations[currentAnimationIndex].DecrementFrame();
			else
				animations[currentAnimationIndex].IncrementFrame();				
		}




	}
	Animation* Animator::GetCurrentAnimation()
	{
		if (IsEmpty())
			return nullptr;

		return &animations[currentAnimationIndex];
	}
	void Animator::deserialize(rapidjson::Value& _value)
	{
		Component::deserialize(_value);
		if (_value.HasMember("Loop"))
			loop = _value["Loop"].GetBool();
		else
			loop = true;

		if (_value.HasMember("Rev"))
			reverse = _value["Rev"].GetBool();
		else
			reverse = false;

		if (_value.HasMember("Count"))
			animationCount = _value["Count"].GetUint();

		if (_value.HasMember("Animations"))
		{
			rapidjson::Value& animArray = _value["Animations"].GetArray();
			int i{ 0 };
			for (rapidjson::Value::ValueIterator iter = animArray.Begin(); iter != animArray.End(); ++iter)
			{
				
				rapidjson::Value& val = *iter;
				std::string animName;
				if (val.HasMember("Name"))
					animName = val["Name"].GetString();

				uint64_t sid{ 0 };
				if (val.HasMember("Sprite ID"))
					sid = val["Sprite ID"].GetUint64();

				double td{ 0.f };
				if (val.HasMember("Time Delay"))
				{
					td = val["Time Delay"].GetDouble();
					PRINT("Time delay:" << td);
				}

				int fc{ 0 };
				if (val.HasMember("FrameCount"))
					fc = val["FrameCount"].GetInt();

				int rows{ 0 }, cols{ 0 };
				if (val.HasMember("Rows"))
					rows = val["Rows"].GetInt();

				if (val.HasMember("Columns"))
					cols = val["Columns"].GetInt();

				animations.push_back(Animation());

				if (sid != 0)
				{
					std::vector<Texture> textures = MyAssetSystem.GetTextures();
					bool reference = false;
					for (int j = 0; j < textures.size(); j++)
					{
						uint64_t pathID = std::hash<std::string>{}(textures[j].get_file_path());
						MetaID metaID = MyAssetSystem.GetMetaID(pathID);

						// Check if the uuid of the sprite is the same as the meta file
						if (metaID.uuid == sid)
						{
							// If so set the reference texture to that file
							reference = true;
							animations[i].spriteSheet.texture = MyAssetSystem.GetTexture(j);
							break;
						}
					}

					// If there is no references, set the spriteID to 0
					if (!reference)
						sid = 0;
				}

				animations[i].timeDelay = td;
				PRINT("Time delay:" << animations[i].timeDelay);
				animations[i].frameCount = fc;
				animations[i].spriteSheet.spriteID = sid;
				animations[i].spriteSheet.name = animName;
				animations[i].spriteSheet.rows = rows;
				animations[i].spriteSheet.columns = cols;
				animations[i].loop = loop;
				++i;

			


			}

		}

		currentAnimationIndex = 0;
		status = AnimatorStatus::idle;


	}
	void Animator::serialize(rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		Component::serialize(_value, _doc);
		rapidjson::Value type;
		std::string tc = MAP_COMPONENT_TYPE_NAME[componentType];
		type.SetString(tc.c_str(), (rapidjson::SizeType)tc.length(), _doc.GetAllocator());
		_value.AddMember("Type", type, _doc.GetAllocator());

		_value.AddMember("Loop", loop, _doc.GetAllocator());
		_value.AddMember("Rev", reverse, _doc.GetAllocator());
		_value.AddMember("Count", animationCount, _doc.GetAllocator());


		rapidjson::Value anims(rapidjson::kArrayType);
		for (Animation& a : animations)
		{
			rapidjson::Value anim(rapidjson::kObjectType);
			rapidjson::Value name;
			name.SetString(a.spriteSheet.name.c_str(), 
							(rapidjson::SizeType)a.spriteSheet.name.length(), _doc.GetAllocator());
			anim.AddMember("Name", name, _doc.GetAllocator());
			anim.AddMember("Sprite ID", a.spriteSheet.spriteID, _doc.GetAllocator());
			anim.AddMember("Time Delay", a.timeDelay, _doc.GetAllocator());
			anim.AddMember("FrameCount", a.frameCount, _doc.GetAllocator());
			anim.AddMember("Rows", a.spriteSheet.rows, _doc.GetAllocator());
			anim.AddMember("Columns", a.spriteSheet.columns, _doc.GetAllocator());

			anims.PushBack(anim, _doc.GetAllocator());
		}
		_value.AddMember("Animations", anims, _doc.GetAllocator());
	}

	void AnimationSystem::init()
	{
		systemFlags |= FLAG_RUN_ON_PLAY | FLAG_RUN_ON_EDITOR;
	}
	void AnimationSystem::update()
	{

		if (!MySceneManager.get_current_scene())
			return;

		for (Copium::GameObject* go : MySceneManager.get_current_scene()->gameObjects)
		{
			for (Component* component : go->getComponents<Animator>()) {

				Animator* anim = reinterpret_cast<Animator*>(component);
				anim->Update(MyFrameRateController.getDt());
				if (MySceneManager.GetSceneState() == Scene::SceneState::play && anim->GetStatus() != Animator::AnimatorStatus::paused)
				{
					anim->SetStatus(Animator::AnimatorStatus::playing);
				}
				else if (MySceneManager.GetSceneState() == Scene::SceneState::paused)
				{
					anim->SetStatus(Animator::AnimatorStatus::idle);

				}


			}

		}
	}
	void AnimationSystem::exit()
	{
	
	}

	void AnimationSystem::PauseAllAnimation()
	{
		for (Copium::GameObject* go : MySceneManager.get_current_scene()->gameObjects)
		{
			Animator* temp = go->getComponent<Animator>();
			if (temp != NULL)
			{
				temp->PauseAnimation();
			}
		}
	}

	void AnimationSystem::PlayAllAnimation()
	{
		for (Copium::GameObject* go : MySceneManager.get_current_scene()->gameObjects)
		{
			Animator* temp = go->getComponent<Animator>();
			if (temp != NULL)
			{
				temp->PlayAnimation();
			}
		}
	}
}
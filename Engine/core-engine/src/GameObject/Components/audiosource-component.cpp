/*!***************************************************************************************
\file			audiosource-component.cpp
\project
\author			Shawn Tanary

\par			Course: GAM200
\par			Section:
\date			25/11/2022

\brief
	This file holds the declaration of functions for audiosource-component.cpp.

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"
#include "audiosource-component.h"
#include "Files/assets-system.h"
#include "Audio/sound-system.h"
#include <Editor/editor-consolelog.h>

namespace Copium
{
	namespace
	{
		Copium::SoundSystem& soundSystem{ *Copium::SoundSystem::Instance() };
	}

	AudioSource::AudioSource(GameObject& _gameObj) :Component(_gameObj, ComponentType::AudioSource)
	{
	}

	AudioSource& AudioSource::operator=(const AudioSource& rhs)
	{
		alias = rhs.alias;
		return *this;
	}

	void AudioSource::deserialize(rapidjson::Value& _value)
	{
		Component::deserialize(_value);
		if (_value.HasMember("Alias"))
		{
			alias = _value["Alias"].GetString();
		}
	}

	void AudioSource::serialize(rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		std::cout << "serializing audio component\n";
		Component::serialize(_value, _doc);
		rapidjson::Value type;
		std::string tc = MAP_COMPONENT_TYPE_NAME[componentType];
		type.SetString(tc.c_str(), (rapidjson::SizeType)tc.length(), _doc.GetAllocator());
		_value.AddMember("Type", type, _doc.GetAllocator());


		rapidjson::Value _alias;
		_alias.SetString(alias.c_str(), (rapidjson::SizeType)alias.length(), _doc.GetAllocator());
		_value.AddMember("Alias", _alias, _doc.GetAllocator());

	}

	void AudioSource::inspector_view()
	{
		static float f1 = 0.5f;

        ImGuiWindowFlags windowFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_NoBordersInBody
            | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_SizingStretchProp;
        if (ImGui::BeginTable("Component Transform", 2, windowFlags))
        {
			ImGui::Indent();
			// Sprite
			// Extern source file

			ImGui::TableSetupColumn("Text", 0, 0.4f);
			ImGui::TableSetupColumn("Input", 0, 0.6f);

			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			ImGui::Text("Audio File");
			ImGui::TableNextColumn();
			ImGui::Button(alias.c_str(), ImVec2(-FLT_MIN, 0.f));
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentBrowserItem"))
				{
					std::string str = (const char*)(payload->Data);
					size_t fileDot = str.find_last_of(".");
					std::string extension = str.substr(fileDot,str.size());

					if (extension == ".wav")
					{
						stop_sound();//stop any currently playing audio

						Copium::SoundSystem* soundsystem = Copium::SoundSystem::Instance();

						size_t lastSlash = str.find_last_of("\\");
						std::string temp = str.substr(lastSlash + 1);
						size_t lastDot = temp.find_last_of(".");
						alias = temp.substr(0, lastDot);
						//std::cout << "Alias: " << temp << "\n";

						if (soundsystem->soundList.find(alias) == soundsystem->soundList.end())//if its true it means file doesnt exist yet
						{
							std::cout << "New sound file detected: " << str << " / Alias (" << alias << ")\n";
							SoundSystem::Instance()->CreateSound(str, alias);
						}
						else
						{
							soundsystem->soundList[alias].first->getVolume(&f1);
						}
					}
					else
					{
						std::cout << "Wrong file type\n";
						Window::EditorConsole::editorLog.add_logEntry("AudioSource only accepts the.wav file format");
						Window::EditorConsole::editorLog.bring_to_front();
					}
					
				}
				ImGui::EndDragDropTarget();
			}

			SoundSystem::Instance()->soundList[alias].first->getVolume(&f1);
			ImGui::SliderFloat("Volume", &f1, 0.0f, 1.0f, "%.2f");
			SoundSystem::Instance()->soundList[alias].first->setVolume(f1);

			ImGui::Checkbox("Overlap", &overLap);
			ImGui::Checkbox("Loop", &loop);

			if (ImGui::Button("Preview"))
			{
				if (alias.size())
				{
					play_sound();
				}
				else
				{
					Window::EditorConsole::editorLog.add_logEntry("Error: No audio file to preview");
				}
			}
			ImGui::SameLine();
			if (ImGui::Button("Stop"))
			{
				if (alias.size())
				{
					stop_sound();
				}
				else
				{
					Window::EditorConsole::editorLog.add_logEntry("Error: No audio file to stop");
				}
			}
			

			if (ImGui::Button("Stop ALL"))
			{
				if (alias.size())
				{
					std::cout << "Stopping all audio\n";
					stop_all_sound();
				}
			}

			ImGui::Unindent();
			ImGui::EndTable();
		}

	}

	void AudioSource::play_sound()
	{
		soundSystem.Play(this->alias, overLap, loop, loopCount);
	}

	void AudioSource::stop_sound()
	{
		soundSystem.Stop(this->alias);
	}

	void AudioSource::stop_all_sound()
	{
		soundSystem.StopAll();
	}

	Component* AudioSource::clone(GameObject& _gameObj) const
	{
		auto component = new AudioSource(_gameObj);
		component->alias = alias;
		return component;
	}
}
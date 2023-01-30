/*!***************************************************************************************
\file			sound-system.cpp
\project
\author			Shawn Tanary

\par			Course: GAM200
\par			Section:
\date			24/09/2022

\brief
	This file implements basic FMOD functionalites. It contains functions to create new
	sound files and group them as well as play and stop them.

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#include "pch.h"
#include "sound-system.h"
#include "Windows/windows-system.h"
#include "Windows/windows-input.h"
#include "SceneManager/scene-manager.h"
#include "GameObject/Components/audiosource-component.h"

namespace
{
	Copium::InputSystem& inputSystem{ *Copium::InputSystem::Instance() };
}

namespace Copium
{

// Initialize sound system
void SoundSystem::init()
{
	systemFlags |= FLAG_RUN_ON_PLAY;
	FMOD::System_Create(&soundSystem);
	soundSystem->init(50, FMOD_INIT_NORMAL, NULL);
	CheckVersion();
	CheckDrivers();
	std::cout << "Sound init was called" << std::endl;
}

void SoundSystem::update()
{
	//soundSystem->update();
	//if (inputSystem.is_key_pressed(GLFW_KEY_1))
	//{
	//	Copium::SoundSystem::Instance()->Play("zap", true, false);
	//	std::cout << "Zap sound is being played\n";
	//}
	//if (inputSystem.is_key_pressed(GLFW_KEY_2))
	//{
	//	Copium::SoundSystem::Instance()->Play("reeling", true, false);
	//	std::cout << "Reeling sound is being played\n";
	//}
	//if (inputSystem.is_key_pressed(GLFW_KEY_3))
	//{
	//	Copium::SoundSystem::Instance()->Play("testbgm", false, true);
	//	std::cout << "BGM is being played\n";
	//}
}

void SoundSystem::exit()
{
	std::map<std::string, std::pair<FMOD::SoundGroup*, FMOD::Sound*>>::iterator soundIt;
	for (soundIt = soundList.begin(); soundIt != soundList.end(); soundIt++)
	{
		soundIt->second.first->release();
		soundIt->second.second->release();
		soundIt->second.first = nullptr;
		soundIt->second.second = nullptr;
	}

	soundSystem->release();
}

// Create sound
void SoundSystem::CreateSound(std::string fileName, std::string alias)
{
	FMOD::Sound *newSound;
	FMOD::SoundGroup *newGroup;
	soundSystem->createSoundGroup(alias.c_str(), &newGroup);
	soundSystem->createSound(fileName.c_str(), FMOD_DEFAULT, nullptr, &newSound);
	newSound->setSoundGroup(newGroup);

	if (soundList.count(alias))
	{
		soundList[alias].first->release();
		soundList[alias].second->release();
		soundList[alias].first = newGroup;
		soundList[alias].second = newSound;
	}
	else
	{
		soundList.insert({ alias, { newGroup, newSound } });
	}
}

void DeleteSound()
{

}
// Play sound
void SoundSystem::Play(std::string alias, bool overLap, bool loop, int loopCount)
{
	FMOD::Sound *rSound(soundList[alias].second);
	int numPlaying(0);

	// Check if overlapping sound
	if (!overLap && soundList[alias].first)
	{
		soundList[alias].first->getNumPlaying(&numPlaying);
		if (numPlaying)
		{
			return;
		}
	}
	// Check if looping
	if (loop)
	{
		rSound->setMode(FMOD_LOOP_NORMAL);
		rSound->setLoopCount(loopCount);
	}
	else
	{
		rSound->setMode(FMOD_LOOP_OFF);
	}
	soundSystem->playSound(rSound, nullptr, false, nullptr);
}

// Stop sound
void SoundSystem::Stop(std::string alias)
{
	if (soundList[alias].first)
	{
			soundList[alias].first->stop();
	}
}

void SoundSystem::StopAll()
{
	SceneManager* sm = SceneManager::Instance();
	Scene* scene = sm->get_current_scene();
	if (scene != nullptr)
	{
		//for(auto gameObj = scene->gameObjects.begin(); gameObj != scene->gameObjects.end(); ++gameObj)
		for (auto gameObj : scene->gameObjects)
		{

			AudioSource* temp = gameObj->getComponent<AudioSource>();
			if (temp != NULL)
			{
				temp->stop_sound();
			}
		}
	}
}


// Set volume
void SoundSystem::SetVolume(std::string alias, float volume)
{
	volume = (volume > 1.0f) ? 1.0f : volume;
	volume = (volume < 0.0f) ? 0.0f : volume;
	soundList[alias].first->setVolume(volume);
}

// Mute sound
void SoundSystem::Mute(bool status)
{
	if (status)
	{
		soundSystem->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
	}
	else
	{
		soundSystem->setOutput(FMOD_OUTPUTTYPE_AUTODETECT);
	}
}

// Private Members
void SoundSystem::CheckVersion()
{
	unsigned versionNo;
	soundSystem->getVersion(&versionNo);
	COPIUM_ASSERT(versionNo > FMOD_VERSION,"Check version of FMOD");
}
void SoundSystem::CheckDrivers()
{
	int numDrivers;
	// Get number of sound cards
	soundSystem->getNumDrivers(&numDrivers);

	// No sound cards (disable sound)
	if (!numDrivers)
	{
		soundSystem->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
	}
}

}

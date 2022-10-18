﻿/*!***************************************************************************************
\file			sound-system.h
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

#pragma once

#include "fmod.hpp"
#include "CopiumCore\system-interface.h"

enum SoundAlias
{
	reeling,
	zap,
};

namespace Copium
{
	CLASS_SYSTEM(SoundSystem)
	{
	public:
		/***************************************************************************/
			/*!
			\brief
				Creates a sound system from FMOD and initialises it
			*/
			/**************************************************************************/
		void init();

		/***************************************************************************/
			/*!
			\brief
				Updates the FMOD sound system
			*/
			/**************************************************************************/
		void update();

		/***************************************************************************/
			/*!
			\brief
				Frees up any created sounds and exits
			*/
			/**************************************************************************/
		void exit();


		/***************************************************************************/
			/*!
			\brief
				Create a sound from the filename provided
			\param fileName
				the file path of the audio file
			\param alias
				easier way to refer to the audio file
			*/
			/**************************************************************************/
		void CreateSound(const char* fileName, unsigned alias);

		/***************************************************************************/
			/*!
			\brief
				Play the given audio
			\param alias
				which sound file should be played
			\param overlap
				should the audio be able to overlap
			\param loop
				should the audio loop
			\param loopCount
				to set for FMOD loop count
			*/
			/**************************************************************************/
		void Play(unsigned alias, bool overLap = true, bool loop = false, int loopCount = -1);

		/***************************************************************************/
			/*!
			\brief
				Stop the given audio
			\param alias
				which sound file should be stopped
			*/
			/**************************************************************************/
		void Stop(unsigned alias);
		// Set volume 0 - 1

		/***************************************************************************/
			/*!
			\brief
				Set the volume of an audio file
			\param alias
				which sound file should be stopped
			\param volume
				from 0.0 to 1.0 how loud should the audio file be
			*/
			/**************************************************************************/
		void SetVolume(unsigned alias, float volume);

		/***************************************************************************/
			/*!
			\brief
				Mutes or unmutes all audio
			\param status
				should all audio be muted
			*/
			/**************************************************************************/
		void Mute(bool status);
	private:

		//a map of all sounds created using CreateSound()
		std::map<unsigned, std::pair<FMOD::SoundGroup*, FMOD::Sound*>> soundList;

		//the sound system playing all the audio
		FMOD::System* soundSystem;

		/***************************************************************************/
			/*!
			\brief
				Checks the FMOD version
			*/
			/**************************************************************************/
		void CheckVersion();

		/***************************************************************************/
			/*!
			\brief
				Checks the users sound cards and disables audio if no sound card is
				found
			*/
			/**************************************************************************/
		void CheckDrivers();
	};
}
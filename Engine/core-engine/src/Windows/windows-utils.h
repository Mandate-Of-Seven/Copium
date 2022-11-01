#pragma once
/*!***************************************************************************************
\file			windows-utils.h
\project
\author			Matthew Lau

\par			Course: GAM200
\par			Section:
\date			30/10/2022

\brief
	Useful windows utilities are declared here.
	1. Open and save file dialogs

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include <string>

namespace Copium
{
	class FileDialogs
	{
	public:
		/*******************************************************************************
		/*!
		*
		\brief
			Create a windows open file dialog, using specified string as filter 

		\param _filter
			string containing filter to use for the file dialog

		\return
			if successful, return the file path of the file selected by the user
			if failed, return empty std::string

		*/
		/*******************************************************************************/
		static std::string open_file(const char* _filter);
		/*******************************************************************************
		/*!
		*
		\brief
			Create a windows save file dialog, using specified string as filter

		\param _filter
			string containing filter to use for the file dialog

		\return
			if successful, return the file path of the file selected by the user
			if failed, return empty std::string

		*/
		/*******************************************************************************/
		static std::string save_file(const char* _filter);
	};
}



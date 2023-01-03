/*!***************************************************************************************
\file			windows-utils.cpp
\project
\author			Matthew Lau

\par			Course: GAM200
\par			Section:
\date			30/10/2022

\brief
	Useful windows utilities are defined here.
	1. Open and save file dialogs

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#include "pch.h"
#include "Windows/windows-utils.h"
#include "Windows/windows-system.h"
#include "Utilities/thread-system.h"

#include <commdlg.h>	// Win32 API
#include <GLFW/glfw3.h>	// GLFW
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace Copium
{
	#define MAX_FILENAME_LENGTH 250
	namespace 
	{
	}

	std::string FileDialogs::open_file(const char* _filter)
	{
		OPENFILENAMEA ofn;
		char buffer[MAX_FILENAME_LENGTH] = {0};

		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);


		ofn.hwndOwner = glfwGetWin32Window(Copium::WindowsSystem::Instance().get_window());
		if (ofn.hwndOwner == nullptr)
			return std::string();

		ofn.lpstrFile = buffer;
		ofn.nMaxFile = sizeof(buffer);
		ofn.lpstrFilter = _filter;
		ofn.nFilterIndex = 1;

		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if ((bool)GetOpenFileNameA(&ofn) == true)
			return ofn.lpstrFile;
		else
			return std::string();
	}
	std::string FileDialogs::save_file(const char* _filter)
	{
		OPENFILENAMEA ofn;
		char buffer[MAX_FILENAME_LENGTH] = { 0 };

		ZeroMemory(&ofn, sizeof(OPENFILENAME));
		ofn.lStructSize = sizeof(OPENFILENAME);


		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow*)Copium::WindowsSystem::Instance().get_window());
		if (ofn.hwndOwner == nullptr)
			return std::string();

		ofn.lpstrFile = buffer;
		ofn.nMaxFile = sizeof(buffer);
		ofn.lpstrFilter = _filter;
		ofn.nFilterIndex = 1;

		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if ((bool)GetSaveFileNameA(&ofn) == true)
			return ofn.lpstrFile;
		else
			return std::string();
	}
}


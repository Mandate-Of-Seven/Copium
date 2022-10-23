#pragma once

#include "Windows\windows-input.h"
#include <string>

namespace Copium::Scripting
{

	extern "C"
	{
		void mono_add_internal_call(const char*, const void*);
	}

	const std::string CSScriptNamespace{ "CopiumEngine." };
	#define Register(CLASS,METHOD) mono_add_internal_call((CSScriptNamespace+#CLASS+"::"+#METHOD).c_str(),METHOD)

	#pragma region Input

		namespace
		{
			Copium::InputSystem& inputSystem{ *Copium::InputSystem::Instance() };
		}

		static bool GetKeyDown(int keyCode)
		{
			return inputSystem.is_key_pressed(keyCode);
		}

		static bool GetKeyUp(int keyCode)
		{
			return false;
		}

		static bool GetKey(int keyCode)
		{
			return inputSystem.is_key_held(keyCode);
		}

	#pragma endregion Input

	static void registerScriptWrappers()
	{
		Register(Input, GetKey);
	}
}



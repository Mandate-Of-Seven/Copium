#pragma once

#include <string>

namespace Copium
{
	static class FileDialogs
	{
	public:
		std::string open_file(const char* filter);
		std::string save_file(const char* filter);
	};
}



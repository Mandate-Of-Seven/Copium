#pragma once
#include <filesystem>

namespace Copium::FilePath
{
	static const std::string roslynCompilerPath{"..\\tools\\Roslyn\\csc"};
	static const std::string scriptsAssemblyPath{ "\\..\\x64\\Debug\\scripts.dll"};
	static const std::string projectName = {"ScriptingSandbox"};
	static const std::string projectPath{"..\\" + projectName};
}
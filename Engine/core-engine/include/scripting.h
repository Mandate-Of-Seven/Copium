#pragma once
#include "pch.h"
#include <string>
#include <filesystem>

#define SCRIPT_START(NAME) class NAME : public Engine::Script{ public: NAME();

#define SCRIPT_END(NAME) };NAME::NAME() :Script(__FILE__)\
	{std::string className{ typeid(*this).name() + 6 };}


#define PUBLIC public:
#define PRIVATE private:
#define PROTECTED protected:

namespace Engine
{
	
	class Script
	{
		using nameToTypeMap = std::map<std::string, std::string>;
	public:
		Script() = delete;
		Script& operator= (const Script&) = delete;
		Script(const Script&) = delete;

		Script(const std::string& name);
		Script(const std::filesystem::path& absolutePath);
		~Script();
		void generate();
		void populateVariablesFromHeader();
		void populateVariablesFromSource();
	protected:
		std::string name;
		nameToTypeMap variables;
	};

	class ScriptingEngine
	{
	public:
		static void init();
		static void shutdown();
		static void updateScriptsDll();
		static void generate();
		static ScriptingEngine* instance;
	};
}
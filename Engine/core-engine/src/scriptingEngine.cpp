#include "pch.h"
#include "scriptingEngine.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

namespace Engine
{
	struct ScriptEngineData
	{
		MonoDomain* RootDomain{ nullptr };
		MonoDomain* AppDomain{ nullptr };
	};

	static ScriptEngineData* s_Data;

	void ScriptingEngine::init()
	{
		initMono();
	}

	void ScriptingEngine::shutdown()
	{

	}

	void ScriptingEngine::initMono()
	{
		mono_set_assemblies_path("mono\lib");

		MonoDomain* rootDomain = mono_jit_init("EngineJITRuntime");
		if (rootDomain == nullptr)
		{
			//ERROR
			return;
		}

		s_Data->RootDomain = rootDomain;
		char appName[] = "MyAppDomain";
		s_Data->AppDomain = mono_domain_create_appdomain(appName,nullptr);
		mono_domain_set(s_Data->AppDomain, true);
	}

}
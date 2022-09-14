#pragma once

#include <mono/metadata/assembly.h>
#include <mono/metadata/attrdefs.h>
#include <mono/jit/jit.h>
#include <thread>
#include <filesystem>

namespace Copium::ScriptingEngine
{
	class ScriptFile final
	{
	public:
		ScriptFile() = delete;
		ScriptFile(const std::filesystem::path& _filePath);
		bool Modified() const;
		void Modified(bool _modified);
		void updateModificationTiming();
		const std::filesystem::path& FilePath() const;
		void FilePath(const std::filesystem::path& _filePath);
	private:
		std::filesystem::path filePath;
		bool modified;
		time_t lastModifiedTime;
	};

	// FLAG FOR MONO FIELDS
	enum class Accessibility : unsigned char
	{
		None = 0,
		Private = (1 << 0),
		Internal = (1 << 1),
		Protected = (1 << 2),
		Public = (1 << 3)
	};

	enum class FieldType
	{
		None,
		Vector2,
		Vector3,
		Vector4,
		Float = MONO_TYPE_R4,
		Integer = MONO_TYPE_I4,
		UnsignedInteger = MONO_TYPE_U4,
		String = MONO_TYPE_STRING
	};

	void init();
	void shutdown();
	void initMono();
	void shutdownMono();
	void tryRecompileDll();
	void createNewScript();

	MonoClass* loadMonoClass(const std::string& name);
	MonoObject* newScriptInstance(MonoClass*& monoClass);
	void trySwapDll(std::thread& compilerThread);
	unsigned char GetFieldAccessibility(MonoClassField* field);


	extern std::thread* compilerThread;
}
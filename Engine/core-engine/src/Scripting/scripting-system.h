#pragma once

#include <mono/metadata/assembly.h>
#include <mono/metadata/attrdefs.h>
#include <mono/jit/jit.h>
#include <thread>
#include <filesystem>
#include "system-interface.h"

namespace Copium::Scripting
{
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

	static FieldType GetType(MonoType* monoType)
	{
		int typeCode = mono_type_get_type(monoType); //MonoTypeEnum
		switch (typeCode)
		{
		case MONO_TYPE_VALUETYPE:
		{
			char* typeName = mono_type_get_name(monoType);
			if (strcmp(typeName, "Vector2") == 0) { return FieldType::Vector2; }
			if (strcmp(typeName, "Vector3") == 0) { return FieldType::Vector3; }
			if (strcmp(typeName, "Vector4") == 0) { return FieldType::Vector4; }
		}
		case MONO_TYPE_CLASS:
			break;
		default:
			return FieldType(typeCode);
			break;
		}
		PRINT("No type found");
		return FieldType::None;
	}

	CLASS_SYSTEM(ScriptingSystem)
	{
	public:

		void init();
		void update();
		void exit();
	};

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
	void createNewScript();

	MonoClass* loadMonoClass(const std::string& name);
	MonoObject* newScriptInstance(MonoClass*& monoClass);
	void trySwapDll(std::thread& compilerThread);
	unsigned char GetFieldAccessibility(MonoClassField* field);
}
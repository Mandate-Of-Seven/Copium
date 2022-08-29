
#include "pch.h"
#include "scripting.h"

#include <cstdio>

std::string scriptTmpDirectory = "..\\lib\\scripts\\src\\stash\\";

std::string scriptsFolder{ "Assets\\scripts\\" };

#define CONTENT(file) (std::istreambuf_iterator<char>(file)),(std::istreambuf_iterator<char>())

namespace Engine
{
	namespace
	{
		void line(std::string str, std::ostream& oStream, int tabsCount = 0);
		std::pair<std::string, std::string> getNameType(const std::string&);
		void strip(std::string&);
		void removeExtraSpaces(std::string&);
		std::list<Script*> scripts;
	}
	ScriptingEngine* ScriptingEngine::instance{new ScriptingEngine()};

	Script::Script(const std::string& _name)
	{
		std::ofstream scriptFile(scriptsFolder + _name + ".cpp");
		scriptFile <<
			"#include <scripting.h>\n\n" <<
			"class " << _name << ": public Engine::Script\n" <<
			"{\n" <<
				"\n\tvoid awake()\n\t{\n\n\t}\n" <<
				"\n\tvoid start()\n\t{\n\n\t}\n" <<
				"\n\tvoid update()\n\t{\n\n\t}\n" <<
			"\n};";
		name = _name;
		scriptFile.close();
	}

	Script::Script(const std::filesystem::path& absolutePath)
	{
		name = absolutePath.stem().string();
		populateVariablesFromHeader();
	}

	Script::~Script()
	{

	}

	void Script::populateVariablesFromHeader()
	{
		std::ifstream headerFile{ scriptTmpDirectory + name + ".h"};
		std::string buffer;
		bool start{false};
		while (std::getline(headerFile, buffer))
		{
			if (start)
			{
				if (buffer.find_first_of("}") != std::string::npos)
					break;
				strip(buffer);
				variables.insert(getNameType(buffer));
				std::cout << "VARIABLE: " << buffer << std::endl;
			}
			else if (buffer.find_first_of("{") != std::string::npos)
				start = true;
		}
		headerFile.close();
	}

	void Script::populateVariablesFromSource()
	{
	}

	std::map<std::string, std::list<Script*>> scriptInstances;

	void ScriptingEngine::init()
	{
		//LOAD Existing scripts
		std::filesystem::recursive_directory_iterator dirIt{ scriptsFolder };
		for (const std::filesystem::directory_entry& dirEntry : dirIt)
		{
			if (dirEntry.path().extension() == ".cpp")
			{
				Script* script = new Script(dirEntry.path().stem());
				script->generate();
				scripts.push_back(script);
				std::cout << "DIRECTORY: " << dirEntry << std::endl;
			}
		}
	}

	void ScriptingEngine::shutdown()
	{
	}

	void Script::generate()
	{
		std::ifstream scriptFile(scriptsFolder + name + ".cpp");

		std::string content{ CONTENT(scriptFile)};
		removeExtraSpaces(content);

		std::string buffer;
		size_t openCount{};
		size_t pos = content.find("class " + name);

		M_Assert(pos != std::string::npos, "class " + name + " not found in script!");

		pos = content.find('{', pos);

		M_Assert(pos != std::string::npos, "Brackets for class " + name + " not found!");

		++openCount;
		std::string::iterator contentIt{ content.begin() + pos + 1};
		while (contentIt != content.end() && openCount != 0)
		{
			char ch{ *contentIt };
			switch (ch)
			{
				case '{':
				{
					variables.insert(getNameType(buffer));
					++openCount;
					buffer.clear();
					break;
				}
				case '}':
					--openCount;
				case ';':
					buffer.clear();
					break;
				default:
					buffer += ch;
			}
			++contentIt;
		}
		scriptFile.close();

		std::ofstream scriptHeader(scriptTmpDirectory + name + ".h");

		line("#include <scripting.h>\n", scriptHeader);
		line("class " + name + ": public Engine::Script\n{", scriptHeader);
		nameToTypeMap::iterator nameTypeMapIt{ variables.begin()};
		while (nameTypeMapIt != variables.end())
		{
			line
			(
				nameTypeMapIt->second + ' ' + nameTypeMapIt->first + ';',
				scriptHeader, 1
			);
			++nameTypeMapIt;
		}
		line("}", scriptHeader);
		scriptHeader.close();
	}

	namespace
	{
		void line(std::string str, std::ostream& oStream, int tabsCount)
		{
			std::string buffer;
			for (int i{}; i < tabsCount; ++i)
				oStream << '\t';
			oStream << str << std::endl;
		}


		std::pair<std::string, std::string> getNameType(const std::string& str)
		{
			std::string buffer{ str };
			strip(buffer);
			removeExtraSpaces(buffer);
			size_t spacePos = buffer.find_first_of(' ');
			std::string type = buffer.substr(0, spacePos);
			std::string name = buffer.substr(spacePos + 1);
			return { name ,type };
		}

		void strip(std::string& str)
		{
			std::regex exprStartWhitespace("^\\s+");
			std::regex exprEndWhitespace("\\s+$");

			str = std::regex_replace(str, exprStartWhitespace, "");
			str = std::regex_replace(str, exprEndWhitespace, "");
		}

		void removeExtraSpaces(std::string& str)
		{
			std::regex exprWhitespace("\\s+");
			str = std::regex_replace(str, exprWhitespace, " ");
		}
	}
}
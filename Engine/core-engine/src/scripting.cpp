
#include "pch.h"
#include "scripting.h"


std::string scriptTmpDirectory = "..\\lib\\scripts\\src\\stash\\";

std::string scriptsFolder{ "Assets\\scripts\\" };

#define CONTENT(file) (std::istreambuf_iterator<char>(file)),(std::istreambuf_iterator<char>())

namespace Engine
{
	namespace
	{
		void line(const std::string& str, std::ostream& oStream, int tabsCount = 0);
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
	}

	Script::~Script()
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
		std::ofstream scriptSource(scriptTmpDirectory + name + ".cpp");

		// Creating a directory
		if (!std::filesystem::exists(scriptTmpDirectory))
			std::filesystem::create_directory(scriptTmpDirectory);

		line("#include \"" + name + "\.h\"", scriptSource);

		std::string buffer;
		size_t openCount{};																																																																	

		std::regex exprClassStartPos("class\\s+" + name + ".+\\s*\\{");

		std::smatch searchMatches;
		char ch;
		while (scriptFile.get(ch))
		{
			if (!searchMatches.size())
			{
				buffer += ch;
				if (std::regex_search(buffer, searchMatches, exprClassStartPos))
				{
					line(buffer.substr(0, buffer.rfind("class")), scriptSource);
					buffer.clear();
					++openCount;
				}
			}
			else if (openCount)
			{
				switch (ch)
				{
				case ';':
					if (openCount == 1)
					{
						strip(buffer);
						scriptSource << name << "::" << buffer << ';' << std::endl;
						variables.insert(getNameType(buffer));
						buffer.clear();
					}
					else
					{
						buffer += ch;
					}
					break;
				case '{':
					if (openCount == 1)
					{
						strip(buffer);
						scriptSource << name << "::" << buffer << '{' << std::endl;
						variables.insert(getNameType(buffer));
						buffer.clear();
					}
					else
					{
						buffer += ch;
					}
					++openCount;
					break;
				case '}':
					--openCount;
					if (openCount == 1)
					{
						scriptSource << buffer << '}' << std::endl;
						buffer.clear();
					}
					else
					{
						buffer += ch;
					}
					break;
				default:
					buffer += ch;
				}
			}
		}

		scriptFile.close();
		scriptSource.close();

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
		void line(const std::string& str, std::ostream& oStream, int tabsCount)
		{
			std::string buffer;
			for (int i{}; i < tabsCount; ++i)
				oStream << '\t';
			oStream << str << std::endl;
		}


		std::pair<std::string, std::string> getNameType(const std::string& str)
		{
			std::string buffer{ str };
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
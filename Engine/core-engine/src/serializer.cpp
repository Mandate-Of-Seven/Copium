/*!***************************************************************************************
****
\file			serializer.cpp
\project
\author			Matthew Lau
\co-authors

\par			Course: GAM200
\par			Section:
\date			07/08/2022

\brief
	Contains definitions for functions that are responsible for the serialization and deserialization of scene data.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/
#include "pch.h"
#include "serializer.h"
#include <rttr/registration>
std::string prefix("Data\\");

/*
int serialize_scene(const std::string& _filename, Scene* _scene) 
{	
	std::cout << "Scene save initiated....................\n";
	std::string _filepath(prefix + _filename);
	std::ofstream ofs(_filepath, std::ios::out | std::ios::binary);
	if (!ofs) 
	{
		std::cout << "Error opening output file\n";
		return 1;
	}

	//Write game object count
	int objCount = (int)_scene->get_gameobjcount();
	ofs.write((char*)&objCount, sizeof(int));
	std::cout << "Number of game objects in scene: " << objCount << std::endl;

	std::vector<GameObject*> parents;
	
	for(size_t i{0}; i < _scene->get_gameobjcount(); ++i)
	{
		//if game object is a pure parent (no parent)
		if (!_scene->get_gameobjectvector()[i]->has_parent())
		{
			parents.push_back(_scene->get_gameobjectvector()[i]);
		}
	}

	char nl{ '\n' };
	ofs.write(&nl, sizeof(char));


	for (GameObject* g : parents)
	{
		//Write the PARENT to save file-----------------
		
		//Write game object's ID
		std::cout << "Game Object's ID: " << g->get_id() << std::endl;
		GameObjectID id = g->get_id();
		ofs.write((char*)&id, sizeof(unsigned long));

		//Write length of name
		int nameLength = g->get_name().length();
		ofs.write((char*)&nameLength, sizeof(int));
		std::cout << "Length of name: " << nameLength << std::endl;

		//Write name
		std::string objName = g->get_name();
		for (size_t j{ 0 }; j < nameLength; ++j) 
		{
			ofs.write((char*)&objName[j], sizeof(char));
			std::cout << objName[j];
		}
		std::cout << '\n';

		//Write pos, rot, scale
		Transform t(g->Trans());
		serialize_transform(ofs, t);

		//Check if game object has any components
		if (!g->Components().empty())
		{

			int compCount = (int)g->Components().size();
			ofs.write((char*)&compCount, sizeof(int));

			for (Component* co : g->Components())
			{
				int compID = (int)co->ID();
				ofs.write((char*)&compID, sizeof(int));

			}
		}
		else
		{
			int compCount = 0;
			ofs.write((char*)&compCount, sizeof(int));
		}

		//----------------------------------------------
			

		size_t childCount = g->childList().size();
		ofs.write((char*)&childCount, sizeof(size_t));
		std::cout << "Number of children in game object: " << childCount << std::endl;
		//Write CHILDREN to save file-------------------
		for (GameObject* child : g->childList())
		{
			//Write children to save file
			//Write game object's ID
			std::cout << "Game Objects ID: " << child->get_id() << std::endl;
			GameObjectID childID = child->get_id();
			ofs.write((char*)&childID, sizeof(unsigned long));

			//Write length of name
			int childNameLength = child->get_name().length();
			ofs.write((char*)&childNameLength, sizeof(int));
			std::cout << "Length of name:" << childNameLength << std::endl;
			//Write name
			std::string childName = child->get_name();
			for (size_t j{ 0 }; j < childNameLength; ++j)
			{
				ofs.write((char*)&childName[j], sizeof(char));
				std::cout << childName[j];
			}
			std::cout << '\n';
			//Write pos, rot, scale
			Transform t1(child->Trans());
			serialize_transform(ofs, t1);
			//Check if game object has any components
			if (!child->Components().empty())
			{
				std::cout << "Game Object has components\n";
				int compCount = (int)child->Components().size();
				ofs.write((char*)&compCount, sizeof(int));

				for (Component* co : child->Components())
				{
					int compID = (int)co->ID();
					ofs.write((char*)&compID, sizeof(int));

				}
			}
			else
			{
				int compCount = 0;
				ofs.write((char*)&compCount, sizeof(int));
			}
			

		}
		ofs.write(&nl, sizeof(char));
		//----------------------------------------------1
	}
	
	ofs.close();
	std::cout << "scene saved...................\n";
	return 0;
	
}

void serialize_transform(std::ofstream& _ofs, const Transform& _trans) 
{
	std::cout << "game obj transform being saved\n";
	Transform t(_trans);

	//Write position
	for (int i{ 0 }; i < 3; ++i)
	{
		_ofs.write((char*)&(t.Position()[i]), sizeof(double));
	}
	//Write rotation
	for (int i{ 0 }; i < 3; ++i)
	{
		_ofs.write((char*)&(t.Rotation()[i]), sizeof(double));
	}
	//Write scale
	for (int i{ 0 }; i < 3; ++i)
	{
		_ofs.write((char*)&(t.Scale()[i]), sizeof(double));
	}




}

int deserialize_scene(const std::string& _filename, Scene* _scene) 
{
	std::string _filepath(prefix + _filename);
	if (!does_file_exist(_filepath)) {
		std::cout << _filepath << " does not exist\n";
		return 1;
	}
	std::ifstream ifs(_filepath, std::ios::out | std::ios::binary);
	if (!ifs) {
		std::cout << "Error opening save file for loading\n";
		return 1;
	}

	std::cout << "Loading scene save\n";
	int gameObjCount;
	ifs.read((char*)&gameObjCount, sizeof(gameObjCount));
	std::cout << "No. of gameobjects in the scene:" << gameObjCount << std::endl;
	for (int i{ 0 }; i < gameObjCount; ++i)
	{
		//create game object
		GameObject* tmp = new GameObject();

		//Read game object ID
		unsigned long objID;
		ifs.read((char*)&objID, sizeof(objID));
		tmp->set_id(objID);

		//Read game object's name length
		int nameLength;
		ifs.read((char*)&nameLength, sizeof(nameLength));

		//Read game object's name
		std::string name(nameLength, '\0');
		for (int j{ 0 }; j < nameLength; ++j)
		{
			char ch;
			ifs.read((char*)&ch, sizeof(ch));
			name[j] = ch;
		}
		tmp->set_name(name);

		//Read game object's transform---------------------------
		Transform t;
		Vector3 v;
		//Position-----
		for (int j{ 0 }; j < 3; ++j)
		{
			double d;
			ifs.read((char*)&d, sizeof(d));
			v[j] = d;
		}
		t.Position(v);
		//Rotation-----
		for (int j{ 0 }; j < 3; ++j)
		{
			double d;
			ifs.read((char*)&d, sizeof(d));
			v[j] = d;
		}
		t.Rotation(v);
		//Scale-----
		for (int j{ 0 }; j < 3; ++j)
		{
			double d;
			ifs.read((char*)&d, sizeof(d));
			v[j] = d;
		}
		t.Scale(v);	
		tmp->Trans(t);
		//--------------------------------------------------------

		//Read game object's components (if any)------------------
		int compCount = 0;
		ifs.read((char*)compCount, sizeof(int));
		if (compCount)
		{
			for (int j{ 0 }; j < compCount; ++j)
			{
				ComponentID newID{0};
				ifs.read((char*)newID, sizeof(ComponentID));
				tmp->addComponent((Component::Type)newID);
			}
		}
		//--------------------------------------------------------

		//Add game object to scene
		_scene->add_gameobject(tmp);

		std::cout << "Parent deserialized\n";

			size_t childCount{0};
			ifs.read((char*)&childCount, sizeof(childCount));

			if (!childCount)
			{
				tmp = nullptr;
				continue;
			}

			GameObject* parent = tmp;

			//Read all the children of this parent
			for (size_t j{ 0 }; j < childCount; ++j)
			{

				tmp = new GameObject();

				//Read game object ID
				ifs.read((char*)&objID, sizeof(objID));
				tmp->set_id(objID);

				//Read game object's name length
				ifs.read((char*)&nameLength, sizeof(nameLength));

				//Read game object's name
				std::string childName(nameLength, '\0');
				for (int k{ 0 }; k < nameLength; ++k)
				{
					char ch;
					ifs.read((char*)&ch, sizeof(ch));
					childName[k] = ch;
				}
				tmp->set_name(childName);

				//Read game object's transform---------------------------
				//Position-----
				for (int k{ 0 }; k < 3; ++k)
				{
					double d;
					ifs.read((char*)&d, sizeof(d));
					v[k] = d;
				}
				t.Position(v);
				//Rotation-----
				for (int k{ 0 }; k < 3; ++k)
				{
					double d;
					ifs.read((char*)&d, sizeof(d));
					v[k] = d;
				}
				t.Rotation(v);
				//Scale-----
				for (int k{ 0 }; k < 3; ++k)
				{
					double d;
					ifs.read((char*)&d, sizeof(d));
					v[k] = d;
				}
				t.Scale(v);
				tmp->Trans(t);
				//--------------------------------------------------------


				ifs.read((char*)compCount, sizeof(int));
				if (compCount)
				{
					for (int k{ 0 }; k < compCount; ++k)
					{
						ComponentID newID{0};
						ifs.read((char*)newID, sizeof(ComponentID));
						tmp->addComponent((Component::Type)newID);
					}
				}

				_scene->add_gameobject(tmp);
				parent->attach_child(tmp);
				tmp = nullptr;

				++i;
				std::cout << "Child deserialized\n";
			}

		ifs.ignore(1);

	}

	ifs.close();
	return 0;
}

bool does_file_exist(const std::string& _filename) 
{
	std::ifstream ifs;
	ifs.open(_filename);
	if (!ifs)
	{
		ifs.close();
		return false;
	}
	else
	{
		ifs.close();
		return true;
	}

}
*/
JsonSerializer::JsonSerializer(const std::string& _filename) : isw{nullptr} {
	std::cout << "Json Serializer default ctor...\n";
	file.open(_filename);
	if (!file)
	{
		std::cout << "error opening file\n";
		return;
	}

	isw = new rapidjson::IStreamWrapper(file);
	if (!isw) {
		std::cout << "error allocating memory for istreamwrapper\n";
		return;
	}
	if (document.ParseStream<rapidjson::kParseStopWhenDoneFlag>(*isw).HasParseError()) {
		std::cout << "parsing error!!\n";
		return;
	}

	std::cout << "Parsing complete!!\n" << "Json Serializer Ready\n";
}
JsonSerializer::~JsonSerializer()
{
	file.close();
	if (isw)
		delete isw;

}

void JsonSerializer::read_int(int64_t& _int)
{
	_int = document["i"].GetInt64();
}
void JsonSerializer::read_double(double& _double)
{
	_double = document["d"].GetDouble();
}
void JsonSerializer::read_float(float& _float)
{
	_float = document["f"].GetFloat();
} 
void JsonSerializer::read_string(std::string& _str)
{
	_str = document["str"].GetString();
}

void stream_read(Serializer& _s, double& _d)
{
	_s.read_double(_d);
}
void stream_read(Serializer& _s, int64_t& _i)
{
	_s.read_int(_i);
}


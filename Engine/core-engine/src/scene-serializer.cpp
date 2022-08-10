/*!***************************************************************************************
****
\file			scene-serializer.cpp
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
#include "scene-serializer.h"

std::string prefix("Data\\");

int serialize_scene(const std::string& _filename, Scene* _scene) 
{	
	std::cout << "Scene save initiated...\n";
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

	for (int i{ 0 }; i < objCount; ++i)
	{
		std::cout << "#" << i + 1 << std::endl;
		GameObject* tmp = _scene->get_gameobjectvector()[i];
		if (!tmp) 
		{
			continue;
		}

		//Write game object's ID
		std::cout << "Game Objects ID: " << tmp->get_id() << std::endl;
		GameObjectID id = tmp->get_id();
		ofs.write((char*)&id, sizeof(unsigned long));

		//Write length of name
		int nameLength = tmp->get_name().length();
		ofs.write((char*)&nameLength, sizeof(int));
		std::cout << "Length of name:" << nameLength << std::endl;
		//Write name
		std::string objName = tmp->get_name();
		for (size_t j{ 0 }; j < nameLength; ++j) {
			ofs.write((char*)&objName[j], sizeof(char));
			std::cout << objName[j];
		}
		std::cout << '\n';
		//Write pos, rot, scale
		Vector3 v1(1, 2, 3);
		Transform t(v1, v1, v1);
		serialize_trans(ofs, t);

	}

	std::cout << "scene saved\n";
	return 0;
	
}

void serialize_trans(std::ofstream& _ofs, const Transform& _trans) 
{
	std::cout << "game obj transform being saved\n";
	Vector3 v{ 1,2,3 };
	Transform t(_trans);

	//Write position
	for (int i{ 0 }; i < 3; ++i)
	{
		_ofs.write((char*)&(t.Position()[i]), sizeof(double));
		std::cout << t.Position()[i] << std::endl;
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

		_scene->add_gameobject(tmp);
		tmp = nullptr;
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

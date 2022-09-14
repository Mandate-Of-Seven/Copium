/*!***************************************************************************************
****
\file			serializer.h
\project
\author			Matthew Lau
\co-authors

\par			Course: GAM200
\par			Section:
\date			07/08/2022

\brief
	Contains declarations for functions that are responsible for the serialization and deserialization of scene data.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/
#pragma once
#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <iostream>
#include <fstream>
#include "game-object.h"
#include "scene.h"
#include "transform.h"
#include <rapidjson/prettywriter.h>
#include <math-library.h>

extern std::string prefix;

class Serializer 
{
public:
	template <typename T>
	bool serialize(std::ofstream& _ofs, const T& _obj)
	{
		rapidjson::PrettyWriter<rapidjson::StringBuffer> w(sb);
		bool result = _obj.serialize(w);
		//bool result = T::serialize(w, _obj);
		std::string json(sb.GetString(), sb.GetSize());
		_ofs << json;
		std::cout << "serializer serialize called\n";
		return false;
	}
	template <typename T, typename Writer>
	bool deserialize(std::ifstream& _ifs, const T& _obj, Writer& _writer)
	{
		return false;
	}
private:
	rapidjson::StringBuffer sb;
};

/*******************************************************************************
/*!
*
\brief
	Serialize the specified scene's data

\param	_filename
	filename of the save file to be created/opened onto which the scene data will be serialized to

\param _scene
	pointer to the scene whose data is to be serialized

\return
	if any errors occur during the serialization, returns 1
	if serialization is smooth, returns 0
*/
/*******************************************************************************/
int serialize_scene(const std::string& _filename, Scene* _scene);

/*******************************************************************************
/*!
*
\brief
	Serialize the specified Transform class

\param	_filestream
	reference to the output stream(save file)

\param _trans
	reference to the Transform class that is to be serialized

\return
	void
*/
/*******************************************************************************/
void serialize_transform(std::ofstream& _filestream, const Transform& _trans);

/*******************************************************************************
/*!
*
\brief
	Deserialize scene data from the specified file and load the data into the specified scene

\param	_filename
	filename of the save file to be opened for deserialization

\param _scene
	pointer to the scene which will be loaded with data from the specified file

\return
	if any errors occur during the deserialization, returns 1
	if deserialization is smooth, returns 0
*/
/*******************************************************************************/
int deserialize_scene(const std::string& _filename, Scene* _scene);

/*******************************************************************************
/*!
*
\brief
	Checks if the specified file exists

\param	_filename
	filename of the file whose existence is to be checked

\return
	if file exists, return true
	if file does not exist, return false 
*/
/*******************************************************************************/
bool does_file_exist(const std::string& _filename);

#endif SERIALIZER_H
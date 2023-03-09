/*!***************************************************************************************
\file			serializer.h
\project
\author			Matthew Lau
\co-author		Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			27/09/2022

\brief
	This file holds the declarations of functions that are involved in de/serialization in Copium Engine.
	Especially for complex types.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef SERIALIZER_H
#define SERIALIZER_H

#include <rapidjson/document.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>

// Utilities
#include "Utilities/json-utilities.h"

// Math Types
#include "Math/Vector4.h"
#include "Math/math-library.h"

// Components
#include "Animation/animation-system.h"
#include "GameObject/game-object.h"
#include "GameObject/components.h"

namespace Copium::Serializer {


	/***************************************************************************/
	/*!
	\brief
		Base template function for serialization of a complex data type

	\param _data
		read-only reference to the variable whose data needs to be serialized

	\param _key
		the key for the value that will hold the serialized data

	\param _value
		reference to the rapidjson::Value that the wanted data is held in

	\return
		void
	*/
	/***************************************************************************/
	template<typename T>
	void Serialize(T& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc)
	{
		PRINT(typeid(T).name() << " does not have a Serialize function for it!");
	}



	// Math Types
	/***************************************************************************/
	/*!
	\brief
		Template specialization for serializing a Copium::Vec2

	\param _data
		read-only reference to the Copium::Vec2 whose data needs to be serialized

	\param _key
		the key for the value that will hold the serialized data

	\param _value
		reference to the rapidjson::Value will hold the data

	\param _doc
		reference to the rapidjson::Document that is being serialized to

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Serialize<Math::Vec2>(Math::Vec2& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc);		
	/***************************************************************************/
	/*!
	\brief
		Template specialization for serializing a Copium::Vec3

	\param _data
		read-only reference to the Copium::Vec3 whose data needs to be serialized

	\param _key
		the key for the value that will hold the serialized data

	\param _value
		reference to the rapidjson::Value will hold the data

	\param _doc
		reference to the rapidjson::Document that is being serialized to

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Serialize<Math::Vec3>(Math::Vec3& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc);
	/***************************************************************************/
	/*!
	\brief
		Template specialization for serializing a Copium::Vec4

	\param _data
		read-only reference to the Copium::Vec4 whose data needs to be serialized

	\param _key
		the key for the value that will hold the serialized data

	\param _value
		reference to the rapidjson::Value will hold the data

	\param _doc
		reference to the rapidjson::Document that is being serialized to

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Serialize<Math::Vec4>(Math::Vec4& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc);



	// Components-----
	/***************************************************************************/
	/*!
	\brief
		Template specialization for serializing a component
		Depending on the type of component to be serialized, the corresponding template specialization for serializing that component will be called.

	\param _type
		the type of component that is to be serialized

	\param _co
		ptr to the component that is to be serialized

	\param _value
		reference to the rapidjson::Value will hold the component data

	\param _doc
		reference to the rapidjson::Document that is being serialized to

	\return
		void
	*/
	/***************************************************************************/
	void SerializeComponent(const ComponentType& _type, Component* _co, rapidjson::Value& _value, rapidjson::Document& _doc);
	// Animator
	template<>
	void Serialize<Animation>(Animation& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc);
	template<>
	void Serialize<Animator>(Animator& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc);
	// Transform
	template<>
	void Serialize<Transform>(Transform& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc);
	// SpriteRenderer
	template<>
	void Serialize<SpriteRenderer>(SpriteRenderer& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc);
	template<>
	void Serialize<Sprite>(Sprite& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc);	
	// Audio Source
	template<>
	void Serialize<AudioSource>(AudioSource& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc);
	// Physics 
	template<>
	void Serialize<Rigidbody2D>(Rigidbody2D& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc);
	template<>
	void Serialize<AABB>(AABB& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc);
	template<>
	void Serialize<BoxCollider2D>(BoxCollider2D& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc);
	
	// Camera
	template<>
	void Serialize<Camera>(Camera& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc);
	// Sorting Group
	template<>
	void Serialize<SortingGroup>(SortingGroup& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc);

	// UI
	template<>
	void Serialize<Image>(Image& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc);
	template<>
	void Serialize<Button>(Button& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc);
	template<>
	void Serialize<Text>(Text& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc);

	// Script
	template<>
	void Serialize<Script>(Script& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc);

	template<>
	void Serialize<GameObject>(GameObject& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc);


	void WriteToFile(rapidjson::StringBuffer& _sb, rapidjson::Document& _doc, std::ofstream& _os);









	template<typename T>
	void Deserialize(T& _data, const std::string& _key, rapidjson::Value& _value)
	{
		PRINT(typeid(T).name() << " does not have a Deserialize function for it!");
	}
	/***************************************************************************/
	/*!
	\brief
		Deserializes a Vec2 from the specified rapidjson Value
		Note: user must ensure that the value specified is valid

	\param _data
		the Vec2 which will store the read data

	\param _key
		key of the data

	\param _value
		reference to the rapidjson::Value that contains the data to be read

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Deserialize<Math::Vec2>(Math::Vec2& _data, const std::string& _key, rapidjson::Value& _value);
	/***************************************************************************/
	/*!
	\brief
		Deserializes a Vec3 from the specified rapidjson Value
		Note: user must ensure that the value specified is valid

	\param _data
		the Vec3 which will store the read data

	\param _key
		key of the data

	\param _value
		reference to the rapidjson::Value that contains the data to be read

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Deserialize<Math::Vec3>(Math::Vec3& _data, const std::string& _key, rapidjson::Value& _value);
	/***************************************************************************/
	/*!
	\brief
		Deserializes a Vec4 from the specified rapidjson Value
		Note: user must ensure that the value specified is valid

	\param _data
		the Vec4 which will store the read data

	\param _key
		key of the data

	\param _value
		reference to the rapidjson::Value that contains the data to be read

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Deserialize<Math::Vec4>(Math::Vec4& _data, const std::string& _key, rapidjson::Value& _value);

	template<>
	void Deserialize<GameObject>(GameObject& _data, const std::string& _key, rapidjson::Value& _value);

	// Transform
	template<>
	void Deserialize<Transform>(Transform& _data, const std::string& _key, rapidjson::Value& _value);

	// Camera
	template<>
	void Deserialize<Camera>(Camera& _data, const std::string& _key, rapidjson::Value& _value);

	// Sorting Group
	template<>
	void Deserialize<SortingGroup>(SortingGroup& _data, const std::string& _key, rapidjson::Value& _value);

	// Audio Source
	template<>
	void Deserialize<AudioSource>(AudioSource& _data, const std::string& _key, rapidjson::Value& _value);

	// Animation
	template<>
	void Deserialize<Animator>(Animator& _data, const std::string& _key, rapidjson::Value& _value);
	template<>
	void Deserialize<Animation>(Animation& _data, const std::string& _key, rapidjson::Value& _value);

	// Physics
	template<>
	void Deserialize<AABB>(AABB& _data, const std::string& _key, rapidjson::Value& _value);
	template<>
	void Deserialize<BoxCollider2D>(BoxCollider2D& _data, const std::string& _key, rapidjson::Value& _value);
	template<>
	void Deserialize<Rigidbody2D>(Rigidbody2D& _data, const std::string& _key, rapidjson::Value& _value);

	// Sprite
	template<>
	void Deserialize<Sprite>(Sprite& _data, const std::string& _key, rapidjson::Value& _value);
	template<>
	void Deserialize<SpriteRenderer>(SpriteRenderer& _data, const std::string& _key, rapidjson::Value& _value);

	// UI
	template<>
	void Deserialize<Button>(Button& _data, const std::string& _key, rapidjson::Value& _value);
	template<>
	void Deserialize<Text>(Text& _data, const std::string& _key, rapidjson::Value& _value);
	template<>
	void Deserialize<Image>(Image& _data, const std::string& _key, rapidjson::Value& _value);

	template<>
	void Deserialize<Script>(Script& _data, const std::string& _key, rapidjson::Value& _value);
	void DeserializeComponent(ComponentType _type, Component* _data, rapidjson::Value& _value);


	void DeserializeLink(GameObject& _data, rapidjson::Value& _value);
	void DeserializeLink(Script& _data,rapidjson::Value& _value);
	void DeserializeLink(Button& _data, rapidjson::Value& _value);
}


#endif 

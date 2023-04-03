/*!***************************************************************************************
\file			serializer.h
\project
\author			Matthew Lau
\co-author		Zacharie Hong

\par			Course: GAM250
\par			Section:
\date			27/09/2022

\brief
	This file holds the declarations of functions that are involved in de/serialization in Copium Engine.
	Especially for complex types.

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
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
	/***************************************************************************/
	/*!
	\brief
		Template specialization for serializing an Animation

	\param _data
		reference to the Animation that is to be serialized

	\param _key
		string that is to be used as the key for the value

	\param _value
		reference to the rapidjson::Value will hold the component data

	\param _doc
		reference to the rapidjson::Document that is being serialized to

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Serialize<Animation>(Animation& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc);
	/***************************************************************************/
	/*!
	\brief
		Template specialization for serializing an Animator

	\param _data
		reference to the Animator that is to be serialized

	\param _key
		string that is to be used as the key for the value

	\param _value
		reference to the rapidjson::Value will hold the component data

	\param _doc
		reference to the rapidjson::Document that is being serialized to

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Serialize<Animator>(Animator& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc);
	// Transform
	/***************************************************************************/
	/*!
	\brief
		Template specialization for serializing a Transform component

	\param _data
		reference to the Transform component that is to be serialized

	\param _key
		string that is to be used as the key for the value

	\param _value
		reference to the rapidjson::Value will hold the component data

	\param _doc
		reference to the rapidjson::Document that is being serialized to

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Serialize<Transform>(Transform& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc);
	// SpriteRenderer
	/***************************************************************************/
	/*!
	\brief
		Template specialization for serializing a SpriteRenderer

	\param _data
		reference to the SpriteRenderer that is to be serialized

	\param _key
		string that is to be used as the key for the value

	\param _value
		reference to the rapidjson::Value will hold the component data

	\param _doc
		reference to the rapidjson::Document that is being serialized to

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Serialize<SpriteRenderer>(SpriteRenderer& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc);
	/***************************************************************************/
	/*!
	\brief
		Template specialization for serializing a Sprite

	\param _data
		reference to the Sprite that is to be serialized

	\param _key
		string that is to be used as the key for the value

	\param _value
		reference to the rapidjson::Value will hold the component data

	\param _doc
		reference to the rapidjson::Document that is being serialized to

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Serialize<Sprite>(Sprite& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc);	
	// Audio Source
	/***************************************************************************/
	/*!
	\brief
		Template specialization for serializing an AudioSource

	\param _data
		reference to the AudioSource that is to be serialized

	\param _key
		string that is to be used as the key for the value

	\param _value
		reference to the rapidjson::Value will hold the component data

	\param _doc
		reference to the rapidjson::Document that is being serialized to

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Serialize<AudioSource>(AudioSource& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc);
	// Physics 
	/***************************************************************************/
	/*!
	\brief
		Template specialization for serializing a RigidBody2D

	\param _data
		reference to the RigidBody2D that is to be serialized

	\param _key
		string that is to be used as the key for the value

	\param _value
		reference to the rapidjson::Value will hold the component data

	\param _doc
		reference to the rapidjson::Document that is being serialized to

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Serialize<Rigidbody2D>(Rigidbody2D& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc);
	/***************************************************************************/
	/*!
	\brief
		Template specialization for serializing an AABB struct

	\param _data
		reference to the AABB struct that is to be serialized

	\param _key
		string that is to be used as the key for the value

	\param _value
		reference to the rapidjson::Value will hold the component data

	\param _doc
		reference to the rapidjson::Document that is being serialized to

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Serialize<AABB>(AABB& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc);
	/***************************************************************************/
	/*!
	\brief
		Template specialization for serializing a BoxCollider2D

	\param _data
		reference to the BoxCollider2D that is to be serialized

	\param _key
		string that is to be used as the key for the value

	\param _value
		reference to the rapidjson::Value will hold the component data

	\param _doc
		reference to the rapidjson::Document that is being serialized to

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Serialize<BoxCollider2D>(BoxCollider2D& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc);
	
	// Camera
	/***************************************************************************/
	/*!
	\brief
		Template specialization for serializing a Camera

	\param _data
		reference to the Camera that is to be serialized

	\param _key
		string that is to be used as the key for the value

	\param _value
		reference to the rapidjson::Value will hold the component data

	\param _doc
		reference to the rapidjson::Document that is being serialized to

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Serialize<Camera>(Camera& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc);
	// Sorting Group
	/***************************************************************************/
	/*!
	\brief
		Template specialization for serializing a SortingGroup

	\param _data
		reference to the SortingGroup that is to be serialized

	\param _key
		string that is to be used as the key for the value

	\param _value
		reference to the rapidjson::Value will hold the component data

	\param _doc
		reference to the rapidjson::Document that is being serialized to

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Serialize<SortingGroup>(SortingGroup& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc);

	// UI
	/***************************************************************************/
	/*!
	\brief
		Template specialization for serializing an Image component

	\param _data
		reference to the Image component that is to be serialized

	\param _key
		string that is to be used as the key for the value

	\param _value
		reference to the rapidjson::Value will hold the component data

	\param _doc
		reference to the rapidjson::Document that is being serialized to

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Serialize<Image>(Image& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc);
	/***************************************************************************/
	/*!
	\brief
		Template specialization for serializing a Button

	\param _data
		reference to the Button that is to be serialized

	\param _key
		string that is to be used as the key for the value

	\param _value
		reference to the rapidjson::Value will hold the component data

	\param _doc
		reference to the rapidjson::Document that is being serialized to

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Serialize<Button>(Button& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc);
	/***************************************************************************/
	/*!
	\brief
		Template specialization for serializing a Text component

	\param _data
		reference to the Text that is to be serialized

	\param _key
		string that is to be used as the key for the value

	\param _value
		reference to the rapidjson::Value will hold the component data

	\param _doc
		reference to the rapidjson::Document that is being serialized to

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Serialize<Text>(Text& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc);

	// Script
	/***************************************************************************/
	/*!
	\brief
		Template specialization for serializing a Script

	\param _data
		reference to the Script that is to be serialized

	\param _key
		string that is to be used as the key for the value

	\param _value
		reference to the rapidjson::Value will hold the component data

	\param _doc
		reference to the rapidjson::Document that is being serialized to

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Serialize<Script>(Script& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc);

	/***************************************************************************/
	/*!
	\brief
		Template specialization for serializing a GameObject

	\param _data
		reference to the GameObject that is to be serialized

	\param _key
		string that is to be used as the key for the value

	\param _value
		reference to the rapidjson::Value will hold the component data

	\param _doc
		reference to the rapidjson::Document that is being serialized to

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Serialize<GameObject>(GameObject& _data, const std::string& _key, rapidjson::Value& _value, rapidjson::Document& _doc);

	/***************************************************************************/
	/*!
	\brief
		Write a rapidjson::Document to a file

	\param _sb
		reference to the string buffer that will contain the data as text

	\param _doc
		reference to the rapidjson::Document that contains the data

	\param _os
		reference to the output stream

	\return
		void
	*/
	/***************************************************************************/
	void WriteToFile(rapidjson::StringBuffer& _sb, rapidjson::Document& _doc, std::ofstream& _os);








	/***************************************************************************/
	/*!
	\brief
		Base template function for deserialization of a complex data type

	\param _data
		read-only reference to the variable to deserialize to

	\param _key
		the key of the value to deserialize from

	\param _value
		reference to the rapidjson::Value to deserialize from

	\return
		void
	*/
	/***************************************************************************/
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

	/***************************************************************************/
	/*!
	\brief
		Template specialization for deserializing a GameObject

	\param _data
		reference to the GameObject to deserialize to

	\param _key
		the key of the value to deserialize from

	\param _value
		reference to the rapidjson::Value to deserialize from

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Deserialize<GameObject>(GameObject& _data, const std::string& _key, rapidjson::Value& _value);

	// Transform
	/***************************************************************************/
	/*!
	\brief
		Template specialization for deserializing a Transform component

	\param _data
		reference to the Transform component to deserialize to

	\param _key
		the key of the value to deserialize from

	\param _value
		reference to the rapidjson::Value to deserialize from

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Deserialize<Transform>(Transform& _data, const std::string& _key, rapidjson::Value& _value);

	// Camera
	/***************************************************************************/
	/*!
	\brief
		Template specialization for deserializing a Camera

	\param _data
		reference to the Camera to deserialize to

	\param _key
		the key of the value to deserialize from

	\param _value
		reference to the rapidjson::Value to deserialize from

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Deserialize<Camera>(Camera& _data, const std::string& _key, rapidjson::Value& _value);

	// Sorting Group
	/***************************************************************************/
	/*!
	\brief
		Template specialization for deserializing a SortingGroup

	\param _data
		reference to the SortingGroup to deserialize to

	\param _key
		the key of the value to deserialize from

	\param _value
		reference to the rapidjson::Value to deserialize from

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Deserialize<SortingGroup>(SortingGroup& _data, const std::string& _key, rapidjson::Value& _value);

	// Audio Source
	/***************************************************************************/
	/*!
	\brief
		Template specialization for deserializing an AudioSource component

	\param _data
		reference to the AudioSource component to deserialize to

	\param _key
		the key of the value to deserialize from

	\param _value
		reference to the rapidjson::Value to deserialize from

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Deserialize<AudioSource>(AudioSource& _data, const std::string& _key, rapidjson::Value& _value);

	// Animation
	/***************************************************************************/
	/*!
	\brief
		Template specialization for deserializing an Animator

	\param _data
		reference to the Animator to deserialize to

	\param _key
		the key of the value to deserialize from

	\param _value
		reference to the rapidjson::Value to deserialize from

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Deserialize<Animator>(Animator& _data, const std::string& _key, rapidjson::Value& _value);
	/***************************************************************************/
	/*!
	\brief
		Template specialization for deserializing a Animation

	\param _data
		reference to the Animation to deserialize to

	\param _key
		the key of the value to deserialize from

	\param _value
		reference to the rapidjson::Value to deserialize from

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Deserialize<Animation>(Animation& _data, const std::string& _key, rapidjson::Value& _value);

	// Physics
	/***************************************************************************/
	/*!
	\brief
		Template specialization for deserializing an AABB struct

	\param _data
		reference to the AABB struct to deserialize to

	\param _key
		the key of the value to deserialize from

	\param _value
		reference to the rapidjson::Value to deserialize from

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Deserialize<AABB>(AABB& _data, const std::string& _key, rapidjson::Value& _value);
	/***************************************************************************/
	/*!
	\brief
		Template specialization for deserializing a BoxCollider2D

	\param _data
		reference to the BoxCollider2D to deserialize to

	\param _key
		the key of the value to deserialize from

	\param _value
		reference to the rapidjson::Value to deserialize from

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Deserialize<BoxCollider2D>(BoxCollider2D& _data, const std::string& _key, rapidjson::Value& _value);
	/***************************************************************************/
	/*!
	\brief
		Template specialization for deserializing a RigidBody2D

	\param _data
		reference to the RigidBody2D to deserialize to

	\param _key
		the key of the value to deserialize from

	\param _value
		reference to the rapidjson::Value to deserialize from

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Deserialize<Rigidbody2D>(Rigidbody2D& _data, const std::string& _key, rapidjson::Value& _value);

	// Sprite
	/***************************************************************************/
	/*!
	\brief
		Template specialization for deserializing a Sprite

	\param _data
		reference to the Sprite to deserialize to

	\param _key
		the key of the value to deserialize from

	\param _value
		reference to the rapidjson::Value to deserialize from

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Deserialize<Sprite>(Sprite& _data, const std::string& _key, rapidjson::Value& _value);
	/***************************************************************************/
	/*!
	\brief
		Template specialization for deserializing a SpriteRenderer

	\param _data
		reference to the SpriteRenderer to deserialize to

	\param _key
		the key of the value to deserialize from

	\param _value
		reference to the rapidjson::Value to deserialize from

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Deserialize<SpriteRenderer>(SpriteRenderer& _data, const std::string& _key, rapidjson::Value& _value);

	// UI
	/***************************************************************************/
	/*!
	\brief
		Template specialization for deserializing a Button

	\param _data
		reference to the Button to deserialize to

	\param _key
		the key of the value to deserialize from

	\param _value
		reference to the rapidjson::Value to deserialize from

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Deserialize<Button>(Button& _data, const std::string& _key, rapidjson::Value& _value);
	/***************************************************************************/
	/*!
	\brief
		Template specialization for deserializing a Text component

	\param _data
		reference to the Text component to deserialize to

	\param _key
		the key of the value to deserialize from

	\param _value
		reference to the rapidjson::Value to deserialize from

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Deserialize<Text>(Text& _data, const std::string& _key, rapidjson::Value& _value);
	/***************************************************************************/
	/*!
	\brief
		Template specialization for deserializing an Image component

	\param _data
		reference to the Image component to deserialize to

	\param _key
		the key of the value to deserialize from

	\param _value
		reference to the rapidjson::Value to deserialize from

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Deserialize<Image>(Image& _data, const std::string& _key, rapidjson::Value& _value);

	/***************************************************************************/
	/*!
	\brief
		Template specialization for deserializing a Script

	\param _data
		reference to the Script to deserialize to

	\param _key
		the key of the value to deserialize from

	\param _value
		reference to the rapidjson::Value to deserialize from

	\return
		void
	*/
	/***************************************************************************/
	template<>
	void Deserialize<Script>(Script& _data, const std::string& _key, rapidjson::Value& _value);
	
	/***************************************************************************/
	/*!
	\brief
		Deserialize a component based on type specified
		(Deprecated)

	\param _type
		the type of component

	\param _data
		ptr to the component to deserialize to

	\param _value
		reference to the rapidjson::Value to deserialize from

	\return
		void
	*/
	/***************************************************************************/
	void DeserializeComponent(ComponentType _type, Component* _data, rapidjson::Value& _value);

	/***************************************************************************/
	/*!
	\brief
		Perform linkages for game objects

	\param _data
		reference to the GameObject to link

	\param _value
		the value that contains the gameobject's data

	\return
		void
	*/
	/***************************************************************************/
	void DeserializeLink(GameObject& _data, rapidjson::Value& _value);
	/***************************************************************************/
	/*!
	\brief
		Perform linkages for scripts

	\param _data
		reference to the Script to deserialize to

	\param _value
		reference to the rapidjson::Value to deserialize from

	\return
		void
	*/
	/***************************************************************************/
	void DeserializeLink(Script& _data,rapidjson::Value& _value);
	/***************************************************************************/
	/*!
	\brief
		perform linkages for buttons

	\param _data
		reference to the Button to deserialize to

	\param _value
		reference to the rapidjson::Value to deserialize from

	\return
		void
	*/
	/***************************************************************************/
	void DeserializeLink(Button& _data, rapidjson::Value& _value);
}


#endif 

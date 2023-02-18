/*!***************************************************************************************
\file			game-object-factory.h
\project
\author			Matthew Lau

\par			Course: GAM200
\par			Section:
\date			18/09/2022

\brief
	Contains declarations for the GameObjectFactory class.
	The GameObjectFactory is responsible for the creation, management and destruction of game objects.
	Note: The GameObjectFactory will also place the created gameobjects inside the current scene.

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/

#ifndef GAME_OBJECT_FACTORY_H
#define GAME_OBJECT_FACTORY_H
#include <CopiumCore/system-interface.h>

#define MyGOF (*Copium::GameObjectFactory::Instance())

namespace Copium {
	class GameObject;
	class Scene;
	class UUID;

	class GameObjectCreator {

	};

	class Default : public GameObjectCreator
	{

	};
	

	class GameObjectFactory : public Singleton<GameObjectFactory>
	{
		
	public:
		GameObjectFactory();
		~GameObjectFactory();

		/*******************************************************************************
		/*!
		*
		\brief
			Builds a game object with default data

		\return
			pointer to the new game object
		*/
		/*******************************************************************************/
		GameObject& Instantiate();

		/*******************************************************************************
		/*!
		*
		\brief
			ASSIGNS new ID and builds a game object that is a copy of specified game object.
			Note: if the specified game object has a family tree, the whole tree is duplicated.

		\return
			pointer to the new game object (head of the tree)
		*/
		/*******************************************************************************/
		GameObject& Instantiate(GameObject& _src, bool copyID = false);

		// Set up for future
		//GameObject* instantiate(prefab);

		/*******************************************************************************
		/*!
		*
		\brief
			Builds a game object from a rapidjson::Value
			Note: Value provided should be a game object value

		\return
			pointer to the new game object
		*/
		/*******************************************************************************/
		//GameObject* Instantiate(rapidjson::Value& _value);

		/*******************************************************************************
		/*!
		*
		\brief
			Destroy a gameobject

		\param _go
			Ptr to the game object that is to be destroyed.
			Note: the game object must exist in the current scene.
		*/
		/*******************************************************************************/
		void Destroy(GameObject& _go);

		/*******************************************************************************
		/*!
		*
		\brief
			Destroy a gameobject by ID

		\param id
			ID of gameobject to be destroyed
		*/
		/*******************************************************************************/
		void Destroy(UUID uuid);

		/*******************************************************************************
		/*!
		*
		\brief
			Build an archetype and place it into the gameObjectCreators map

		\param _value
			reference to the rapidjson value which should contain the archetype game object

		\return
			if successful, return pointer to the newly created game object
			if there were any errors in the process, return nullptr

		*/
		/*******************************************************************************/
		//GameObject* build_archetype(rapidjson::Value& _value);

		/*******************************************************************************
		/*!
		*
		\brief
			Build a game object using an archetype registered to the GOF

		\param _archetype
			read-only reference to a string which contains the key of the archetype that is to be created

		\return
			if successful, return ptr to the newly created game object
			if there were any errors in the process, return nullptr

		*/
		/*******************************************************************************/
		GameObject& Instantiate(const std::string& _archetype);

		/*******************************************************************************
		/*!
		*
		\brief
			Iterates through the filepath that is specified and creates an archetype game object for each archetype json found in the folder.
			Note: user must guarantee that the path provided is correct and that the files contain the correct format for a single game object

		\param _directoryPath
			read-only reference to the file path of the directory containing all the archetype files

		\return
			if successful, return true
			if there were any errors in the process, return false

		*/
		/*******************************************************************************/
		bool register_archetypes(const std::filesystem::path& _directoryPath);

		/*******************************************************************************
		/*!
		*
		\brief
			Clear the memory allocated to the Original Archetype templates

		\return
			void
		*/
		/*******************************************************************************/
		void clear_archetypes();

		/*******************************************************************************
		/*!
		*
		\brief
			Get reference of the archetype map

		\return
			reference to the archetype map

		*/
		/*******************************************************************************/
		std::map<std::string, GameObject*>& get_archetype_map();

		//M3

		/*******************************************************************************
		/*!
		*
		\brief
			Create a default child game object and attach it to the specified game object which will be the parent

		\param _parent
			reference to the parent of the newly created child game object

		\return
			ptr to thew newly created child game object

		*/
		/*******************************************************************************/
		GameObject* create_child(GameObject& _parent);


	private:
		std::map<std::string, GameObject*> archetypes;
	};

	
}


#endif

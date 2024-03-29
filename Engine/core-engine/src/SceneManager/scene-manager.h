/*!***************************************************************************************
****
\file			sm.h
\project
\author			Matthew Lau
\co-authors

\par			Course: GAM250
\par			Section:
\date			21/09/2022

\brief
	Contains declarations for the SceneManager class.
	SceneManager is responsible for managing the current scene in the engine. 
	It is also in charge of:
	1. loading in scene data from a file
	2. switching to another scene
	3. de-allocation of resources used by current scene (cleanup before engine close)
	4. Calling scene's update functions

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************
****/
#ifndef SM_H
#define SM_H


#include "CopiumCore/system-interface.h"
#include "SceneManager/scene.h"
#include "Events/events-system.h"
#include <filesystem>
#include <config.h>

#define MySceneManager (*Copium::SceneManager::Instance())

namespace Copium {

	class Camera;

	CLASS_SYSTEM(SceneManager)
	{
	public:

		/*******************************************************************************
		/*!
		*
		\brief
			Find a game object by its UUID.

		\return
			the gameobject with the specified UUID
			if no game objects exists with the specified UUID, nullptr is returned
		*/
		/*******************************************************************************/
		GameObject* FindGameObjectByID(UUID _id);

		/*******************************************************************************
		/*!
		*
		\brief
			Find a component by its UUID.

		\return
			the component with the specified UUID
			if no component exists with the specified UUID, nullptr is returned
		*/
		/*******************************************************************************/
		template <typename T>
		T* FindComponentByID(UUID _id);
		/*******************************************************************************
		/*!
		*
		\brief
			The Initialization function for SceneManager ISystem

		\return
			void
		*/
		/*******************************************************************************/
		void init();
		/*******************************************************************************
		/*!
		*
		\brief
			The Update function for SceneManager ISystem

		\return
			void
		*/
		/*******************************************************************************/
		void update();
		/*******************************************************************************
		/*!
		*
		\brief
			The Exit function for SceneManager ISystem.
			Should do the cleanup for any resources used by this ISystem

		\return
			void
		*/
		/*******************************************************************************/
		void exit();


		/*******************************************************************************
		/*!
		*
		\brief
			Back up the current scene into storage scene and make a new scene which will be a copy of the original scene
			Note: the new current scene will be used for the preview

		\return
			void
		*/
		/*******************************************************************************/
		void backUpCurrScene();

		/*******************************************************************************
		/*!
		*
		\brief
			Load scene data from a file

		\param _filepath
			reference to a string containing the filepath of the file that contains the scene data to be loaded.

		\return
			if loading of scene data is successful, return true
			if there are errors with loading scene data, return false
		*/
		/*******************************************************************************/
		bool load_scene(const std::string& _filepath);


		/*******************************************************************************
		/*!
		*
		\brief
			save the current scene data into the file that is at filepath denoted by scenefilepath data member in the scene manager.
			Note: this function calls save_scene(const std::string& _filepath)

		\return
			if success, return true
			if there are errors, return false
		*/
		/*******************************************************************************/
		bool save_scene();
		/*******************************************************************************
		/*!
		*
		\brief
			save the current scene data into the file whose filepath is specified as a parameter

		\param	_filepath
			read-only reference to a string which contains the filepath of the file that the current scene data
			is to be saved to

		\return
			if success, return true
			if there are errors, return false
		*/
		/*******************************************************************************/
		bool save_scene(const std::string& _filepath);

		/*******************************************************************************
		/*!
		*
		\brief
			save the current scene data into the file whose filepath is specified as a parameter

		\param	_filepath
			read-only reference to a string which contains the filepath of the file that the current scene data
			is to be saved to

		\param _filename
			the filename of the scene file

		\param
			flag to check if the filename should be modified or not

		\return
			if success, return true
			if there are errors, return false
		*/
		/*******************************************************************************/
		bool save_scene(const std::string& _filepath, const std::string& _filename, bool _modifyname = false);

		/*******************************************************************************
		/*!
		*
		\brief
			Get pointer to the current scene

		\return
			pointer to the current scene
			if there is no scene loaded, return nullptr
		*/
		/*******************************************************************************/
		Scene* get_current_scene();

		/*******************************************************************************
		/*!
		*
		\brief
			Set the current scene

		\param _src
			pointer to the scene to be set as current scene

		\return
			void
		*/
		/*******************************************************************************/
		void set_current_scene(Scene* _src);

		std::shared_ptr<GameObject>& get_selected_gameobject_sptr();
		std::shared_ptr<GameObject>* find_gameobject_sptr(GameObject* _go);


		/*******************************************************************************
		/*!
		*
		\brief
			Clone the current scene and put the current scene into a scene buffer for safekeeping. The copy will made the current scene and be
			used for the preview.
			Note: this will allow the original scene to remain unmodified by the scripts as preview is just a preview

		\return
			if success, return true
			if there are errors in allocating memory for a copy of the current scene, return false
		*/
		/*******************************************************************************/
		bool startPreview();
		/*******************************************************************************
		/*!
		*
		\brief
			Free the memory allocated to the preview scene and put the unmodifed scene back to current scene

		\return
			if success, return true
			if there are errors in deallocating memory for the copy of the current scene, return false
		*/
		/*******************************************************************************/
		bool endPreview();

		/*******************************************************************************
		/*!
		*
		\brief
			Get this scene's filepath

		\return
			reference to this scene's filepath string
		*/
		/*******************************************************************************/
		std::string& get_scenefilepath();

		/*******************************************************************************
		/*!
		*
		\brief
			Create an empty scene 

		\return
			if success, return true
			if there are errors in allocating memory for the empty scene, return false
		*/
		/*******************************************************************************/
		bool create_scene();

		/*******************************************************************************
		/*!
		*
		\brief
			Get a ptr to the storage scene.

		\return
			return ptr to the storage scene 
			Note: if there is no storage scene, nullptr should be returned
		*/
		/*******************************************************************************/
		Scene* get_storage_scene();

		/*******************************************************************************
		/*!
		*
		\brief
			Get a reference to the vector of scenes

		\return
			reference to the vector of scenes
		*/
		/*******************************************************************************/
		//std::vector<Scene*>& GetSceneVector() { return scenes; }

		/*******************************************************************************
		/*!
		*
		\brief
			Perform linkages on all components that require it after deserialization.
			This entails linking images to their sprites, script game object references to the game object etc..

		\return
			void
		*/
		/*******************************************************************************/
		void DeserializeLink();
		/*******************************************************************************
		/*!
		*
		\brief
			Perform linkages on all components that require it before starting the preview.
			This entails linking images to their sprites, script game object references to the game object etc..

		\return
			void
		*/
		/*******************************************************************************/
		void PreviewLink();
		/*******************************************************************************
		/*!
		*
		\brief
			Get the current scene state

		\return
			the current scene state
		*/
		/*******************************************************************************/
		Scene::SceneState GetSceneState() const { return currSceneState; }


		template <typename T>
		void CallbackComponentAdd(ComponentAddEvent<T>* pEvent);

		template <typename T>
		void CallbackComponentDelete(ComponentDeleteEvent<T>* pEvent);


		template<typename T, typename... Ts>
		void SubscribeComponentsFunctions(TemplatePack<T,Ts...> pack)
		{
			MyEventSystem->subscribe(this, &SceneManager::CallbackComponentAdd<T>);
			MyEventSystem->subscribe(this, &SceneManager::CallbackComponentDelete<T>);
			if constexpr (sizeof...(Ts) != 0)
			{
				SubscribeComponentsFunctions(TemplatePack<Ts...>());
			}
		}

		template <typename T>
		bool DelinkComponent(T*& container);

		void DelinkDeleted();

		Camera* mainCamera{nullptr};
		std::string sceneFile{};

		bool inPlayMode = false;

	private:
		void CallbackQuitEngine(QuitEngineEvent* pEvent);
		void CallbackChildInstantiate(ChildInstantiateEvent* pEvent);
		void CallbackGameObjectInstantiate(GameObjectInstantiateEvent* pEvent);
		void CallbackGameObjectDestroy(GameObjectDestroyEvent* pEvent);
	private:
		Scene* currentScene;	// Pointer to the current scene
		Scene* storageScene;	// Scene Pointer that acts as buffer for preview scene
		rapidjson::Document document;
		std::string sceneFilePath;
		Scene::SceneState currSceneState{ Scene::SceneState::edit };
	};

	/*******************************************************************************
	/*!
	*
	\brief
		Helper function for creating a rapidjson string and placing it inside a rapidjson value

	\param	_doc
		reference to the rapidjson document associated with the save file

	\param _value
		reference to the rapidjson value which is to hold the string

	\param _str
		read-only reference to the string that is to be made into a rapidjson string

	\return
		void
	*/
	/*******************************************************************************/
	void create_rapidjson_string(rapidjson::Document& _doc, rapidjson::Value& _value, const std::string& _str);


}

#endif

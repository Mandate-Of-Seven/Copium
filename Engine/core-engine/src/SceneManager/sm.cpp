/*!***************************************************************************************
****
\file			sm.cpp
\project
\author			Matthew Lau
\co-authors

\par			Course: GAM200
\par			Section:
\date			21/09/2022

\brief
	Contains definitions for the NewSceneManager class.
	NewSceneManager is responsible for managing the current scene in the engine.
	It is also in charge of:
	1. loading in scene data from a file
	2. switching to another scene
	3. de-allocation of resources used by current scene (cleanup before engine close)
	4. Calling scene's update functions

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************
****/
#include <pch.h>
#include "SceneManager/sm.h"
#include "Graphics/graphics-system.h"
#include "Windows/windows-system.h"
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>
#include "GameObject/Components/ui-components.h"

namespace Copium {

	std::string prefix("../PackedTracks/Assets/Scenes/");

	GameObject* NewSceneManager::findGameObjByID(GameObjectID _ID)
	{
		for (GameObject* pGameObj : currentScene->gameObjects)
		{
			if (pGameObj->id == _ID)
			{
				return pGameObj;
			}
		}
		return nullptr;
	}
	GameObject* NewSceneManager::findGameObjByName(const std::string& name)
	{
		for (GameObject* pGameObj : currentScene->gameObjects)
		{
			if (pGameObj->get_name() == name)
			{
				return pGameObj;
			}
		}
		return nullptr;
	}

	Component* NewSceneManager::findComponentByID(ComponentID _ID)
	{
		for (GameObject* pGameObj : currentScene->gameObjects)
		{
			for (Component* pComponent : pGameObj->components)
			{	
				if (pComponent->id == _ID)
				{
					return pComponent;
				}
			}
		}
		return nullptr;
	}

	NewSceneManager::NewSceneManager() : currentScene{nullptr}, selectedGameObject{nullptr}, storageScene{nullptr}, currSceneState{Scene::SceneState::edit}
	{
	}

	NewSceneManager::~NewSceneManager()
	{


		//std::cout << "new scene manager destruction called\n";

	}

	void NewSceneManager::init()
	{
		// Load default scene

		systemFlags |= FLAG_RUN_ON_EDITOR | FLAG_RUN_ON_PLAY;
		storageScene = nullptr;
		MyGOF.register_archetypes("Data/Archetypes");
		//load_scene(prefix + "Demo.scene");


		//std::cout << "No. of GameObjects in scene:" << currentScene->get_gameobjcount() << std::endl;
	}
	void NewSceneManager::update()
	{

	}
	void NewSceneManager::exit()
	{
		selectedGameObject = nullptr;

		if (currentScene)
		{
			delete currentScene;
			currentScene = nullptr;
		}
		if (storageScene)
		{
			delete storageScene;
			storageScene = nullptr;
		}

		
	}

	bool NewSceneManager::load_scene(const std::string& _filepath)
	{
		std::cout << "loading " << _filepath << std::endl;

		if (_filepath.find(".scene") == std::string::npos)
		{
			Window::EditorConsole::editorLog.add_logEntry("file selected is not a Copium Scene!");
			return false;
		}
		
		if (!currentScene)
		{
			currentScene = new NormalScene(_filepath);
		}

		sceneFilePath = _filepath;

		std::ifstream ifs(_filepath);
		if (!ifs)
			return false;

		rapidjson::IStreamWrapper isw(ifs);
		if (document.ParseStream(isw).HasParseError())
		{
			ifs.close();
			return false;
		}

		// WAIT
		MessageSystem::Instance()->dispatch(MESSAGE_TYPE::MT_SCENE_OPENED);

		if (document.HasMember("Name"))
		{
			currentScene->set_name(document["Name"].GetString());
		}

		if (document.HasMember("Unused GIDs"))
		{
			std::cout << "Adding unused gids: ";
			rapidjson::Value& _arr = document["Unused GIDs"].GetArray();
			for (rapidjson::Value::ValueIterator iter = _arr.Begin(); iter != _arr.End(); ++iter)
			{
				GameObjectID id = (*iter).GetUint64();
				currentScene->add_unused_gid(id);
				std::cout << id << ' ';
			}
			std::cout << std::endl;
		}
		if (document.HasMember("Unused CIDs"))
		{
			rapidjson::Value& arr = document["Unused CIDs"].GetArray();
			for (rapidjson::Value::ValueIterator iter = arr.Begin(); iter != arr.End(); ++iter)
			{
				ComponentID id = (*iter).GetUint64();
				currentScene->add_unused_cid(id);
				std::cout << id << ' ';
			}
			std::cout << std::endl;

		}

		if (document.HasMember("GameObjects"))
		{
			rapidjson::Value& _gameObjArr = document["GameObjects"].GetArray();
			for (rapidjson::Value::ValueIterator iter = _gameObjArr.Begin(); iter != _gameObjArr.End(); ++iter)
			{
				GameObject* tmpGO = nullptr;
				tmpGO = MyGOF.instantiate(*iter);
			}

			// Linkage of components to each other
			auto gameObjectIt = _gameObjArr.Begin();
			for (GameObject* go : currentScene->gameObjects)
			{
				if ((*gameObjectIt).HasMember("Components"))
				{
					rapidjson::Value& compArr = (*gameObjectIt)["Components"].GetArray();
					auto componentIt = compArr.Begin();
					for (Component* component : go->components)
					{
						//Offset TransformComponent
						++componentIt;
						component->deserializeLink(*componentIt);
					}
				}
				++gameObjectIt;
			}
		}
		
		ifs.close();


		MessageSystem::Instance()->dispatch(MESSAGE_TYPE::MT_SCENE_DESERIALIZED);

		return true;

	}
	bool NewSceneManager::change_scene(const std::string& _newfilepath)
	{
		bool result = false;

		// No scene loaded, therefore cannot change
		if (!currentScene)
		{
			return result;
		}

		// Clear game objects in scene
		// Deserialize from new file and overwrite other scene data
		if (std::filesystem::exists(_newfilepath))
		{
			std::cout << "file exists\n";
			delete currentScene;
			currentScene = nullptr;
			selectedGameObject = nullptr;
			load_scene(_newfilepath);
			//Scene* tmp = currentScene;
			//result = load_scene(_newfilepath);
			//delete tmp;
		}
		else
		{
			std::cout << "file does not exist, scene change aborted\n";
			return false;
		}

		return result;

	}

	Scene* NewSceneManager::get_current_scene()
	{
		return currentScene;
	}
	void NewSceneManager::set_selected_gameobject(GameObject* _go) { selectedGameObject = _go; }
	GameObject* NewSceneManager::get_selected_gameobject() { return selectedGameObject; }


	// M2
	bool NewSceneManager::startPreview()
	{
		if (!currentScene)
		{
			PRINT("There is no scene to preview...\n");
			return false;
		}
		if (storageScene)
		{
			PRINT("Currently in play mode...\n");
			return false;
		}

		backUpCurrScene();


		selectedGameObject = nullptr;

		currSceneState = Scene::SceneState::play;
		currentScene->set_state(Scene::SceneState::play);

		return true;
	}
	bool NewSceneManager::endPreview()
	{
		if (!currentScene)
		{
			PRINT("There is no scene to stop preview...\n");
			return false;
		}

		// Delete memory for the preview scene
		if (!storageScene)
			return false;
		delete currentScene;

		// Swap the original unmodified scene back to current scene
		currentScene = storageScene;
		storageScene = nullptr;
		selectedGameObject = nullptr;

		currSceneState = Scene::SceneState::edit;
		currentScene->set_state(Scene::SceneState::edit);

		return true;

	}

	bool NewSceneManager::save_scene()
	{
		if (!currentScene)
		{
			PRINT("There is no scene to save...\n");
			return false;
		}

		double startTime = glfwGetTime();
		std::cout << "saving scene...\n";
		if (!save_scene(sceneFilePath))
			return false;

		std::cout << "save complete\n";
		std::cout << "Time taken: " << glfwGetTime() - startTime << std::endl;
		return true;
	}
	bool NewSceneManager::save_scene(const std::string& _filepath)
	{
		if(!currentScene)
		{
			Window::EditorConsole::editorLog.add_logEntry("No Scene loaded, cannot perform save!");
			return false;
		}

		if (currSceneState != Scene::SceneState::edit)
		{
			Window::EditorConsole::editorLog.add_logEntry("In edit mode, cannot perform save!");
			return false;
		}

		std::string fp(_filepath);
		if (fp.find(".scene") == std::string::npos)
		{
			fp += ".scene";
		}
		std::ofstream ofs(fp);
		rapidjson::Document doc;

		doc.SetObject();
		rapidjson::Value name;
		create_rapidjson_string(doc, name,  currentScene->get_name());
		doc.AddMember("Name", name, doc.GetAllocator());

		// Serialize UGIDse
		rapidjson::Value ugids(rapidjson::kArrayType);
		for (GameObjectID id : currentScene->get_unusedgids())
		{
			ugids.PushBack(id, doc.GetAllocator());
		}
		doc.AddMember("Unused GIDs", ugids, doc.GetAllocator());
		// Serialize UCIDs
		rapidjson::Value ucids(rapidjson::kArrayType);
		for (ComponentID id : currentScene->get_unusedcids())
		{
			ucids.PushBack(id, doc.GetAllocator());
		}
		doc.AddMember("Unused CIDs", ucids, doc.GetAllocator());

		std::vector<GameObject*> roots;
		for (GameObject* pGameObject : currentScene->gameObjects)
		{
			if (!pGameObject->transform.hasParent())
				roots.emplace_back(pGameObject);
		}
		//Create array of game objects
		rapidjson::Value gameObjects(rapidjson::kArrayType);
		for (GameObject* pGameObject : currentScene->gameObjects)
		{
			if (pGameObject->transform.hasParent())
				continue;

			rapidjson::Value go(rapidjson::kObjectType);
			pGameObject->serialize(go, doc);


			//rapidjson::Value components(rapidjson::kArrayType);
			// Insert transform component into component array


			gameObjects.PushBack(go, doc.GetAllocator());
		}

		doc.AddMember("GameObjects", gameObjects, doc.GetAllocator());

		rapidjson::StringBuffer sb;
		rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(sb);
		doc.Accept(writer);
		ofs << sb.GetString();
		return true;
	}

	void NewSceneManager::backUpCurrScene()
	{
		storageScene = currentScene;
		currentScene = new NormalScene();

		if (!currentScene)
		{
			currentScene = storageScene;
			storageScene = nullptr;
			return;
		}

		currentScene->unusedCIDs = storageScene->unusedCIDs;
		currentScene->unusedGIDs = storageScene->unusedGIDs;
		// Copy game object data
		for (const GameObject* gameObj : storageScene->gameObjects)
		{
			if (gameObj && !gameObj->transform.hasParent())
			{
				MyGOF.clone(*gameObj, currentScene);
			}
		}

		for (size_t goIndex{ 0 }; goIndex < storageScene->get_gameobjcount(); ++goIndex)
		{
			GameObject* currGameObj = currentScene->gameObjects[goIndex];
			GameObject* storedGameObj = storageScene->gameObjects[goIndex];
			for (size_t compIndex{ 0 }; compIndex < currGameObj->components.size(); ++compIndex)
			{
				currGameObj->components[compIndex]->previewLink(storedGameObj->components[compIndex]);
			}
		}
	}

	void create_rapidjson_string(rapidjson::Document& _doc, rapidjson::Value& _value, const std::string& _str)
	{
		rapidjson::SizeType sz = static_cast<rapidjson::SizeType>(_str.size());
		_value.SetString(_str.c_str(), sz, _doc.GetAllocator());
	}


	// M3
	std::string& NewSceneManager::get_scenefilepath() { return sceneFilePath; }
	bool NewSceneManager::create_scene()
	{
		if (currentScene) {
			delete currentScene;
			currentScene = nullptr;
			sceneFilePath.clear();
		}

		currentScene = new NormalScene();
		if (!currentScene)
			return false;

		return true;
	}
	Scene* NewSceneManager::get_storage_scene() { return storageScene; }
}

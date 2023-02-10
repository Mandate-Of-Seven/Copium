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
	Contains definitions for the SceneManager class.
	SceneManager is responsible for managing the current scene in the engine.
	It is also in charge of:
	1. loading in scene data from a file
	2. switching to another scene
	3. de-allocation of resources used by current scene (cleanup before engine close)
	4. Calling scene's update functions

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************
****/
#include <pch.h>
#include "SceneManager/scene-manager.h"
#include "Graphics/graphics-system.h"
#include "Windows/windows-system.h"
#include "Editor/editor-system.h"
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>
#include <GameObject/Components/camera-component.h>
#include "GameObject/Components/ui-components.h"
#include "GameObject/Components/sorting-group-component.h"
#include <Audio/sound-system.h>
#include <Events/events-system.h>



namespace Copium {

	namespace
	{
		EditorSystem& es{ *EditorSystem::Instance() };
	}

	std::string prefix("../PackedTracks/Assets/Scenes/");

	GameObject* SceneManager::findGameObjByID(GameObjectID _ID)
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
	GameObject* SceneManager::findGameObjByName(const std::string& name)
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
	Component* SceneManager::findComponentByID(ComponentID _ID)
	{
		for (GameObject* pGameObj : currentScene->gameObjects)
		{
			for (Component* pComponent : pGameObj->components)
			{	
				if (!pComponent)
					continue;
				if (pComponent->id == _ID)
				{
					return pComponent;
				}
			}
		}
		return nullptr;
	}

	SceneManager::SceneManager() : currentScene{nullptr}, selectedGameObject{nullptr}, storageScene{nullptr}, currSceneState{Scene::SceneState::edit}
	{
	}

	SceneManager::~SceneManager()
	{


		//std::cout << "new scene manager destruction called\n";

	}

	void SceneManager::init()
	{
		systemFlags |= FLAG_RUN_ON_EDITOR | FLAG_RUN_ON_PLAY;
		storageScene = nullptr;
		//MyGOF.register_archetypes("Data/Archetypes");
	}
	void SceneManager::update()
	{

	}
	void SceneManager::exit()
	{
		selectedGameObject = nullptr;

		if (currSceneState == Scene::SceneState::play)
		{
			endPreview();
		}

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

		// For multiple scenes
		//for (Scene* sc : scenes)
		//{
		//	if (sc)
		//	{
		//		delete sc;
		//		sc = nullptr;
		//	}
		//}

		
	}

	bool SceneManager::load_scene(const std::string& _filepath)
	{
		if (currSceneState == Scene::SceneState::play)
		{
			PRINT("CANT LOAD SCENE IN PLAY MODE");
			return false;
		}
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

		if (document.HasMember("Name"))
		{
			currentScene->set_name(document["Name"].GetString());
			std::cout << "Scene name:" << currentScene->name << std::endl;
		}

		MyEventSystem->publish(new SceneOpenedEvent(currentScene->get_name().c_str()));

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

		if (document.HasMember("Layers"))
		{
			rapidjson::Value& arr = document["Layers"].GetArray();
			if (es.getLayers()->SortLayers()->GetLayerCount())
			{
				es.getLayers()->SortLayers()->GetSortingLayers().clear();
				es.getLayers()->SortLayers()->GetSortingLayers().shrink_to_fit();
				PRINT(es.getLayers()->SortLayers()->GetSortingLayers().size());
			}

			unsigned int idx{ 0 };
			// Set-up all the layers
			for (rapidjson::Value::ValueIterator iter = arr.Begin(); iter != arr.End(); ++iter)
			{
				std::string name;
				rapidjson::Value& layer = *iter;
				if (layer.HasMember("Name"))
				{
					name = layer["Name"].GetString();
				}
				Layer* lay = es.getLayers()->SortLayers()->CreateNewLayer(name);
				PRINT("making new layer");
				if (layer.HasMember("ID"))
				{
					lay->layerID = layer["ID"].GetUint();
					PRINT("Layer's id: " << lay->layerID);
				}
				else
				{
					lay->layerID = idx;
				}

				++idx;
				
			}

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
					go->transform.deserializeLink(*componentIt);
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
		
		// Place Game Objects that are in layers into respective layers
		for (GameObject* go : currentScene->gameObjects)
		{
			bool layered{ false };
			SortingGroup* sg{ nullptr };
			for (Component* component : go->GetComponents<SortingGroup>())
			{
				if (component->Enabled())
				{
					sg = reinterpret_cast<SortingGroup*>(component);
					layered = true;
					break;
				}
			}

			if (layered)
			{
				PRINT("Layer ID: " << sg->GetLayerID());
				es.getLayers()->SortLayers()->AddGameObject(sg->GetLayerID(), *go);
			}
		}
		// Sort based on order in layer
		for (Layer& la : es.getLayers()->SortLayers()->GetSortingLayers())
		{
			bool swapped{ false };
			if (la.gameObjects.size() <= 1)
				continue;

			for (size_t i{ 0 }; i < la.gameObjects.size() - 1; ++i)
			{
				for (size_t j{ 0 }; j < la.gameObjects.size() - 1 - i; ++j)
				{
					SortingGroup* sg1{ nullptr }, * sg2{ nullptr };

					if (!la.gameObjects[j] && la.gameObjects[j+1])
					{
						std::swap(la.gameObjects[j], la.gameObjects[j + 1]);
						swapped = true;
						continue;
					}

					if (la.gameObjects[j] && la.gameObjects[j + 1])
					{
						Component* co1 = la.gameObjects[j]->GetComponent<SortingGroup>();
						Component* co2 = la.gameObjects[j + 1]->GetComponent<SortingGroup>();

						if (co1 && co2)
						{
							sg1 = reinterpret_cast<SortingGroup*>(co1);
							sg2 = reinterpret_cast<SortingGroup*>(co2);

							if (sg1->GetOrderInLayer() > sg2->GetOrderInLayer())
							{
								std::swap(la.gameObjects[j], la.gameObjects[j + 1]);
								swapped = true;
							}
						}
					}


				}

				if (!swapped)
					break;
			}
		}

		ifs.close();


		MessageSystem::Instance()->dispatch(MESSAGE_TYPE::MT_SCENE_DESERIALIZED);

		// For multiple scenes within an instance
		//scenes.emplace_back(currentScene);

		return true;

	}
	bool SceneManager::change_scene(const std::string& _newfilepath)
	{
		if (currSceneState == Scene::SceneState::play)
		{
			PRINT("CANT LOAD SCENE IN PLAY MODE");
			return false;
		}
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

	Scene* SceneManager::get_current_scene(){ return currentScene; }
	void SceneManager::set_current_scene(Scene* _src) { currentScene = _src; }

	void SceneManager::set_selected_gameobject(GameObject* _go) { selectedGameObject = _go; }
	GameObject* SceneManager::get_selected_gameobject() { return selectedGameObject; }
	std::shared_ptr<GameObject>& SceneManager::get_selected_gameobject_sptr()
	{
		for (size_t i{ 0 }; i < currentScene->gameObjectSPTRS.size(); ++i)
		{
			if (currentScene->gameObjectSPTRS[i].get() == selectedGameObject)
			{
				return currentScene->gameObjectSPTRS[i];
			}
		}
		
		return currentScene->gameObjectSPTRS[0];
	}
	std::shared_ptr<GameObject>* SceneManager::find_gameobject_sptr(GameObject* _go)
	{
		for (size_t i{ 0 }; i < currentScene->gameObjectSPTRS.size(); ++i)
		{
			if (currentScene->gameObjectSPTRS[i].get() == _go)
			{
				return &currentScene->gameObjectSPTRS[i];
			}
		}

		return nullptr;
	}

	// M2
	bool SceneManager::startPreview()
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

		MyEventSystem->publish(new StartPreviewEvent());


		GameObjectID prevSelected = 0;
		if (selectedGameObject)
			prevSelected = selectedGameObject->id;

		backUpCurrScene();

		for (GameObject* gameObj : currentScene->gameObjects)
		{
			mainCamera = gameObj->GetComponent<Camera>();
			if (mainCamera)
				break;
		}

		currSceneState = Scene::SceneState::play;
		currentScene->set_state(Scene::SceneState::play);

		//if (mainCamera == nullptr)
		//{
		//	std::cout << "start preview\n";

		//	delete currentScene;
		//	currentScene = storageScene;
		//	storageScene = 0;
		//	return false;
		//}

		if (prevSelected)
			selectedGameObject = findGameObjByID(prevSelected);

		SoundSystem::Instance()->StopAll();

		return true;
	}
	bool SceneManager::endPreview()
	{
		if (!currentScene)
		{
			PRINT("There is no scene to stop preview...\n");
			
			return false;
		}

		std::cout << "stop preview\n";
		currSceneState = Scene::SceneState::edit;


		// Delete memory for the preview scene
		if (!storageScene)
			return false;

		Scene* tmp = currentScene;
		currentScene = storageScene;
		mainCamera = nullptr;
		storageScene = nullptr;

		Button::hoveredBtn = nullptr;

		if (selectedGameObject)
		{
			selectedGameObject = findGameObjByID(selectedGameObject->id);
		}

		delete tmp;

		currentScene->set_state(Scene::SceneState::edit);

		SoundSystem::Instance()->StopAll();

		return true;

	}

	bool SceneManager::save_scene()
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
	bool SceneManager::save_scene(const std::string& _filepath)
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

		if (sceneFilePath.empty())
		{
			sceneFilePath = fp;
		}
		std::ofstream ofs(fp);
		rapidjson::Document doc;

		doc.SetObject();

		//rapidjson::Value sceneArray(rapidjson::kArrayType);
		//for (Scene* sc : scenes)
		//{
		//	rapidjson::Value scene(rapidjson::kObjectType);
		//	rapidjson::Value name;
		//	create_rapidjson_string(doc, name, sc->get_name());
		//	scene.AddMember("Name", name, doc.GetAllocator());

		//	// Serialize UGIDs
		//	rapidjson::Value ugids(rapidjson::kArrayType);
		//	for (GameObjectID id : sc->get_unusedgids())
		//	{
		//		ugids.PushBack(id, doc.GetAllocator());
		//	}
		//	scene.AddMember("Unused GIDs", ugids, doc.GetAllocator());

		//	// Serialize UCIDs
		//	rapidjson::Value ucids(rapidjson::kArrayType);
		//	for (ComponentID id : sc->get_unusedcids())
		//	{
		//		ucids.PushBack(id, doc.GetAllocator());
		//	}
		//	scene.AddMember("Unused CIDs", ucids, doc.GetAllocator());

		//	std::vector<GameObject*> roots;
		//	for (GameObject* pGameObject : sc->gameObjects)
		//	{
		//		if (!pGameObject->transform.hasParent())
		//			roots.emplace_back(pGameObject);
		//	}
		//	//Create array of game objects
		//	rapidjson::Value gameObjects(rapidjson::kArrayType);
		//	for (GameObject* pGameObject : sc->gameObjects)
		//	{
		//		rapidjson::Value go(rapidjson::kObjectType);
		//		pGameObject->serialize(go, doc);


		//		//rapidjson::Value components(rapidjson::kArrayType);
		//		// Insert transform component into component array


		//		gameObjects.PushBack(go, doc.GetAllocator());
		//	}

		//	scene.AddMember("GameObjects", gameObjects, doc.GetAllocator());
		//	sceneArray.PushBack(scene, doc.GetAllocator());
		//}
		//doc.AddMember("Scenes:", sceneArray, doc.GetAllocator());
		rapidjson::Value name;
		create_rapidjson_string(doc, name,  currentScene->get_name());
		doc.AddMember("Name", name, doc.GetAllocator());

		// Serialize UGIDs
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

		// Serialize Layer Data
		rapidjson::Value layers(rapidjson::kArrayType);
		for (Layer& layer : es.getLayers()->SortLayers()->GetSortingLayers())
		{
			rapidjson::Value obj(rapidjson::kObjectType);
			rapidjson::Value layerName;
			create_rapidjson_string(doc, layerName, layer.name);
			obj.AddMember("Name", layerName, doc.GetAllocator());

			obj.AddMember("ID", layer.layerID, doc.GetAllocator());

			layers.PushBack(obj, doc.GetAllocator());

		}
		doc.AddMember("Layers", layers, doc.GetAllocator());

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

	void SceneManager::backUpCurrScene()
	{
		storageScene = currentScene;
		currentScene = new NormalScene();

		if (!currentScene)
		{
			currentScene = storageScene;
			storageScene = nullptr;
			return;
		}

		currentScene->name = storageScene->name;

		currentScene->unusedCIDs = storageScene->unusedCIDs;
		currentScene->unusedGIDs = storageScene->unusedGIDs;

		// Copy game object data
		for (GameObject* gameObj : storageScene->gameObjects)
		{
			MyGOF.clone(*gameObj, currentScene);
		}

		for (size_t goIndex{ 0 }; goIndex < storageScene->get_gameobjcount(); ++goIndex)
		{
			GameObject* currGameObj = currentScene->gameObjects[goIndex];
			GameObject* storedGameObj = storageScene->gameObjects[goIndex];
			currGameObj->transform.previewLink(&storedGameObj->transform);
			//std::cout << "Name comparisons: " << currGameObj->get_name() << '|' << storedGameObj->get_name() << std::endl;
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
	std::string& SceneManager::get_scenefilepath() { return sceneFilePath; }
	bool SceneManager::create_scene()
	{
		// If there is a scene loaded, 
		if (currentScene) {
			std::cout << "Destroying Current Scene!\n";
			delete currentScene;
			currentScene = nullptr;
			selectedGameObject = nullptr;
			sceneFilePath.clear();
		}

		// Create default scene
		currentScene = new NormalScene();
		if (!currentScene)
			return false;

		return true;
	}
	Scene* SceneManager::get_storage_scene() { return storageScene; }
}

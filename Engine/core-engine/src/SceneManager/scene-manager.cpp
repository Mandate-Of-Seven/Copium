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

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************
****/
#include <pch.h>
#include "SceneManager/scene-manager.h"
//#include "Graphics/graphics-system.h"
//#include "Windows/windows-system.h"
#include "Editor/editor-system.h"
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/prettywriter.h>
#include <GameObject/game-object-factory.h>
#include <SceneManager/state-manager.h>
//#include <Audio/sound-system.h>
#include <Events/events-system.h>

#include "Utilities/json-utilities.h"
#include "SceneManager/scene-manager.h"
#include "Utilities/serializer.h"

#include "Files/assets-system.h"

namespace Copium 
{
	std::string prefix("../PackedTracks/Assets/Scenes/");

	GameObject* SceneManager::FindGameObjectByID(UUID _id)
	{
		if (!currentScene)
			return nullptr;

		for (GameObject& go : currentScene->gameObjects)
		{
			if (go.uuid == _id)
				return &go;
		}
		return nullptr;
	}
	Component* SceneManager::FindComponentByID(UUID _id)
	{


		if (!currentScene)
			return nullptr;

		return currentScene->componentArrays.FindByUUID(_id);
	}

	void SceneManager::init()
	{
		systemFlags |= FLAG_RUN_ON_EDITOR | FLAG_RUN_ON_PLAY;
		storageScene = nullptr;
		//MyGOF.register_archetypes("Data/Archetypes");
		SubscribeComponentsFunctions(ComponentTypes());
		MyEventSystem->subscribe(this, &SceneManager::CallbackQuitEngine);
		MyEventSystem->subscribe(this, &SceneManager::CallbackChildInstantiate);
		MyEventSystem->subscribe(this, &SceneManager::CallbackGameObjectInstantiate);
	}

	template <typename T, typename... Ts>
	struct CleanerStruct
	{
		Scene& scene;
		CleanerStruct(TemplatePack<T, Ts...> pack){}
		CleanerStruct(Scene& _scene) : scene{_scene}
		{
			CleanComponents<T,Ts...>();
		}
		template <typename T1, typename... T1s>
		void CleanComponents()
		{
			for (T1* pComponent : scene.componentsForDeletion.GetArray<T1>())
			{
				MyGOF.RemoveComponent(*pComponent,scene);
			}
			scene.componentsForDeletion.GetArray<T1>().clear();
			if constexpr (sizeof...(T1s) != 0)
			{
				CleanComponents<T1s...>();
			}
		}
		template <typename T1, typename... T1s>
		void CleanGameObjects()
		{
			//iterate through an array for deletion of game object
			//if found, call remove
			for (auto& gameobj : scene.gameObjectsForDeletion)
			{
				for (T1* pComponent : gameobj->componentPtrArrays.GetArray<T1>())
				{
					MyGOF.RemoveComponent(*pComponent, scene);
				}
				
			}
			if constexpr (sizeof...(T1s) != 0)
			{
				CleanGameObjects<T1s...>();
			}
			else
			{
				for (auto& gameobj : scene.gameObjectsForDeletion)
				{

					MyGOF.Destroy(gameobj, scene.gameObjects);

				}
				scene.gameObjectsForDeletion.clear();
			}

		}
	};

	using CleanUpSceneStruct = decltype(CleanerStruct(ComponentTypes()));


	void CleanUpScene(Scene& scene)
	{
		CleanUpSceneStruct cleanUp(scene);
	}

	void SceneManager::update() 
	{
		if (currentScene)
			CleanUpScene(*currentScene);
	}
	void SceneManager::exit()
	{
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

		/*
		// For multiple scenes
		//for (Scene* sc : scenes)
		//{
		//	if (sc)
		//	{
		//		delete sc;
		//		sc = nullptr;
		//	}
		//}
		*/	
	}

	bool SceneManager::load_scene(const std::string& _filepath)
	{
		std::cout << "loading " << _filepath << std::endl;

		if (_filepath.find(".scene") == std::string::npos)
		{
			MyEventSystem->publish(new EditorConsoleLogEvent("File selected is not a Copium Scene!"));
			return false;
		}

		if (currentScene)
		{
			delete currentScene;
		}
		currentScene = new NormalScene(_filepath);

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

		MyEventSystem->publish(new SceneChangingEvent());
		//MyEventSystem->publish(
		//	new SceneOpenedEvent(
		//		currentScene->get_name().c_str(),
		//		currentScene->gameObjects,
		//		currentScene->componentArrays
		//	)
		//);

		if (document.HasMember("Layers"))
		{
			rapidjson::Value& arr = document["Layers"].GetArray();
			if (MyEditorSystem.getLayers()->SortLayers()->GetLayerCount())
			{
				MyEditorSystem.getLayers()->SortLayers()->GetSortingLayers().clear();
				MyEditorSystem.getLayers()->SortLayers()->GetSortingLayers().shrink_to_fit();
				PRINT(MyEditorSystem.getLayers()->SortLayers()->GetSortingLayers().size());
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
				Layer* lay = MyEditorSystem.getLayers()->SortLayers()->CreateNewLayer(name);
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
				MyGOF.Instantiate(*iter,*currentScene);
			}

			// Linkages
			for (GameObject& go : currentScene->gameObjects)
			{
				// Transform and Parent
				if (go.transform.pid.GetUUID())
				{
					GameObject* p = MySceneManager.FindGameObjectByID(go.transform.pid);
					if (p)
						go.transform.SetParent(&p->transform);

				}
				
				// Place into respective sorting layer
				if (go.HasComponent<SortingGroup>())
				{
					SortingGroup* sg = go.GetComponent<SortingGroup>();
					MyEditorSystem.getLayers()->SortLayers()->AddGameObject(sg->sortingLayer, go);
				}

				// Sprite Renderer
				if (go.HasComponent<SpriteRenderer>())
				{
					SpriteRenderer* sr = go.GetComponent<SpriteRenderer>();
					if (sr->sprite.spriteID != 0)
					{
						const std::vector<Texture>& textures = MyAssetSystem.GetTextures();
						bool reference = false;
						for (int i = 0; i < textures.size(); i++)
						{
							uint64_t pathID = std::hash<std::string>{}(textures[i].get_file_path());
							MetaID metaID = MyAssetSystem.GetMetaID(pathID);
							// Check if the uuid of the sprite is the same as the meta file
							if (metaID.uuid == sr->sprite.spriteID)
							{
								// If so set the reference texture to that file
								reference = true;
								sr->sprite.refTexture = MyAssetSystem.GetTexture(i);
								break;
							}
						}
						// If there is no references, set the spriteID to 0
						if (!reference)
							sr->sprite.spriteID = 0;
					}
				}

				// SpriteSheet
				if (go.HasComponent<Animator>())
				{
					Animator* animator = go.GetComponent<Animator>();
					for (Animation& anim : animator->get_animation_vector())
					{
						uint64_t sid = anim.spriteSheet.spriteID;
						if (sid != 0)
						{
							const std::vector<Texture>& textures = MyAssetSystem.GetTextures();
							bool reference = false;
							for (int j = 0; j < textures.size(); j++)
							{
								uint64_t pathID = std::hash<std::string>{}(textures[j].get_file_path());
								MetaID metaID = MyAssetSystem.GetMetaID(pathID);

								// Check if the uuid of the sprite is the same as the meta file
								if (metaID.uuid == sid)
								{
									// If so set the reference texture to that file
									reference = true;
									anim.spriteSheet.texture = MyAssetSystem.GetTexture(j);
									break;
								}
							}

							// If there is no references, set the spriteID to 0
							if (!reference)
								sid = 0;
						}
					}
				}

				// Target Graphic
				if (go.HasComponent<Button>())
				{
					Button* button = go.GetComponent<Button>();
					if (button->graphicID)
						button->targetGraphic = reinterpret_cast<IUIComponent*>(FindComponentByID(button->graphicID));
				}

				if (go.HasComponent<Script>())
				{
					Script* script = go.GetComponent<Script>();
					if (!script->fieldDataReferences.empty())
						for (auto it = script->fieldDataReferences.begin(); it != script->fieldDataReferences.end(); ++it)
						{
							const std::string& _name{ it->first };
							Field& field = it->second;
							FieldType fType = field.fType;

							switch (fType)
							{
							case FieldType::Component:
							{
								uint64_t id{ field.Get<uint64_t>() };
								Component* pComponent = FindComponentByID(id);
								if (pComponent)
									script->fieldComponentReferences[_name] = pComponent;
								break;
							}
							case FieldType::GameObject:
							{
								uint64_t id{ field.Get<uint64_t>() };
								GameObject* pGameObject = FindGameObjectByID(id);
								if (pGameObject)
									script->fieldGameObjReferences[_name] = pGameObject;
								break;
							}
							}
						}
				}

			}

		}
		MyEditorSystem.getLayers()->SortLayers()->BubbleSortGameObjects();

		ifs.close();

		MessageSystem::Instance()->dispatch(MESSAGE_TYPE::MT_SCENE_DESERIALIZED);

		return true;

	}


	Scene* SceneManager::get_current_scene(){ return currentScene; }
	void SceneManager::set_current_scene(Scene* _src) { currentScene = _src; }

	std::shared_ptr<GameObject>& SceneManager::get_selected_gameobject_sptr()
	{
		for (size_t i{ 0 }; i < currentScene->gameObjectSPTRS.size(); ++i)
		{
			//if (currentScene->gameObjectSPTRS[i].get() == selectedGameObject)
			//{
			//	return currentScene->gameObjectSPTRS[i];
			//}
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
		for (GameObject& gameObj : currentScene->gameObjects)
		{
			mainCamera = gameObj.GetComponent<Camera>();
			if (mainCamera)
				break;
		}
		if (!mainCamera)
		{
			PRINT("No camera!");
			return false;

		}

		MyEventSystem->publish(new StartPreviewEvent());


		//UUID prevSelected = 0;
		//if (selectedGameObject)
		//	prevSelected = selectedGameObject->id;

		backUpCurrScene();

		for (GameObject& gameObj : currentScene->gameObjects)
		{
			mainCamera = gameObj.GetComponent<Camera>();
			if (mainCamera)
				break;
		}

		for (GameObject& go : currentScene->gameObjects)
		{
			// Transform and Parent
			if (go.transform.pid.GetUUID())
			{
				GameObject* p = MySceneManager.FindGameObjectByID(go.transform.pid);
				if (p)
					go.transform.SetParent(&p->transform);

			}

			// Replacement
			if (go.HasComponent<SortingGroup>())
			{
				SortingGroup* sg = go.GetComponent<SortingGroup>();
				MyEditorSystem.getLayers()->SortLayers()->ReplaceGameObject(sg->sortingLayer, go);

			}

			// Target Graphic
			if (go.HasComponent<Button>())
			{
				Button* button = go.GetComponent<Button>();
				if (button->graphicID)
					button->targetGraphic = reinterpret_cast<IUIComponent*>(FindComponentByID(button->graphicID));
			}

			if (go.HasComponent<Script>())
			{
				Script* script = go.GetComponent<Script>();
				if (!script->fieldDataReferences.empty())
					for (auto it = script->fieldDataReferences.begin(); it != script->fieldDataReferences.end(); ++it)
					{
						const std::string& _name{ it->first };
						Field& field = it->second;
						FieldType fType = field.fType;

						switch (fType)
						{
						case FieldType::Component:
						{
							uint64_t id{ field.Get<uint64_t>() };
							Component* pComponent = FindComponentByID(id);
							if (pComponent)
								script->fieldComponentReferences[_name] = pComponent;
							break;
						}
						case FieldType::GameObject:
						{
							uint64_t id{ field.Get<uint64_t>() };
							GameObject* pGameObject = FindGameObjectByID(id);
							if (pGameObject)
								script->fieldGameObjReferences[_name] = pGameObject;
							break;
						}
						}
					}
			}

		}

		currSceneState = Scene::SceneState::play;
		currentScene->set_state(Scene::SceneState::play);


		//if (prevSelected)
		//	selectedGameObject = findGameObjByID(prevSelected);

		//SoundSystem::Instance()->StopAll();

		return true;
	}

	bool SceneManager::endPreview()
	{
		if (!currentScene)
		{
			PRINT("There is no scene to stop preview...\n");
			
			return false;
		}

		MyEventSystem->publish(new StopPreviewEvent());

		std::cout << "stop preview\n";
		currSceneState = Scene::SceneState::edit;


		// Delete memory for the preview scene
		if (!storageScene)
			return false;

		// Replace gameobjects in sorting layer
		MyEditorSystem.getLayers()->SortLayers()->ClearAllLayer();
		for (GameObject& go : storageScene->gameObjects)
		{
			if (go.HasComponent<SortingGroup>())
			{
				SortingGroup* sg = go.GetComponent<SortingGroup>();
				MyEditorSystem.getLayers()->SortLayers()->AddGameObject(sg->GetLayerID(), go);
			}
		}
		// Sort based on order in layer
		MyEditorSystem.getLayers()->SortLayers()->BubbleSortGameObjects();

		Scene* tmp = currentScene;
		currentScene = storageScene;
		mainCamera = nullptr;
		storageScene = nullptr;

		//if (selectedGameObject)
		//{
		//	selectedGameObject = FindGameObjByID(selectedGameObject->id);
		//}

		delete tmp;

		currentScene->set_state(Scene::SceneState::edit);

		//SoundSystem::Instance()->StopAll();

		return true;

	}

	bool SceneManager::save_scene()
	{
		if (!currentScene)
		{
			PRINT("There is no scene to save...\n");
			return false;
		}

		//double startTime = glfwGetTime();
		std::cout << "saving scene...\n";
		if (!save_scene(sceneFilePath))
			return false;

		std::cout << "save complete\n";
		//std::cout << "Time taken: " << glfwGetTime() - startTime << std::endl;
		return true;
	}

	bool SceneManager::save_scene(const std::string& _filepath)
	{
		//if(!currentScene)
		//{
		//	Window::EditorConsole::editorLog.add_logEntry("No Scene loaded, cannot perform save!");
		//	return false;
		//}

		//if (currSceneState != Scene::SceneState::edit)
		//{
		//	Window::EditorConsole::editorLog.add_logEntry("In edit mode, cannot perform save!");
		//	return false;
		//}

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

		Copium::SerializeBasic(currentScene->get_name(), doc, doc, "Name");

		//// Serialize Layer Data
		//rapidjson::Value layers(rapidjson::kArrayType);
		//for (Layer& layer : MyEditorSystem.getLayers()->SortLayers()->GetSortingLayers())
		//{
		//	rapidjson::Value obj(rapidjson::kObjectType);
		//	rapidjson::Value layerName;
		//	create_rapidjson_string(doc, layerName, layer.name);
		//	obj.AddMember("Name", layerName, doc.GetAllocator());

		//	obj.AddMember("ID", layer.layerID, doc.GetAllocator());

		//	layers.PushBack(obj, doc.GetAllocator());
		// Serialize Layer Data
		rapidjson::Value layers(rapidjson::kArrayType);
		for (Layer& layer : MyEditorSystem.getLayers()->SortLayers()->GetSortingLayers())
		{
			rapidjson::Value obj(rapidjson::kObjectType);
			SerializeBasic(layer.name, obj, doc, "Name");
			SerializeBasic(layer.layerID, obj, doc, "ID");

			layers.PushBack(obj, doc.GetAllocator());
		}
		doc.AddMember("Layers", layers, doc.GetAllocator());

		
		//Create array of game objects
		rapidjson::Value gameObjects(rapidjson::kArrayType);
		for (GameObject& gameObject : currentScene->gameObjects)
		{
			rapidjson::Value go(rapidjson::kObjectType);
			Serializer::Serialize(gameObject, "", go, doc);
			gameObjects.PushBack(go, doc.GetAllocator());
		}
		doc.AddMember("GameObjects", gameObjects, doc.GetAllocator());

		rapidjson::StringBuffer sb;
		Serializer::WriteToFile(sb, doc, ofs);
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

		// Copy game object data
		for (GameObject& gameObj : storageScene->gameObjects)
		{
			MyGOF.Instantiate(gameObj,*currentScene, true);
		}


		//ZACH: COMPONENT LINKING
		//for (size_t goIndex{ 0 }; goIndex < storageScene->get_gameobjcount(); ++goIndex)
		//{
		//	GameObject* currGameObj = currentScene->gameObjects[goIndex];
		//	GameObject* storedGameObj = storageScene->gameObjects[goIndex];
		//	currGameObj->transform.previewLink(&storedGameObj->transform);
		//	//std::cout << "Name comparisons: " << currGameObj->name << '|' << storedGameObj->name << std::endl;
		//	for (size_t compIndex{ 0 }; compIndex < currGameObj->components.size(); ++compIndex)
		//	{
		//		currGameObj->components[compIndex]->previewLink(storedGameObj->components[compIndex]);
		//	}
		//}

		// MATT: Sorting Layer Replacement HERE

	}

	template <typename T>
	void SceneManager::CallbackComponentAdd(ComponentAddEvent<T>* pEvent)
	{
		if constexpr (std::is_same<T,Script>())
		{
			T& component = currentScene->componentArrays.GetArray<T>().emplace_back(pEvent->gameObject,pEvent->uuid,pEvent->scriptName);
			pEvent->gameObject.AddComponent(&component);
			pEvent->componentContainer = &component;
			MyEventSystem->publish(new ReflectComponentEvent(component));
		}
		else
		{
			T& component = currentScene->componentArrays.GetArray<T>().emplace_back(pEvent->gameObject, pEvent->uuid);
			pEvent->gameObject.AddComponent(&component);
			pEvent->componentContainer = &component;
			MyEventSystem->publish(new ReflectComponentEvent(component));
		}
	}

	template <typename T>
	void SceneManager::CallbackComponentDelete(ComponentDeleteEvent<T>* pEvent)
	{
		currentScene->componentsForDeletion.GetArray<T>().push_back(&pEvent->component);
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
			sceneFilePath.clear();
		}

		// Create default scene
		currentScene = new NormalScene();
		if (!currentScene)
			return false;

		return true;
	}
	Scene* SceneManager::get_storage_scene() { return storageScene; }

	void SceneManager::CallbackQuitEngine(QuitEngineEvent* pEvent)
	{
		quit_engine();
	}




	void SceneManager::CallbackChildInstantiate(ChildInstantiateEvent* pEvent)
	{
		GameObject& child = MyGOF.InstantiateChild(*pEvent->parent, *currentScene);
		pEvent->instanceContainer = &child;
	}

	void SceneManager::CallbackGameObjectInstantiate(GameObjectInstantiateEvent* pEvent)
	{

		if (!pEvent->pOriginal)
		{
			GameObject& go = MyGOF.Instantiate(*currentScene);
			pEvent->instanceContainer = &go;
		}
		else
		{
			GameObject& go = MyGOF.Instantiate(*pEvent->pOriginal, *currentScene);
			pEvent->instanceContainer = &go;
		}
	}
	void SceneManager::CallbackGameObjectDelete(GameObjectDestroyEvent* pEvent)
	{
		currentScene->gameObjectsForDeletion.push_back(&pEvent->gameObject);
	}

}

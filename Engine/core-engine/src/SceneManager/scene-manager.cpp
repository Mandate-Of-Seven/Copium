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
		SubscribeComponentsFunctions(ComponentTypes());
		MyEventSystem->subscribe(this, &SceneManager::CallbackQuitEngine);
		MyEventSystem->subscribe(this, &SceneManager::CallbackChildInstantiate);
		MyEventSystem->subscribe(this, &SceneManager::CallbackGameObjectInstantiate);
		MyEventSystem->subscribe(this, &SceneManager::CallbackGameObjectDestroy);
	}

	void SceneManager::DeserializeLink()
	{
		for (GameObject& go : currentScene->gameObjects)
		{
			// Target Graphic
			if (go.HasComponent<Button>())
			{
				Button* button = go.GetComponent<Button>();
				UUID uuid{ (uint64_t)button->targetGraphic };
				button->targetGraphic = reinterpret_cast<IUIComponent*>(FindComponentByID(uuid));
			}

			for (Script* pScript : go.GetComponents<Script>())
			{
				for (auto& pair : pScript->fieldDataReferences)
				{
					const std::string& fieldName{ pair.first };
					Field& field{ pair.second };
					if (field.fType == FieldType::GameObject)
					{
						UUID uuid{ (uint64_t)pScript->fieldGameObjReferences[fieldName] };
						if (uuid == 0)
							continue;
						GameObject* gameObject{ FindGameObjectByID(uuid) };
						pScript->fieldGameObjReferences[fieldName] = gameObject;
						MyEventSystem->publish(new ScriptSetFieldReferenceEvent(*pScript, fieldName.c_str(), gameObject));
					}
					else if (field.fType >= FieldType::Component)
					{
						UUID uuid{ (uint64_t)pScript->fieldComponentReferences[fieldName] };
						if (uuid == 0)
							continue;
						Component* component = FindComponentByID(uuid);
						pScript->fieldComponentReferences[fieldName] = component;
						switch ((ComponentType)field.fType)
						{
						case(ComponentType::Animator):
						{
							MyEventSystem->publish(new ScriptSetFieldReferenceEvent(*pScript, fieldName.c_str(), (Animator*)component));
							break;
						}
						case(ComponentType::AudioSource):
						{
							MyEventSystem->publish(new ScriptSetFieldReferenceEvent(*pScript, fieldName.c_str(), (AudioSource*)component));
							break;
						}
						case(ComponentType::BoxCollider2D):
						{
							MyEventSystem->publish(new ScriptSetFieldReferenceEvent(*pScript, fieldName.c_str(), (BoxCollider2D*)component));
							break;
						}
						case(ComponentType::Button):
						{
							MyEventSystem->publish(new ScriptSetFieldReferenceEvent(*pScript, fieldName.c_str(), (Button*)component));
							break;
						}
						case(ComponentType::Camera):
						{
							MyEventSystem->publish(new ScriptSetFieldReferenceEvent(*pScript, fieldName.c_str(), (Camera*)component));
							break;
						}
						case(ComponentType::Image):
						{
							MyEventSystem->publish(new ScriptSetFieldReferenceEvent(*pScript, fieldName.c_str(), (Image*)component));
							break;
						}
						case(ComponentType::Rigidbody2D):
						{
							MyEventSystem->publish(new ScriptSetFieldReferenceEvent(*pScript, fieldName.c_str(), (Rigidbody2D*)component));
							break;
						}
						case(ComponentType::SpriteRenderer):
						{
							MyEventSystem->publish(new ScriptSetFieldReferenceEvent(*pScript, fieldName.c_str(), (SpriteRenderer*)component));
							break;
						}
						case(ComponentType::Script):
						{
							//Different scripts
							if (component && ((Script*)component)->Name() != field.typeName)
								continue;
							MyEventSystem->publish(new ScriptSetFieldReferenceEvent(*pScript, fieldName.c_str(), (Script*)component));
							break;
						}
						case(ComponentType::Text):
						{
							MyEventSystem->publish(new ScriptSetFieldReferenceEvent(*pScript, fieldName.c_str(), (Text*)component));
							break;
						}
						case(ComponentType::SortingGroup):
						{
							MyEventSystem->publish(new ScriptSetFieldReferenceEvent(*pScript, fieldName.c_str(), (SortingGroup*)component));
							break;
						}
						}
					}
				}
			}

		}
	}

	void SceneManager::PreviewLink()
	{
		for (GameObject& go : currentScene->gameObjects)
		{
			// Target Graphic
			for (Button* pButton : go.GetComponents<Button>())
			{
				if (pButton->targetGraphic)
					pButton->targetGraphic = reinterpret_cast<IUIComponent*>(FindComponentByID(pButton->targetGraphic->uuid));
			}

			for (Script* pScript : go.GetComponents<Script>())
			{
				for (auto& pair : pScript->fieldGameObjReferences)
				{
					if (pair.second)
						pair.second = FindGameObjectByID(pair.second->uuid);
				}

				for (auto& pair : pScript->fieldComponentReferences)
				{
					if (!pair.second)
						continue;
					pair.second = FindComponentByID(pair.second->uuid);
					std::string fieldName{ pair.first };
					Field& field = pScript->fieldDataReferences[pair.first];
					Component* component = pair.second;
					switch ((ComponentType)field.fType)
					{
					case(ComponentType::Animator):
					{
						MyEventSystem->publish(new ScriptSetFieldReferenceEvent(*pScript, fieldName.c_str(), (Animator*)component));
						break;
					}
					case(ComponentType::AudioSource):
					{
						MyEventSystem->publish(new ScriptSetFieldReferenceEvent(*pScript, fieldName.c_str(), (AudioSource*)component));
						break;
					}
					case(ComponentType::BoxCollider2D):
					{
						MyEventSystem->publish(new ScriptSetFieldReferenceEvent(*pScript, fieldName.c_str(), (BoxCollider2D*)component));
						break;
					}
					case(ComponentType::Button):
					{
						MyEventSystem->publish(new ScriptSetFieldReferenceEvent(*pScript, fieldName.c_str(), (Button*)component));
						break;
					}
					case(ComponentType::Camera):
					{
						MyEventSystem->publish(new ScriptSetFieldReferenceEvent(*pScript, fieldName.c_str(), (Camera*)component));
						break;
					}
					case(ComponentType::Image):
					{
						MyEventSystem->publish(new ScriptSetFieldReferenceEvent(*pScript, fieldName.c_str(), (Image*)component));
						break;
					}
					case(ComponentType::Rigidbody2D):
					{
						MyEventSystem->publish(new ScriptSetFieldReferenceEvent(*pScript, fieldName.c_str(), (Rigidbody2D*)component));
						break;
					}
					case(ComponentType::SpriteRenderer):
					{
						MyEventSystem->publish(new ScriptSetFieldReferenceEvent(*pScript, fieldName.c_str(), (SpriteRenderer*)component));
						break;
					}
					case(ComponentType::Script):
					{
						//Different scripts
						if (((Script*)component)->Name() != field.typeName)
							continue;
						MyEventSystem->publish(new ScriptSetFieldReferenceEvent(*pScript, fieldName.c_str(), (Script*)component));
						break;
					}
					case(ComponentType::Text):
					{
						MyEventSystem->publish(new ScriptSetFieldReferenceEvent(*pScript, fieldName.c_str(), (Text*)component));
						break;
					}
					case(ComponentType::SortingGroup):
					{
						MyEventSystem->publish(new ScriptSetFieldReferenceEvent(*pScript, fieldName.c_str(), (SortingGroup*)component));
						break;
					}
					}
					pScript->fieldComponentReferences[fieldName] = component;
				}
			}
		}
	}

	template <typename T>
	bool SceneManager::DelinkComponent(T*& container)
	{
		for (T* pComponent : currentScene->componentsForDeletion.GetArray<T>())
		{
			//If there is no reference
			if (container == pComponent)
			{
				container = nullptr;
				return true;
			}
		}
		return false;
	}

	void SceneManager::DelinkDeleted()
	{
		// Target Graphic
		for (Button& button : currentScene->componentArrays.GetArray<Button>())
		{
			//If button has no reference
			if (button.targetGraphic == nullptr)
			{
				continue;
			}
			//If target was a image and was delinked
			if (DelinkComponent((Image*&)button.targetGraphic))
				continue;
			DelinkComponent((Text*&)button.targetGraphic);
		}

		for (Script& script : currentScene->componentArrays.GetArray<Script>())
		{
			for (auto& pair : script.fieldGameObjReferences)
			{
				//If there is no reference
				if (pair.second == nullptr)
					continue;
				for (GameObject* pGameObject : currentScene->gameObjectsForDeletion)
				{
					//If there is no reference
					if (pair.second == pGameObject)
					{
						pair.second = nullptr;
						MyEventSystem->publish(new ScriptSetFieldReferenceEvent(script, pair.first.c_str(), pair.second));
						break;
					}
				}
			}

			for (auto& pair : script.fieldComponentReferences)
			{
				//If there is no reference
				if (!pair.second)
					continue;
				const std::string fieldName{ pair.first };
				Component*& component{ pair.second };
				Field& field{ script.fieldDataReferences[fieldName] };
				switch ((ComponentType)field.fType)
				{
				case(ComponentType::Animator):
				{
					if (DelinkComponent((Animator*&)component))
						MyEventSystem->publish(new ScriptSetFieldReferenceEvent(script, fieldName.c_str(), (Animator*)component));
					break;
				}
				case(ComponentType::AudioSource):
				{
					if (DelinkComponent((AudioSource*&)component))
						MyEventSystem->publish(new ScriptSetFieldReferenceEvent(script, fieldName.c_str(), (AudioSource*)component));
					break;
				}
				case(ComponentType::BoxCollider2D):
				{
					if (DelinkComponent((BoxCollider2D*&)component))
						MyEventSystem->publish(new ScriptSetFieldReferenceEvent(script, fieldName.c_str(), (BoxCollider2D*)component));
					break;
				}
				case(ComponentType::Button):
				{
					if (DelinkComponent((Button*&)component))
						MyEventSystem->publish(new ScriptSetFieldReferenceEvent(script, fieldName.c_str(), (Button*)component));
					break;
				}
				case(ComponentType::Camera):
				{
					if (DelinkComponent((Camera*&)component))
						MyEventSystem->publish(new ScriptSetFieldReferenceEvent(script, fieldName.c_str(), (Camera*)component));
					break;
				}
				case(ComponentType::Image):
				{
					if (DelinkComponent((Image*&)component))
						MyEventSystem->publish(new ScriptSetFieldReferenceEvent(script, fieldName.c_str(), (Image*)component));
					break;
				}
				case(ComponentType::Rigidbody2D):
				{
					if (DelinkComponent((Rigidbody2D*&)component))
						MyEventSystem->publish(new ScriptSetFieldReferenceEvent(script, fieldName.c_str(), (Rigidbody2D*)component));
					break;
				}
				case(ComponentType::SpriteRenderer):
				{
					if (DelinkComponent((SpriteRenderer*&)component))
						MyEventSystem->publish(new ScriptSetFieldReferenceEvent(script, fieldName.c_str(), (SpriteRenderer*)component));
					break;
				}
				case(ComponentType::Script):
				{
					//Different scripts
					if (((Script*)component)->Name() != field.typeName)
						continue;
					if (DelinkComponent((Script*&)component))
						MyEventSystem->publish(new ScriptSetFieldReferenceEvent(script, fieldName.c_str(), (Script*)component));
					break;
				}
				case(ComponentType::Text):
				{
					if (DelinkComponent((Text*&)component))
						MyEventSystem->publish(new ScriptSetFieldReferenceEvent(script, fieldName.c_str(), (Text*)component));
					break;
				}
				case(ComponentType::SortingGroup):
				{
					if (DelinkComponent((SortingGroup*&)component))
						MyEventSystem->publish(new ScriptSetFieldReferenceEvent(script, fieldName.c_str(), (SortingGroup*)component));
					break;
				}
				}
			}
		}
	}

	template <typename T, typename... Ts>
	struct CleanerStruct
	{
		Scene& scene;
		CleanerStruct(TemplatePack<T, Ts...> pack){}
		CleanerStruct(Scene& _scene) : scene{_scene}
		{
			CleanComponents<T,Ts...>();
			CleanGameObjects();
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
		void CleanGameObjects()
		{
			//iterate through an array for deletion of game object
			//if found, call remove

			for (GameObject* gameobj : scene.gameObjectsForDeletion)
			{
				PRINT("DESTROYING " << gameobj->name);
				MyGOF.Destroy(*gameobj, scene.gameObjects);

			}
			scene.gameObjectsForDeletion.clear();
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
		{
			DelinkDeleted();
			CleanUpScene(*currentScene);
		}
		//if theres a scene to load
		if (!sceneFile.empty())
		{
			//load_scene(sceneFile);
			//sceneFile.clear();
		}
	}
	void SceneManager::exit()
	{
		if (currSceneState == Scene::SceneState::play)
		{
			endPreview();
		}

		for (GameObject& go : currentScene->gameObjects)
		{
			PRINT(std::hex << &go);
		}

		if (currentScene)
		{
			PRINT("Scene Address:" << currentScene);
			delete currentScene;
			currentScene = nullptr;
		}
		if (storageScene)
		{
			delete storageScene;
			storageScene = nullptr;
		}

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

		MyEventSystem->publish(new SceneChangingEvent(*currentScene));

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
			DeserializeLink();
			for (GameObject& go : currentScene->gameObjects)
			{
				// Transform and Parent
				if (go.transform.parent)
				{
					UUID uuid{ (uint64_t)go.transform.parent };
					go.transform.parent = nullptr;
					GameObject* p = MySceneManager.FindGameObjectByID(uuid);
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



				// Image
				if (go.HasComponent<Image>())
				{
					Image* image = go.GetComponent<Image>();
					if (image->sprite.spriteID != 0)
					{
						const std::vector<Texture>& textures = MyAssetSystem.GetTextures();
						bool reference = false;
						for (int i = 0; i < textures.size(); i++)
						{
							uint64_t pathID = std::hash<std::string>{}(textures[i].get_file_path());
							MetaID metaID = MyAssetSystem.GetMetaID(pathID);
							// Check if the uuid of the sprite is the same as the meta file
							if (metaID.uuid == image->sprite.spriteID)
							{
								// If so set the reference texture to that file
								reference = true;
								image->sprite.refTexture = MyAssetSystem.GetTexture(i);
								break;
							}
						}
						// If there is no references, set the spriteID to 0
						if (!reference)
							image->sprite.spriteID = 0;
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

		backUpCurrScene();

		for (GameObject& gameObj : currentScene->gameObjects)
		{
			mainCamera = gameObj.GetComponent<Camera>();
			if (mainCamera)
				break;
		}

		PreviewLink();
		for (GameObject& go : currentScene->gameObjects)
		{
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
				UUID uuid{ (uint64_t) button->targetGraphic };
				button->targetGraphic = reinterpret_cast<IUIComponent*>(FindComponentByID(uuid));
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

		MyEventSystem->publish(new StopPreviewEvent());

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

	bool SceneManager::save_scene(const std::string& _filepath, const std::string& _filename, bool _modifyname)
	{
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

		if (_modifyname)
		{
			currentScene->set_name(_filename);
		}

		Copium::SerializeBasic(_filename, doc, doc, "Name");

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
			if (!gameObj.transform.HasParent())
				MyGOF.Instantiate(gameObj,*currentScene, true);
		}

		// MATT: Sorting Layer Replacement HERE

	}

	template <typename T>
	void SceneManager::CallbackComponentAdd(ComponentAddEvent<T>* pEvent)
	{
		if constexpr (std::is_same<T,Script>())
		{
			
			T& component = MyGOF.AddComponent(pEvent->gameObject, *currentScene, pEvent->scriptName, pEvent->uuid );
			pEvent->componentContainer = &component;
		}
		else
		{
			T& component = MyGOF.AddComponent<T>(pEvent->gameObject, *currentScene, pEvent->uuid,nullptr);
			pEvent->componentContainer = &component;
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

	template <typename T, typename... Ts>
	struct CleanGameObjectStruct
	{
		GameObject& gameObject;
		Scene& scene;
		CleanGameObjectStruct(TemplatePack<T, Ts...> pack) {}
		CleanGameObjectStruct(GameObject& _gameObject, Scene& _scene) : gameObject{ _gameObject }, scene{_scene}
		{
			Clean<T, Ts...>();
		}
		template <typename T1, typename... T1s>
		void Clean()
		{
			for (T1* pComponent : gameObject.componentPtrArrays.GetArray<T1>())
			{
				scene.componentsForDeletion.GetArray<T1>().push_back(pComponent);
			}
			if constexpr (sizeof...(T1s) != 0)
			{
				Clean<T1s...>();
			}
		}
	};

	using CleanGameObject = decltype(CleanGameObjectStruct(ComponentTypes()));

	void SceneManager::CallbackGameObjectDestroy(GameObjectDestroyEvent* pEvent)
	{
		COPIUM_ASSERT(currentScene == nullptr, "No scene is loaded, where did you get this gameObject from?");
		CleanGameObject(pEvent->gameObject, *currentScene);
		currentScene->gameObjectsForDeletion.push_back(&pEvent->gameObject);
	}

}

/*!***************************************************************************************
\file			sorting-layers.cpp
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			12/01/2023

\brief
	This file holds the definition of the sorting layers for gameobjects.

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"

#include "Utilities/sorting-layers.h"
#include "GameObject/game-object.h"

namespace Copium
{
	int SortingLayers::layerCount;

	SortingLayers::SortingLayers()
	{
		layerCount = 1;
		Layer layer{ "Default", 0, std::vector<GameObject*>(maxObjects, nullptr)};
		sortingLayers.push_back(layer);
	}

	// Bean: Use a hash map to identify layers instead of the name
	Layer* SortingLayers::CreateNewLayer(const std::string& _name)
	{
		COPIUM_ASSERT(layerCount + 1 > maxLayers, "Maxed layers reached!");
		
		Layer layer{ _name, (unsigned int)layerCount, std::vector<GameObject*>(maxObjects) };
		sortingLayers.push_back(layer);
		layerCount = sortingLayers.size();

		return &sortingLayers.back();
	}

	void SortingLayers::RemoveLayer(const std::string& _name)
	{
		for (int i = 0; i < sortingLayers.size(); i++)
		{
			if (!sortingLayers[i].name.compare(_name))
			{
				// Empty out the array of gameobjects
				sortingLayers[i].gameObjects.clear();

				// Remove the layer
				std::swap(sortingLayers[i], sortingLayers.back());
				sortingLayers.pop_back();
				layerCount--;
				break;
			}
		}
	}

	void SortingLayers::RemoveLayer(const unsigned int& _layerID)
	{
		for (int i = 0; i < sortingLayers.size(); i++)
		{
			if (sortingLayers[i].layerID == _layerID)
			{
				// Empty out the array of gameobjects
				sortingLayers[i].gameObjects.clear();

				// Remove the layer
				std::swap(sortingLayers[i], sortingLayers.back());
				sortingLayers.pop_back();
				layerCount--;
				break;
			}
		}
	}

	void SortingLayers::SortLayers()
	{
		std::sort(sortingLayers.begin(), sortingLayers.end(),
			[](const Layer& x, const Layer& y)
			{
				return x.layerID < y.layerID;
			}
		);
	}

	void SortingLayers::BubbleSortGameObjects()
	{
		for (Layer& layer : sortingLayers)
		{
			bool swapped{ false };
			if (layer.gameObjects.size() <= 1)
				continue;

			for (size_t i{ 0 }; i < layer.gameObjects.size() - 1; ++i)
			{
				for (size_t j{ 0 }; j < layer.gameObjects.size() - 1 - i; ++j)
				{
					SortingGroup* sg1{ nullptr }, * sg2{ nullptr };

					if (!layer.gameObjects[j] && layer.gameObjects[j + 1])
					{
						std::swap(layer.gameObjects[j], layer.gameObjects[j + 1]);
						swapped = true;
						continue;
					}

					if (layer.gameObjects[j] && layer.gameObjects[j + 1])
					{
						Component* co1 = layer.gameObjects[j]->GetComponent<SortingGroup>();
						Component* co2 = layer.gameObjects[j + 1]->GetComponent<SortingGroup>();

						if (co1 && co2)
						{
							sg1 = reinterpret_cast<SortingGroup*>(co1);
							sg2 = reinterpret_cast<SortingGroup*>(co2);

							if (sg1->GetOrderInLayer() > sg2->GetOrderInLayer())
							{
								std::swap(layer.gameObjects[j], layer.gameObjects[j + 1]);
								swapped = true;
							}
						}
					}
				}

				if (!swapped)
					break;
			}
		}
	}

	void SortingLayers::SwapLayers(const std::string& _name01, const std::string& _name02)
	{
		int first = 0, second = 0;
		for (int i = 0; i < sortingLayers.size(); i++)
		{
			if (first > -1 && second > -1)
				break;

			if (!sortingLayers[i].name.compare(_name01))
				first = i;

			if (!sortingLayers[i].name.compare(_name02))
				second = i;
		}

		if (first > -1 && second > -1)
		{
			sortingLayers[first].layerID = first;
			sortingLayers[second].layerID = second;
			std::swap(sortingLayers[first], sortingLayers[second]);
		}
	}

	void SortingLayers::SwapLayers(const unsigned int& _layer01, const unsigned int& _layer02)
	{
		std::swap(sortingLayers[_layer01], sortingLayers[_layer02]);
		return;
		//int first = -1, second = -1;
		//for (int i = 0; i < sortingLayers.size(); i++)
		//{
		//	if (first > -1 && second > -1)
		//		break;

		//	if (sortingLayers[i].layerID == _layer01)
		//		first = i;

		//	if (sortingLayers[i].layerID == _layer02)
		//		second = i;
		//}

		//if (first > -1 && second > -1)
		//{
		//	sortingLayers[first].layerID = first;
		//	sortingLayers[second].layerID = second;
		//	std::swap(sortingLayers[first], sortingLayers[second]);
		//}
	}

	void SortingLayers::AddGameObject(const std::string& _name, GameObject& _gameObject)
	{
		// Check if the gameobject already exist in the layer
		if (DoesGameObjectExist(_gameObject))
			return;

		for (Layer& layer : sortingLayers)
		{
			if (layer.name.compare(_name))
				continue;
				
			for (GameObject*& gameObject : layer.gameObjects)
			{
				if (gameObject)
					continue;

				//PRINT("	Added " << _gameObject.name << " to " << layer.name);
				gameObject = &_gameObject;
				break;
			}
		}
	}

	void SortingLayers::AddGameObject(const unsigned int& _layerID, GameObject& _gameObject)
	{
		// Check if the gameobject already exist in the layer
		if (DoesGameObjectExist(_gameObject))
			return;
		
		for (Layer& layer : sortingLayers)
		{
			if (layer.layerID != _layerID)
				continue;

			for (GameObject*& gameObject : layer.gameObjects)
			{
				if (gameObject)
					continue;

				//PRINT("	Added " << _gameObject.name << " to " << layer.name);
				gameObject = &_gameObject;
				break;
			}
		}
	}

	void SortingLayers::ReplaceGameObject(const unsigned int& _layerID, GameObject& _gameObject)
	{
		for (Layer& layer : sortingLayers)
		{
			if (layer.layerID != _layerID)
				continue;

			for (GameObject*& gameObject : layer.gameObjects)
			{
				if (!gameObject)
					continue;

				if (gameObject->uuid == _gameObject.uuid)
				{
					//PRINT("Replaced " << gameObject->name);
					gameObject = &_gameObject;
					break;
				}
			}
		}
	}

	void SortingLayers::RemoveGameObject(const std::string& _name, GameObject& _gameObject)
	{
		for (Layer& layer : sortingLayers)
		{
			if (layer.name.compare(_name))
				continue;

			for (GameObject*& gameObject : layer.gameObjects)
			{
				if (!gameObject)
					continue;

				if (gameObject->uuid == _gameObject.uuid)
				{
					//PRINT("Replaced " << gameObject->name);
					gameObject = nullptr;
					break;
				}
			}
		}
	}

	void SortingLayers::RemoveGameObject(const unsigned int& _layerID, GameObject& _gameObject)
	{
		for (Layer& layer : sortingLayers)
		{
			if (layer.layerID != _layerID)
				continue;
			
			//PRINT("Attempt to remove an object from " << layer.name);

			for (GameObject*& gameObject : layer.gameObjects)
			{
				if (!gameObject)
					continue;

				if (gameObject->uuid == _gameObject.uuid)
				{
					//PRINT("Replaced " << gameObject->name);
					gameObject = nullptr;
					break;
				}
			}
		}
	}

	bool SortingLayers::DoesGameObjectExist(const std::string& _name, const GameObject& _gameObject)
	{
		Layer* layer = GetLayer(_name);

		// Layer does not exist, thus not in layer
		if (!layer)
			return false;

		for (GameObject* gameObject : layer->gameObjects)
		{
			if (!gameObject)
				continue;

			// It exist in this layer
			if (gameObject->uuid == _gameObject.uuid)
				return true;
		}

		return false;
	}

	bool SortingLayers::DoesGameObjectExist(const unsigned int& _layerID, const GameObject& _gameObject)
	{
		Layer* layer = GetLayer(_layerID);

		// Layer does not exist, thus not in layer
		if (!layer)
			return false;

		for (GameObject* gameObject : layer->gameObjects)
		{
			if (!gameObject)
				continue;

			// It exist in this layer
			if (gameObject->uuid == _gameObject.uuid)
				return true;
		}

		return false;
	}

	bool SortingLayers::DoesGameObjectExist(const GameObject& _gameObject)
	{
		for (Layer& layer : sortingLayers)
		{
			for (GameObject* gameObject : layer.gameObjects)
			{
				if (!gameObject)
					continue;

				// It exist in this layer
				if (gameObject->uuid == _gameObject.uuid)
					return true;
			}
		}

		return false;
	}

	void SortingLayers::ClearAllLayer(const bool& _clear)
	{
		if (_clear)
			for (Layer& layer : sortingLayers)
				layer.gameObjects.clear();
		else if (!_clear)
			for (Layer& layer : sortingLayers)
				for (GameObject*& gameObject : layer.gameObjects)
					gameObject = nullptr;
	}

	Layer* SortingLayers::GetLayer(const std::string& _name)
	{
		for (Layer& layer : sortingLayers)
		{
			if (layer.name.compare(_name))
				continue;

			return &layer;
		}

		return nullptr;
	}

	Layer* SortingLayers::GetLayer(const int& _layerID)
	{
		for (Layer& layer : sortingLayers)
		{
			if (layer.layerID != _layerID)
				continue;

			return &layer;
		}
		
		return nullptr;
	}
}
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
		Layer layer{ "Default", 0, std::vector<GameObject*>(maxObjects) };
		sortingLayers.push_back(layer);
	}

	void SortingLayers::CreateNewLayer(const std::string& _name)
	{
		COPIUM_ASSERT(layerCount + 1 >= maxLayers, "Maxed layers reached!");
		
		Layer layer{ _name, layerCount, std::vector<GameObject*>(maxObjects) };
		sortingLayers.push_back(layer);
		layerCount++;
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

	void SortingLayers::RemoveLayer(const int& _layerID)
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

	void SortingLayers::SwapLayers(const std::string& _name01, const std::string& _name02)
	{
		int first = 0, second = 0;
		for (int i = 0; i < sortingLayers.size(); i++)
		{
			if (first && second)
				break;

			if (!sortingLayers[i].name.compare(_name01))
				first = i;

			if (!sortingLayers[i].name.compare(_name02))
				second = i;
		}

		std::swap(sortingLayers[first], sortingLayers[second]);
	}

	void SortingLayers::SwapLayers(const int& _layer01, const int& _layer02)
	{
		int first = 0, second = 0;
		for (int i = 0; i < sortingLayers.size(); i++)
		{
			if (first && second)
				break;

			if (sortingLayers[i].layerID == _layer01)
				first = i;

			if (sortingLayers[i].layerID == _layer02)
				second = i;
		}

		std::swap(sortingLayers[first], sortingLayers[second]);
	}

	void SortingLayers::AddGameObject(const std::string& _name, GameObject& _gameObject)
	{
		for (int i = 0; i < sortingLayers.size(); i++)
		{
			if (!sortingLayers[i].name.compare(_name))
			{
				for (int j = 0; j < sortingLayers[i].gameObjects.size(); j++)
				{
					if (sortingLayers[i].gameObjects[j] == nullptr)
					{
						sortingLayers[i].gameObjects[j] = &_gameObject;
					}
				}
			}
		}
	}

	void SortingLayers::AddGameObject(const int& _layerID, GameObject& _gameObject)
	{
		for (int i = 0; i < sortingLayers.size(); i++)
		{
			if (sortingLayers[i].layerID == _layerID)
			{
				for (int j = 0; j < sortingLayers[i].gameObjects.size(); j++)
				{
					if (sortingLayers[i].gameObjects[j] == nullptr)
					{
						sortingLayers[i].gameObjects[j] = &_gameObject;
					}
				}
			}
		}
	}

	void SortingLayers::RemoveGameObject(const std::string& _name, GameObject& _gameObject)
	{
		for (int i = 0; i < sortingLayers.size(); i++)
		{
			if (!sortingLayers[i].name.compare(_name))
			{
				for (int j = 0; j < sortingLayers[i].gameObjects.size(); j++)
				{
					if (sortingLayers[i].gameObjects[j]->id == _gameObject.id)
					{
						sortingLayers[i].gameObjects[j] = nullptr;
					}
				}
			}
		}
	}

	void SortingLayers::RemoveGameObject(const int& _layerID, GameObject& _gameObject)
	{
		for (int i = 0; i < sortingLayers.size(); i++)
		{
			if (sortingLayers[i].layerID == _layerID)
			{
				for (int j = 0; j < sortingLayers[i].gameObjects.size(); j++)
				{
					if (sortingLayers[i].gameObjects[j]->id == _gameObject.id)
					{
						sortingLayers[i].gameObjects[j] = nullptr;
					}
				}
			}
		}
	}
}
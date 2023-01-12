/*!***************************************************************************************
\file			sorting-layer.cpp
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			10/01/2023

\brief
	This file holds the definition of the layering system for gameobjects and render pass.

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"

#include "Utilities/sorting-layer.h"

namespace Copium
{
	Layering::Layering()
	{
		for (int i = 0; i < maxLayers; i++)
		{
			sortingLayer.emplace(
				std::make_pair(i, std::vector<GameObjectID>(maxObjects))
			);

			layerStatus[i] = true;
		}
	}

	void Layering::AddGameObject(const GameObjectID& _id)
	{
		sortingLayer[defaultLayer].push_back(_id);
	}

	void Layering::AddGameObject(const int& _layer, const GameObjectID& _id)
	{
		if (_layer < maxLayers)
		{
			int tempLayer = 0;
			if (CheckIfExist(tempLayer, _id))
			{
				RemoveGameObject(tempLayer, _id);

				int slotToUse = GetEmptySlot(_layer);
				if (slotToUse != -1)
					sortingLayer[_layer][slotToUse] = _id;
				else
					sortingLayer[_layer].push_back(_id);
			}
			else
			{
				int slotToUse = GetEmptySlot(_layer);
				if (slotToUse != -1)
					sortingLayer[_layer][slotToUse] = _id;
				else
					sortingLayer[_layer].push_back(_id);
			}
		}
	}

	void Layering::RemoveGameObject(const int& _layer, const GameObjectID& _id)
	{
		if (_layer < maxLayers)
		{
			for (int i = 0; i < sortingLayer[_layer].size(); i++)
			{
				// Reset the container at this index to be -1
				if (sortingLayer[_layer][i] == _id)
					sortingLayer[_layer][i] = -1;
			}
		}
	}

	bool Layering::CheckIfExist(int& _layer, const GameObjectID& _id)
	{
		// Check if object already exist in another layer
		for (int i = 0; i < maxLayers; i++)
		{
			auto result = std::find(sortingLayer[i].begin(), sortingLayer[i].end(), _id);
			if (result != sortingLayer[i].end())
			{
				_layer = i;
				return true;
			}
		}

		return false;
	}

	int Layering::GetEmptySlot(const int& _layer)
	{
		for (int i = 0; i < sortingLayer[_layer].size(); i++)
		{
			if (sortingLayer[_layer][i] == -1)
				return i;
		}

		return -1;
	}
}
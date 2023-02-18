/*!***************************************************************************************
\file			layers.cpp
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

#include "Utilities/layers.h"
#include <CopiumCore/uuid.h>

namespace Copium
{
	Layering::Layering()
	{
		for (int i = 0; i < maxLayers; i++)
		{
			layers.emplace(
				std::make_pair(i, std::vector<UUID>(maxObjects))
			);

			layerStatus[i] = true;
		}
	}

	void Layering::AddGameObject(const UUID& _id)
	{
		layers[defaultLayer].push_back(_id);
	}

	void Layering::AddGameObject(const int& _layer, const UUID& _id)
	{
		//if (_layer < maxLayers)
		//{
		//	int tempLayer = 0;
		//	if (CheckIfExist(tempLayer, _id))
		//	{
		//		RemoveGameObject(tempLayer, _id);

		//		int slotToUse = GetEmptySlot(_layer);
		//		if (slotToUse != -1)
		//			layers[_layer][slotToUse] = _id;
		//		else
		//			layers[_layer].push_back(_id);
		//	}
		//	else
		//	{
		//		int slotToUse = GetEmptySlot(_layer);
		//		if (slotToUse != -1)
		//			layers[_layer][slotToUse] = _id;
		//		else
		//			layers[_layer].push_back(_id);
		//	}
		//}
	}

	void Layering::RemoveGameObject(const int& _layer, const UUID& _id)
	{
		//if (_layer < maxLayers)
		//{
		//	for (int i = 0; i < layers[_layer].size(); i++)
		//	{
		//		// Reset the container at this index to be -1
		//		if (layers[_layer][i] == _id)
		//			layers[_layer][i] = UUID(-1);
		//	}
		//}
	}

	bool Layering::CheckIfExist(int& _layer, const UUID& _id)
	{
		// Check if object already exist in another layer
		for (int i = 0; i < maxLayers; i++)
		{
			auto result = std::find(layers[i].begin(), layers[i].end(), _id);
			if (result != layers[i].end())
			{
				_layer = i;
				return true;
			}
		}

		return false;
	}

	int Layering::GetEmptySlot(const int& _layer)
	{
		//for (int i = 0; i < layers[_layer].size(); i++)
		//{
		//	if (layers[_layer][i] == -1)
		//		return i;
		//}

		return -1;
	}
}
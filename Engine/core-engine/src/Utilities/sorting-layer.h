/*!***************************************************************************************
\file			sorting-layer.h
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			10/01/2023

\brief
	This file holds the declaration of the layering system for gameobjects and render pass.

All content � 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef SORTING_LAYER_H
#define SORTING_LAYER_H

#include <map>
#include <vector>


namespace Copium
{
	// From game-object.h
	using GameObjectID = int64_t;
	
	// Each object will have a reference to the layer that it is on in the editor

	// Store layers in a orderer map vector containing pointers to 
	class Layering
	{
	public:
		// Initialize 32 empty layers with 100 objects each
		Layering();

		// Adding gameobject into the layer in the map
		void AddGameObject(const GameObjectID& _id);

		// Adding gameobject into the layer in the map
		void AddGameObject(const int& _layer, const GameObjectID& _id);

		// Remove gameobject from the layer in the map
		void RemoveGameObject(const int& _layer, const GameObjectID& _id);

		bool CheckIfExist(int& _layer, const GameObjectID& _id);

		int GetEmptySlot(const int& _layer);
	private:
		const int defaultLayer = 0;								// The default layer in which objects when created will be added to
		const int maxLayers = 32;								// Number of layers in the sorting layer system
		const int maxObjects = 100;								// Number of objects in a layer
		std::map<int, std::vector<GameObjectID>> sortingLayer;	// The container which holds the layers which contains the gameobject's ID
	};
}

#endif // !SORTING_LAYER_H

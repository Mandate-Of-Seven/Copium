/*!***************************************************************************************
\file			layers.h
\project
\author			Sean Ngo

\par			Course: GAM250
\par			Section:
\date			10/01/2023

\brief
	This file holds the declaration of the layering system for gameobjects and render pass.

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef LAYERS_H
#define LAYERS_H

#include <map>
#include <vector>

namespace Copium
{	
	class UUID;
	// Each object will have a reference to the layer that it is on in the editor

	// Store layers in a orderer map vector containing pointers to 
	class Layering
	{
	public:

		/***************************************************************************/
		/*!
		\brief
			Constructs the class with 32 empty layers with 100 objects each
		*/
		/***************************************************************************/
		Layering();

		/***************************************************************************/
		/*!
		\brief
			Adds a gameobject into the default layer in the map
		\param _id
			The id of the gameobject to add
		*/
		/***************************************************************************/
		void AddGameObject(const UUID& _id);

		/***************************************************************************/
		/*!
		\brief
			Adds a gameobject into a specific layer in the map
		\param _layer
			The layer to add the gameobject to
		\param _id
			The id of the gameobject to add
		*/
		/***************************************************************************/
		void AddGameObject(const int& _layer, const UUID& _id);

		// Remove gameobject from the layer in the map
		/***************************************************************************/
		/*!
		\brief
			Removes a gameobject from a specific layer in the map
		\param _layer
			The layer to remove from
		\param _id
			The id of the gameobject to remove
		*/
		/***************************************************************************/
		void RemoveGameObject(const int& _layer, const UUID& _id);

		/***************************************************************************/
		/*!
		\brief
			Checks if the gameobject in the layer exists
		\param _layer
			The layer to check
		\param _id
			The id of the gameobject to check
		\return
			True if it exists in the layer, else false
		*/
		/***************************************************************************/
		bool CheckIfExist(int& _layer, const UUID& _id);

		/***************************************************************************/
		/*!
		\brief
			Gets the index to the empty slot in the layer
		\param _layer
			The layer to get an empty slot from
		\return
			The index of the layer
		*/
		/***************************************************************************/
		int GetEmptySlot(const int& _layer);

		void EnableLayer(const int& _layer) { layerStatus[_layer] = true; }
		void DisableLayer(const int& _layer) { layerStatus[_layer] = false; }
		bool GetLayerStatus(const int& _layer) const { return layerStatus[_layer]; }

	private:
		static const int defaultLayer = 0;						// The default layer in which objects when created will be added to
		static const int maxLayers = 32;						// Number of layers in the sorting layer system
		static const int maxObjects = 100;						// Number of objects in a layer
		bool layerStatus[maxLayers];							// The current status of the layers in the engine, either enabled or disabled
		std::map<int, std::vector<UUID>> layers;		// The container which holds the layers which contains the gameobject's ID
	};
}

#endif // !LAYERS_H

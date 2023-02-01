/*!***************************************************************************************
\file			sorting-layers.h
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			12/01/2023

\brief
	This file holds the declaration of the sorting layers for gameobjects.

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef SORTING_LAYERS_H
#define SORTING_LAYERS_H

#include <vector>

namespace Copium
{
	class GameObject;

	// A layer in which the user sorts the objects by
	struct Layer
	{
		std::string name;						// Name of layer
		unsigned int layerID;					// The index of the layer to sort the layers
		std::vector<GameObject*> gameObjects;	// Reference to the gameobjects in this layer
	};

	// Bean: Move to ECS once it is done
	// A component for gameobjects
	/*struct SortingGroup
	{
		int sortingLayer;
		int orderInLayer;
	};*/

	// Sorts objects based on the layer the object is in
	class SortingLayers
	{
	public:
		// Initialize a default sort layer with 100 objects each
		SortingLayers();

		// Reorder the layers based on the layerID
		void SortLayers();

		// Swap the layers
		void SwapLayers(const std::string& _name01, const std::string& _name02);
		void SwapLayers(const unsigned int& _layer01, const unsigned int& _layer02);

		// Create new sorting layer
		Layer* CreateNewLayer(const std::string& _name);

		// Remove sorting layer
		void RemoveLayer(const std::string& _name);
		void RemoveLayer(const unsigned int& _layerID);

		// Add gameobject into the sorting layer
		void AddGameObject(const std::string& _name, GameObject& _gameObject);
		void AddGameObject(const unsigned int& _layerID, GameObject& _gameObject);

		// Remove gameobject in the sorting layer
		void RemoveGameObject(const std::string& _name, GameObject& _gameObject);
		void RemoveGameObject(const unsigned int& _layerID, GameObject& _gameObject);

		const int& GetLayerCount() const { return layerCount; }
		const int& GetCharLength() const { return maxCharLength; }
		std::vector<Layer>& GetSortingLayers() { return sortingLayers; }

	private:
		static int layerCount;				// The number of layers in the engine
		static const int defaultLayer = 0;	// The default layer in which objects when created will be added to
		static const int maxCharLength = 16;// The default character length of the name
		static const int maxLayers = 32;	// Number of layers in the sorting layer system
		static const int maxObjects = 100;	// Number of objects in a layer
		std::vector<Layer> sortingLayers;	// The container which holds the layers which contains the gameobject's ID
	};
}

#endif // !SORTING_LAYERS_H

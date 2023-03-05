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
		
		/***************************************************************************/
		/*!
		\brief
			Initialize a default sort layer with 100 objects each
		*/
		/***************************************************************************/
		SortingLayers();

		/***************************************************************************/
		/*!
		\brief
			Reorder the layers based on the layerID
		*/
		/***************************************************************************/
		void SortLayers();

		/***************************************************************************/
		/*!
		\brief
			Reorder the gameobjects based on the order in layer
		*/
		/***************************************************************************/
		void BubbleSortGameObjects();

		/***************************************************************************/
		/*!
		\brief
			Swap two layers based on their names
		\param _name01
			First layer to swap
		\param _name02
			Second layer to swap
		*/
		/***************************************************************************/
		void SwapLayers(const std::string& _name01, const std::string& _name02);

		/***************************************************************************/
		/*!
		\brief
			Swap two layers based on their layer index
		\param _layer01
			First layer to swap
		\param _layer01
			Second layer to swap
		*/
		/***************************************************************************/
		void SwapLayers(const unsigned int& _layer01, const unsigned int& _layer02);

		// Create new sorting layer
		/***************************************************************************/
		/*!
		\brief
			Creates a new sorting layer
		\param _name
			The name of the layer to create
		\return
			A pointer to the layer that was created
		*/
		/***************************************************************************/
		Layer* CreateNewLayer(const std::string& _name);

		/***************************************************************************/
		/*!
		\brief
			Remove a sorting layer based on its name
		\param _name
			The fisrt layer found with the name to remove
		*/
		/***************************************************************************/
		void RemoveLayer(const std::string& _name);

		/***************************************************************************/
		/*!
		\brief
			Remove a sorting layer based on its layer index
		\param _layerID
			The layer with the index to remove
		*/
		/***************************************************************************/
		void RemoveLayer(const unsigned int& _layerID);

		/***************************************************************************/
		/*!
		\brief
			Add a gameobject into the sorting layer
		\param _name
			The fisrt layer found with the name to add the gameobject to
		\param _gameObject
			The gameobject to add into the layer
		*/
		/***************************************************************************/
		void AddGameObject(const std::string& _name, GameObject& _gameObject);

		/***************************************************************************/
		/*!
		\brief
			Add a gameobject into the sorting layer
		\param _layerID
			The layer with the specific index to add the gameobject to
		\param _gameObject
			The gameobject to add into the layer
		*/
		/***************************************************************************/
		void AddGameObject(const unsigned int& _layerID, GameObject& _gameObject);

		/***************************************************************************/
		/*!
		\brief
			Replace a gameobject in the sorting layer
		\param _layerID
			The layer with the specific index to add the gameobject to
		\param _gameObject
			The gameobject to replace into the layer
		*/
		/***************************************************************************/
		void ReplaceGameObject(const unsigned int& _layerID, GameObject& _gameObject);

		/***************************************************************************/
		/*!
		\brief
			Remove a gameobject from the sorting layer
		\param _name
			The fisrt layer found with the name to remove the gameobject from
		\param _gameObject
			The gameobject to remove from the layer
		*/
		/***************************************************************************/
		void RemoveGameObject(const std::string& _name, GameObject& _gameObject);

		/***************************************************************************/
		/*!
		\brief
			Remove a gameobject from the sorting layer
		\param _layerID
			The layer with the specific index to remove the gameobject from
		\param _gameObject
			The gameobject to remove from the layer
		*/
		/***************************************************************************/
		void RemoveGameObject(const unsigned int& _layerID, GameObject& _gameObject);

		bool DoesGameObjectExist(const std::string& _name, const GameObject& _gameObject);
		bool DoesGameObjectExist(const unsigned int& _layerID, const GameObject& _gameObject);
		bool DoesGameObjectExist(const GameObject& _gameObject);

		/***************************************************************************/
		/*!
		\brief
			Remove all gameobjects in all layers
		\param _clear
			Empty the layers
		*/
		/***************************************************************************/
		void ClearAllLayer(const bool& _clear = false);

		Layer* GetLayer(const int& _layerID);
		Layer* GetLayer(const std::string& _name);

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

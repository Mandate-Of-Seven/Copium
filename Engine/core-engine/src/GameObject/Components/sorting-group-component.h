/*!***************************************************************************************
\file			sorting-group-component.h
\project
\author			Sean Ngo
\co-author		Matthew Lau

\par			Course: GAM200
\par			Section:
\date			13/01/2023

\brief
	This file holds the declaration of the sorting group component.

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef SORTING_GROUP_COMPONENT_H
#define SORTING_GROUP_COMPONENT_H

#include <GameObject/Components/component.h>

namespace Copium
{
	class SortingGroup : public Component
	{
	public:
		/*******************************************************************************
		/*!
		*
		\brief
			Constructs the component of the sorting group for this gameobject
		\param _gameObj
			The gameoject to add into the sorting group
		*/
		/*******************************************************************************/
		SortingGroup(GameObject& _gameObj);

		/*******************************************************************************
		/*!
		*
		\brief
			Displays the inspector view with its fields
		*/
		/*******************************************************************************/
		void inspector_view();

		/*******************************************************************************
		/*!
		\brief
			Deep copies a BoxCollider2DComponent into another
		\param gameobj
			Reference to another BoxCollider2DComponent
		\return
			Reference to this BoxCollider2DComponent
		*/
		/*******************************************************************************/
		Component* clone(GameObject& _gameObj, ComponentID* newID = nullptr) const
		{
			SortingGroup* component = new SortingGroup(_gameObj);
			return component;
		}
		/*******************************************************************************
		/*!
		*
		\brief
			Deserialize this renderer component's data from the specified rapidjson Value
		\param _value
			reference to the rapidjson Value which the renderer component's data
			deserializes its data from
		\return
			void
		*/
		/*******************************************************************************/
		void deserialize(rapidjson::Value& _value)
		{
			if (_value.HasMember("SortingLayer"))
				sortingLayer = _value["SortingLayer"].GetInt();

			if (_value.HasMember("OrderInLayer"))
				orderInLayer = _value["OrderInLayer"].GetInt();
		}
		/*******************************************************************************
		/*!
		*
		\brief
			Serialize this renderer component's data to the specified rapidjson Value
		\param _value
			reference to the rapidjson Value which the renderer component's data is to
			serialize its data to
		\param _doc
			reference to the rapidjson Document which is associated to the save file which
			the data is being saved to
		\return
			void
		*/
		/*******************************************************************************/
		void serialize(rapidjson::Value& _value, rapidjson::Document& _doc)
		{
			rapidjson::Value type;
			std::string tc = MAP_COMPONENT_TYPE_NAME[componentType];
			type.SetString(tc.c_str(), rapidjson::SizeType(tc.length()), _doc.GetAllocator());
			_value.AddMember("Type", type, _doc.GetAllocator());

			_value.AddMember("SortingLayer", sortingLayer, _doc.GetAllocator());
			_value.AddMember("OrderInLayer", orderInLayer, _doc.GetAllocator());
		}

		/*******************************************************************************
		/*!
		*
		\brief
			Get the id of the layer this sorting group is in

		\return
			id of the layer this sorting group is in
		*/
		/*******************************************************************************/
		int GetLayerID() const { return sortingLayer; }
		/*******************************************************************************
		/*!
		*
		\brief
			Get the gameobject's order in layer

		\return
			this gameobject's order in layer
		*/
		/*******************************************************************************/
		int GetOrderInLayer() const { return orderInLayer; }
	private:
		int sortingLayer;
		int orderInLayer;
	};
}

#endif // !SORTING_GROUP_COMPONENT_H
/*!***************************************************************************************
****
\file			collider-components.h
\project		
\author			Abdul Hadi

\par			Course: GAM200
\par			Section: 
\date			28/11/2022

\brief
	This file contains the 2dboxCollider component, for performing collision.

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************
****/


#include <GameObject/Components/component.h>
#include <Physics/collision.h>

#ifndef COLLIDER_COMPONENTS_H 
#define COLLIDER_COMPONENTS_H

namespace Copium
{
	struct AABB;
	

	class BoxCollider2D : public Component
	{
		public:
			/***************************************************************************/
			/*!
			\brief
				Constructor for collider Components
			\param _gameObj
				Owner of this component
			*/
			/**************************************************************************/
			BoxCollider2D(GameObject& _gameObj);

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
				BoxCollider2D* component = new BoxCollider2D(_gameObj);
				component->boundingBox = boundingBox;
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
				if (_value.HasMember("BoundingBox"))
				{
					rapidjson::Value& _v = _value["BoundingBox"].GetObj();
					boundingBox.deserialize(_v);
				}
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

				rapidjson::Value bb(rapidjson::kObjectType);
				boundingBox.serialize(bb, _doc);
				_value.AddMember("BoundingBox", bb, _doc.GetAllocator());
			}
			/*******************************************************************************
			/*!
			*	
			\brief
			Gets the current bounding box of the component
			*/
			/*******************************************************************************/
			AABB getBounds();


			void SetBounds(const AABB& _value) { boundingBox = _value; }

		private:
			AABB boundingBox;			//bounding box of object

	};

}

#endif // !COLLIDER_COMPONENTS_H

/*!***************************************************************************************
****
\file			
\project		
\author			
\co-authors		// DELETE IF NOT APPLICABLE

\par			Course: GAM200
\par			Section: 
\date			//DATE LAST MODIFIED

\brief
	// BRIEF DESCRIPTION OF FILE PURPOSE

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

			/***************************************************************************/
			/*!
			\brief
				Deserialize this component's data from specified rapidjson value
			*/
			/**************************************************************************/
			//void deserialize(rapidjson::Value& _value);

			/*******************************************************************************
			/*!
			*
			\brief
				Deep copies a BoxCollider2DComponent into another
			\param rhs
				Reference to another BoxCollider2DComponent
			\return
				Reference to this BoxCollider2DComponent
			*/
			/*******************************************************************************/

			BoxCollider2D& operator=(const Component& rhs) 
			{ boundingBox = reinterpret_cast<const BoxCollider2D*>(&rhs)->boundingBox; return *this; }

			void deserialize(rapidjson::Value& _value)
			{
				if (_value.HasMember("BoundingBox"))
				{
					rapidjson::Value& _v = _value["BoundingBox"].GetObj();
					boundingBox.deserialize(_v);
				}
			}

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

			AABB getBounds();
		private:
			AABB boundingBox;			//bounding box of object

	};

}

#endif // !COLLIDER_COMPONENTS_H

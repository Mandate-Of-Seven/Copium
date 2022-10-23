/*!***************************************************************************************
\file			renderer-component.h
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			21/09/2022

\brief
	This file holds the declaration of the Renderer component class.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#ifndef RENDERER_COMPONENT_H
#define RENDERER_COMPONENT_H

#include "GameObject/component.h"
#include "Math/transform.h"
#include "Graphics/sprite-renderer.h"

namespace Copium
{
	class RendererComponent : public Component
	{
	public:
		RendererComponent(GameObject& _gameObj);

		void deserialize(rapidjson::Value& _value);

		const Copium::Graphics::SpriteRenderer& get_sprite_renderer() const { return spriteRenderer; }
		void set_sprite_renderer(const Copium::Graphics::SpriteRenderer& _spriteRenderer) { spriteRenderer = _spriteRenderer; }

	protected:
		Copium::Graphics::SpriteRenderer spriteRenderer;
	};

    class RendererCreator : public ComponentCreator
    {
    public:
        /*******************************************************************************
        /*!
        *
        \brief
            Default contructor for Renderer Creator
        */
        /*******************************************************************************/
        RendererCreator()
        {
            std::cout << "Renderer registered\n";
        }
        /***************************************************************************/
        /*!
        \brief
            Creates an Renderer Component

        \return
            if successful in creating an Renderer Component, return ptr to it
            if there were errors in the process, return nullptr
        */
        /**************************************************************************/
        virtual Component* create(GameObject& _gameObj)
        {
            return new RendererComponent(_gameObj);
        }
        /*******************************************************************************
        /*!
        *
        \brief
            Destructor for Renderer Creator

        \return
            void
        */
        /*******************************************************************************/
        ~RendererCreator()
        {
            //std::cout << "Renderer Creator destructed\n";
        }
    };
}

#endif // !RENDERER_COMPONENT_H
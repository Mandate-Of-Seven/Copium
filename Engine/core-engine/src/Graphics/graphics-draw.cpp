/*!***************************************************************************************
\file			graphics-draw.cpp
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			16/11/2022

\brief
	This file holds the definitions of functions which emcompasses the drawing aspect
	of the graphic's system.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
#include "pch.h"

#include "Graphics/graphics-draw.h"

#include "Files/assets-system.h"
#include "Editor/editor-system.h"
#include "Graphics/graphics-system.h"

// Bean: remove this after NewManagerInstance is moved
#include "GameObject/Components/renderer-component.h"
#include "GameObject/Components/ui-components.h"
#include "GameObject/Components/collider-components.h"
#include "SceneManager/sm.h"
#include "Math/math-library.h"

namespace Copium
{
	namespace
	{
		AssetsSystem* assets = AssetsSystem::Instance();
		EditorSystem* editorSys = EditorSystem::Instance();
		GraphicsSystem* graphicsSys = GraphicsSystem::Instance();
		NewSceneManager* sm = NewSceneManager::Instance();
		Renderer* renderer;
	}

	void Draw::init()
	{
		renderer = graphicsSys->get_renderer();

		enable(DRAW::EDITOR);
		enable(DRAW::WORLD);
		enable(DRAW::DEVELOPMENT);
	}

	void Draw::update()
	{
		if(drawMode[DRAW::EDITOR])
			editor();

		if (drawMode[DRAW::WORLD])
			world();

		if (drawMode[DRAW::DEBUG])
			debug();

		if (drawMode[DRAW::DEVELOPMENT])
			development();
	}

	void Draw::exit()
	{

	}

	void Draw::world()
	{
		renderer->begin_batch();

		/*
			Bean Theory:
			For each gameobject, check if it has a renderer component
				If it does:
					Check if it is active
						If not: Continue
					Check if it has a spriteID of not 0 (default)
						If it does:
							Render sprite with color
						Else
							Render default sprite (white texture) with color
				Else
					Continue
		*/

		// Theory WIP
		
		Scene* scene = sm->get_current_scene();
		if (scene != nullptr)
		{
			for (GameObject* gameObject : scene->get_gameobjectvector())
			{
				for (Component* component : gameObject->getComponents<SpriteRenderer>())
				{
					if (!component->Enabled())
						continue;

					Transform& t = gameObject->transform;
					SpriteRenderer* rc = reinterpret_cast<SpriteRenderer*>(component);
					Sprite& sr = rc->get_sprite_renderer();
					glm::vec2 size(t.glmScale().x, t.glmScale().y);
					float rotation = t.glmRotation().z;
					// Bean: It should be set in inspector view of the renderer component instead
					unsigned int id = sr.get_sprite_id() - 1;

					// The index of the texture must be less than the size of textures
					if (id != -1 && id < assets->get_textures().size())
					{
						sr.set_texture(assets->get_texture(id));
					}
					else
					{
						sr.set_sprite_id(0);
						sr.set_texture(nullptr);
					}

					if (gameObject->has_parent())
					{
						Transform& t1 = gameObject->get_parent()->transform;
						renderer->draw_quad(t.glmPosition() + t1.glmPosition(), size, rotation, sr);

					}

					renderer->draw_quad(t.glmPosition(), size, rotation, sr);
				}
				for (Component* component : gameObject->getComponents<ImageComponent>())
				{
					if (!component->Enabled())
						continue;
					Transform& t = gameObject->transform;
					ImageComponent* rc = reinterpret_cast<ImageComponent*>(component);
					Sprite& sr = rc->get_sprite_renderer();
					glm::vec2 size(t.glmScale().x, t.glmScale().y);
					float rotation = t.glmRotation().z;
					// Bean: It should be set in inspector view of the renderer component instead
					unsigned int id = sr.get_sprite_id() - 1;

					// The index of the texture must be less than the size of textures
					if (id != -1 && id < assets->get_textures().size())
					{
						sr.set_texture(assets->get_texture(id));
					}
					else
					{
						sr.set_sprite_id(0);
						sr.set_texture(nullptr);
					}

					renderer->draw_quad({ rc->Offset(),0 }, size, rotation, sr);
				}
			}

		}

		// Bean : Testing Text
		/*glm::vec3 position = { 0.f, -1.f, 0.f };
		color = { 1.f, 1.f, 0.f, 1.f };
		renderer->draw_text("Testing Arial", position, color, 0.1f, 0);*/

		renderer->end_batch();

		renderer->flush();
	}

	void Draw::editor()
	{
		renderer->begin_batch();

		// Grid system
		glm::vec4 color = { 1.f, 1.f, 1.f, 0.4f };
		float start = -100.f, end = -start;
		float numDivision = 24.f, iteration = (end - start) / numDivision;
		for (float i = start; i < end + iteration; i += iteration)
		{
			renderer->draw_line({ i, start }, { i, end }, color);
			renderer->draw_line({ start, i }, { end, i }, color);
		}

		float subDivision = 5.f;
		numDivision *= subDivision;
		iteration = (end - start) / numDivision;
		start = -10.f, end = -start;
		for (float i = start; i <= end; i += iteration)
		{
			renderer->draw_line({ i, start }, { i, end }, color);
			renderer->draw_line({ start, i }, { end, i }, color);
		}

		// Colliders
		Scene* scene = sm->get_current_scene();
		if (scene != nullptr)
		{
			color = { 0.3f, 1.f, 0.3f, 1.f };
			GameObject* gameObject = sm->get_selected_gameobject();
			if (gameObject != nullptr)
			{
				for (Component* component : gameObject->getComponents<BoxCollider2D>())
				{
					if (!component->Enabled())
						continue;

					BoxCollider2D* collider = reinterpret_cast<BoxCollider2D*>(component);
					AABB bounds = collider->getBounds();
					glm::vec2 pos0_1 = { bounds.min.to_glm()};
					glm::vec2 pos1_1 = { bounds.max.x, bounds.min.y };
					glm::vec2 pos2_1 = { bounds.max.to_glm()};
					glm::vec2 pos3_1 = { bounds.min.x, bounds.max.y };

					renderer->draw_line(pos0_1, pos1_1, color);
					renderer->draw_line(pos1_1, pos2_1, color);
					renderer->draw_line(pos2_1, pos3_1, color);
					renderer->draw_line(pos3_1, pos0_1, color);
				}

				for (Component* component : gameObject->getComponents<Button>())
				{
					Transform& t = gameObject->transform;

					glm::vec3 position = t.glmPosition();
					glm::vec2 size(t.glmScale().x, t.glmScale().y);
					float rotation = t.glmRotation().z;

					glm::mat4 translate = {
						glm::vec4(1.f, 0.f, 0.f, 0.f),
						glm::vec4(0.f, 1.f, 0.f, 0.f),
						glm::vec4(position.x, position.y, 1.f, 0.f),
						glm::vec4(0.f, 0.f, 0.f, 1.f)
					};

					float rad = glm::radians(rotation);

					glm::mat4 rotate = {
						glm::vec4(cos(rad), sin(rad), 0.f, 0.f),
						glm::vec4(-sin(rad), cos(rad), 0.f, 0.f),
						glm::vec4(0.f, 0.f, 1.f, 0.f),
						glm::vec4(0.f, 0.f, 0.f, 1.f)
					};

					glm::mat4 transform = translate * rotate;

					glm::vec4 color = { 0.3f, 1.f, 0.3f, 1.f };

					glm::vec4 pos0 = transform * glm::vec4(-size.x / 2, -size.y / 2, 1.f, 1.f);
					glm::vec4 pos1 = transform * glm::vec4(size.x / 2, -size.y / 2, 1.f, 1.f);
					glm::vec4 pos2 = transform * glm::vec4(size.x / 2, size.y / 2, 1.f, 1.f);
					glm::vec4 pos3 = transform * glm::vec4(-size.x / 2, size.y / 2, 1.f, 1.f);

					float minX = fminf(pos0.x, fminf(pos1.x, fminf(pos2.x, pos3.x)));
					float minY = fminf(pos0.y, fminf(pos1.y, fminf(pos2.y, pos3.y)));
					float maxX = fmaxf(pos0.x, fmaxf(pos1.x, fmaxf(pos2.x, pos3.x)));
					float maxY = fmaxf(pos0.y, fmaxf(pos1.y, fmaxf(pos2.y, pos3.y)));

					glm::vec2 pos0_1 = { minX, minY };
					glm::vec2 pos1_1 = { maxX, minY };
					glm::vec2 pos2_1 = { maxX, maxY };
					glm::vec2 pos3_1 = { minX, maxY };

					renderer->draw_line(pos0_1, pos1_1, color);
					renderer->draw_line(pos1_1, pos2_1, color);
					renderer->draw_line(pos2_1, pos3_1, color);
					renderer->draw_line(pos3_1, pos0_1, color);
				}
			}
		}

		renderer->end_batch();

		renderer->flush();
	}

	void Draw::debug()
	{
		renderer->begin_batch();
		// Bean: Temporary green dot in the centre of the scene
		glm::vec4 color = { 0.1f, 1.f, 0.1f, 1.f };
		glm::vec2 worldNDC{ 0 };
		glm::vec2 scale = { 0.01f, 0.01f };
		glm::vec2 cameraPos = editorSys->get_camera()->get_position();
		float zoom = editorSys->get_camera()->get_zoom();

		worldNDC = { cameraPos.x, cameraPos.y };
		scale *= zoom;

		renderer->end_batch();
		renderer->flush();

		renderer->begin_batch();

		// Button Colliders
		Scene* scene = sm->get_current_scene();
		renderer->set_line_width(1.5f);
		if (scene != nullptr)
		{
			for (GameObject* gameObject : scene->get_gameobjectvector())
			{
				for (Component* component : gameObject->getComponents<BoxCollider2D>())
				{
					if (!component->Enabled())
						continue;

					BoxCollider2D* collider = reinterpret_cast<BoxCollider2D*>(component);
					AABB bounds = collider->getBounds();
					glm::vec2 pos0_1 = { bounds.min.to_glm() };
					glm::vec2 pos1_1 = { bounds.max.x, bounds.min.y };
					glm::vec2 pos2_1 = { bounds.max.to_glm() };
					glm::vec2 pos3_1 = { bounds.min.x, bounds.max.y };

					renderer->draw_line(pos0_1, pos1_1, color);
					renderer->draw_line(pos1_1, pos2_1, color);
					renderer->draw_line(pos2_1, pos3_1, color);
					renderer->draw_line(pos3_1, pos0_1, color);
				}

				for (Component* component : gameObject->getComponents<Button>())
				{
					Transform& t = gameObject->transform;

					glm::vec3 position = t.glmPosition();
					glm::vec2 size(t.glmScale().x, t.glmScale().y);
					float rotation = t.glmRotation().z;

					glm::mat4 translate = {
						glm::vec4(1.f, 0.f, 0.f, 0.f),
						glm::vec4(0.f, 1.f, 0.f, 0.f),
						glm::vec4(position.x, position.y, 1.f, 0.f),
						glm::vec4(0.f, 0.f, 0.f, 1.f)
					};

					float rad = glm::radians(rotation);

					glm::mat4 rotate = {
						glm::vec4(cos(rad), sin(rad), 0.f, 0.f),
						glm::vec4(-sin(rad), cos(rad), 0.f, 0.f),
						glm::vec4(0.f, 0.f, 1.f, 0.f),
						glm::vec4(0.f, 0.f, 0.f, 1.f)
					};

					glm::mat4 transform = translate * rotate;

					glm::vec4 color = { 0.3f, 1.f, 0.3f, 1.f };

					glm::vec4 pos0 = transform * glm::vec4(-size.x / 2, -size.y / 2, 1.f, 1.f);
					glm::vec4 pos1 = transform * glm::vec4(size.x / 2, -size.y / 2, 1.f, 1.f);
					glm::vec4 pos2 = transform * glm::vec4(size.x / 2, size.y / 2, 1.f, 1.f);
					glm::vec4 pos3 = transform * glm::vec4(-size.x / 2, size.y / 2, 1.f, 1.f);

					float minX = fminf(pos0.x, fminf(pos1.x, fminf(pos2.x, pos3.x)));
					float minY = fminf(pos0.y, fminf(pos1.y, fminf(pos2.y, pos3.y)));
					float maxX = fmaxf(pos0.x, fmaxf(pos1.x, fmaxf(pos2.x, pos3.x)));
					float maxY = fmaxf(pos0.y, fmaxf(pos1.y, fmaxf(pos2.y, pos3.y)));

					glm::vec2 pos0_1 = { minX, minY };
					glm::vec2 pos1_1 = { maxX, minY };
					glm::vec2 pos2_1 = { maxX, maxY };
					glm::vec2 pos3_1 = { minX, maxY };

					renderer->draw_line(pos0_1, pos1_1, color);
					renderer->draw_line(pos1_1, pos2_1, color);
					renderer->draw_line(pos2_1, pos3_1, color);
					renderer->draw_line(pos3_1, pos0_1, color);
				}
			}
		}

		renderer->draw_quad({ worldNDC.x, worldNDC.y , 1.f }, scale, 0.f, color);

		renderer->end_batch();
		renderer->flush();
	}

	void Draw::development()
	{
		// Bean : Testing Animations
		renderer->begin_batch();
		/*AssetsSystem* assets = AssetsSystem::Instance();
		if (!assets->get_spritesheets().empty())
		{
			position = {-4.f, 1.f, 0.f};
			glm::vec2 size(2.f, 2.f);
			static GLuint animIndex = 0;
			GLuint indexSize = assets->get_spritesheets()[animID].get_size() - 1;

			GLfloat dt = (GLfloat) WindowsSystem::Instance()->get_delta_time();
			static float animTimer = 0.f;
			animTimer += dt;
			if (animTimer > 0.01f && toggleAnimation)
			{
				animTimer = 0.f;
				animIndex++;
			}

			if (animIndex > indexSize)
			{
				animIndex = 0;
			}

			GLuint id = 0;
			for (GLuint i = 0; i < assets->get_textures().size(); ++i)
			{
				if (assets->get_textures()[i].get_object_id() == assets->get_spritesheets()[animID].get_texture().get_object_id())
					id = i + 1;
			}

			renderer->draw_quad(position, size, 0.f, assets->get_spritesheets()[animID], animIndex, id);
		}*/

		Scene* scene = sm->get_current_scene();
		if (scene != nullptr)
		{
			for (GameObject* gameObject : scene->get_gameobjectvector())
			{
				for (Component* component : gameObject->getComponents<Text>())
				{
					if (!component->Enabled())
						continue;
					Text* text = reinterpret_cast<Text*>(component);
					text->render();
				}
			}

		}

		renderer->end_batch();

		renderer->flush();
	}
}
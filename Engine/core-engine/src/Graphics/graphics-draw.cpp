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

#include <GL/glew.h> // for access to OpenGL API declarations

#include "Graphics/graphics-draw.h"
#include "Graphics/graphics-system.h"
#include "Files/assets-system.h"
#include "Editor/editor-system.h"
#include "Debugging/frame-rate-controller.h"
#include "Windows/windows-input.h"

// Bean: remove this after NewManagerInstance is moved
#include "GameObject/Components/renderer-component.h"
#include "GameObject/Components/ui-components.h"
#include "GameObject/Components/collider-components.h"
#include "GameObject/Components/sorting-group-component.h"
#include "Animation/animation-system.h"
#include "SceneManager/scene-manager.h"
#include "Math/math-library.h"
#include "Graphics/fonts.h"
#include "glm/gtc/matrix_transform.hpp"

namespace Copium
{
	namespace
	{
		AssetsSystem* assets = AssetsSystem::Instance();
		EditorSystem* editorSys = EditorSystem::Instance();
		GraphicsSystem* graphics = GraphicsSystem::Instance();
		SceneManager* sm = SceneManager::Instance();
		InputSystem* inputSystem = InputSystem::Instance();

		bool toggleAnim = false;
	}

	void Draw::init(BaseCamera* _camera)
	{
		camera = _camera;
		renderer.init(_camera);

		enable(DRAW::EDITOR);
		enable(DRAW::WORLD);
		enable(DRAW::DEVELOPMENT);
	}

	void Draw::update(CameraType _type)
	{
		renderer.reset_stats();

		switch (_type)
		{
		case NONE:
			for (bool& value : drawMode) value = false;
			break;
		case GAME:
			for (bool& value : drawMode) value = false;
			drawMode[DRAW::WORLD] = true;
			drawMode[DRAW::DEVELOPMENT] = true;
			break;
		case SCENEVIEW:
			drawMode[DRAW::EDITOR] = true;
			drawMode[DRAW::WORLD] = true;
			drawMode[DRAW::DEVELOPMENT] = true;
			break;
		case PREVIEW:
			for (bool& value : drawMode) value = false;
			drawMode[DRAW::WORLD] = true;
			break;
		}

		// Clear the screen
		glm::vec4 clr = camera->get_bg_color();
		glClearColor(clr.r, clr.g, clr.b, clr.a);

		// Clear the screen bits
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT);


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
		camera = nullptr;
		renderer.shutdown();
	}

	void Draw::editor()
	{
		// Bean: this should be the background color of the camera
		//glClearColor(0.278f, 0.278f, 0.278f, 1.f);
		renderer.begin_batch();

		// Grid system
		glm::vec4 color = { 1.f, 1.f, 1.f, 0.4f };
		float start = -100.f, end = -start;
		float numDivision = 24.f, iteration = (end - start) / numDivision;
		for (float i = start; i < end + iteration; i += iteration)
		{
			renderer.draw_line({ i, start, -100.f }, { i, end, -100.f }, color);
			renderer.draw_line({ start, i, -100.f }, { end, i, -100.f }, color);
		}

		float subDivision = 5.f;
		numDivision *= subDivision;
		iteration = (end - start) / numDivision;
		start = -10.f, end = -start;
		for (float i = start; i <= end; i += iteration)
		{
			renderer.draw_line({ i, start, -100.f }, { i, end, -100.f }, color);
			renderer.draw_line({ start, i, -100.f }, { end, i, -100.f }, color);
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
					glm::vec3 pos0_1 = { bounds.min.to_glm(), 0.f };
					glm::vec3 pos1_1 = { bounds.max.x, bounds.min.y, 0.f };
					glm::vec3 pos2_1 = { bounds.max.to_glm(), 0.f };
					glm::vec3 pos3_1 = { bounds.min.x, bounds.max.y, 0.f };

					renderer.draw_line(pos0_1, pos1_1, color);
					renderer.draw_line(pos1_1, pos2_1, color);
					renderer.draw_line(pos2_1, pos3_1, color);
					renderer.draw_line(pos3_1, pos0_1, color);
				}

				for (Component* component : gameObject->getComponents<Button>())
				{
					Button* collider = reinterpret_cast<Button*>(component);
					AABB bounds = collider->getBounds();
					glm::vec3 pos0_1 = { bounds.min.to_glm(),0.f };
					glm::vec3 pos1_1 = { bounds.max.x, bounds.min.y,0.f };
					glm::vec3 pos2_1 = { bounds.max.to_glm(),0.f };
					glm::vec3 pos3_1 = { bounds.min.x, bounds.max.y,0.f };

					renderer.draw_line(pos0_1, pos1_1, color);
					renderer.draw_line(pos1_1, pos2_1, color);
					renderer.draw_line(pos2_1, pos3_1, color);
					renderer.draw_line(pos3_1, pos0_1, color);
				}
			}
		}

		renderer.end_batch();

		renderer.flush();
	}

	void Draw::world()
	{

		/*
			Bean Theory:
			For each layer, check if it is enabled
				If it is:
					For each gameobject in the layer, check if it has a renderer component
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
				Else
					Continue
		*/

		// Theory WIP
		renderer.begin_batch();
		Scene* scene = sm->get_current_scene();
		if (scene != nullptr)
		{
			if (scene->get_state() == Scene::SceneState::play)
				toggleAnim = true;
			else
				toggleAnim = false;

			int count = 0;

			// Draw non layered game objects first, followed by game objects in layers in the order of the layer
			for (GameObject* gameObject : scene->gameObjects)
			{
				if (!gameObject)
					continue;

				if (!gameObject->isActive())
					continue;

				bool layered{ false };
				for (Component * component : gameObject->getComponents<SortingGroup>())
				{
					if (component->Enabled())
					{
						layered = true;
						break;
					}
				}

				if (layered)
					continue;

				for (Component* component : gameObject->getComponents<SpriteRenderer>())
				{
					if (!component->Enabled())
						continue;

					Transform& t = gameObject->transform;
					SpriteRenderer* rc = reinterpret_cast<SpriteRenderer*>(component);
					Sprite& sr = rc->get_sprite_renderer();
					glm::vec2 size(t.scale.x, t.scale.y);
					float rotation = t.rotation.z;

					if (gameObject->transform.hasParent())
					{
						Transform* tempObj = gameObject->transform.parent;
						glm::vec3 updatedPos = t.position.glmVec3;
						glm::vec3 updatedScale = t.scale.glmVec3;
						float updatedRot = t.rotation.z;

						renderer.draw_quad(intermediate + t1.position, size, rotation + t1.rotation.z, sr);
					}
						while (tempObj)
						{
							glm::vec3 tempPos = tempObj->position.glmVec3;
							glm::mat4 translate = glm::translate(glm::mat4(1.f), tempPos);
						
							float rot = glm::radians(tempObj->rotation.z);
							size.x = tempObj->scale.x;
							size.y = tempObj->scale.y;
							glm::mat4 rotate = {
							glm::vec4(cos(rot), sin(rot), 0.f, 0.f),
							glm::vec4(-sin(rot), cos(rot), 0.f, 0.f),
							glm::vec4(0.f, 0.f, 1.f, 0.f),
							glm::vec4(0.f, 0.f, 0.f, 1.f)
							};

							glm::vec3 size = tempObj->scale.glmVec3;
							glm::mat4 scale = {
								glm::vec4(size.x, 0.f, 0.f, 0.f),
								glm::vec4(0.f, size.y, 0.f, 0.f),
								glm::vec4(0.f, 0.f, 1.f, 0.f),
								glm::vec4(0.f, 0.f, 0.f, 1.f)
							};

							glm::mat4 transform = translate * rotate * scale;

							updatedPos = glm::vec3(transform * glm::vec4(updatedPos,1.f));

							updatedScale *= tempObj->scale.glmVec3;
							updatedRot += tempObj->rotation.z;

							tempObj = tempObj->parent;
						}

						renderer.draw_quad(updatedPos, glm::vec2(updatedScale.x, updatedScale.y), updatedRot, sr);
					}
					else
					{
						renderer.draw_quad(t.position, size, rotation, sr);
					}

				}
				for (Component* component : gameObject->getComponents<ImageComponent>())
				{
					if (!component->Enabled())
						continue;

					Transform& t = gameObject->transform;
					ImageComponent* rc = reinterpret_cast<ImageComponent*>(component);
					Sprite& sr = rc->get_sprite_renderer();
					glm::vec2 size(t.scale.x, t.scale.y);
					float rotation = t.rotation.z;

					if (gameObject->transform.hasParent())
					{
						Transform& t1 = *gameObject->transform.parent;
						Copium::Math::Matrix3x3 rot;
						Copium::Math::matrix3x3_rotdeg(rot, t1.rotation.z);
						Copium::Math::Vec3 intermediate = (rot * t.position);

						renderer.draw_quad(intermediate + t1.position, size, rotation + t1.rotation.z, sr);
					}
					else
					{
						renderer.draw_quad(t.position, size, rotation, sr);
					}

					//renderer.draw_quad({ rc->Offset(),t.position.z }, size, rotation, sr);
				}
				for (Component* component : gameObject->getComponents<Text>())
				{
					if (!component->Enabled())
						continue;

					Text* text = reinterpret_cast<Text*>(component);
					text->render(camera);
				}
				for (Component* component : gameObject->getComponents<Animator>())
				{
					if (!component->Enabled())
						continue;

					Animator* animator = reinterpret_cast<Animator*>(component);
					Animation* anim = animator->GetCurrentAnimation();
					
					if (!anim || !anim->spriteSheet.GetTexture())
						continue;

					Transform& t = gameObject->transform;
					glm::vec2 size(t.scale.x, t.scale.y);
					float rotation = t.rotation.z;

					if (gameObject->transform.hasParent())
					{
						Transform& t1 = *gameObject->transform.parent;
						Copium::Math::Matrix3x3 rot;
						Copium::Math::matrix3x3_rotdeg(rot, t1.rotation.z);
						Copium::Math::Vec3 intermediate = (rot * t.position);

						renderer.draw_quad(intermediate + t1.position, size, rotation + t1.rotation.z, anim->spriteSheet, anim->currentFrameIndex, anim->frameCount);
					}
					else
					{
						renderer.draw_quad(t.position, size, t.rotation.z, anim->spriteSheet, anim->currentFrameIndex, anim->frameCount);
					}
				}

				++count;
			}

			//PRINT("Num of Rendered GO: " << count);
		}
		renderer.end_batch();
		renderer.flush();

		// Bean : Testing Text
		/*glm::vec3 position = { 0.f, -1.f, 0.f };
		color = { 1.f, 1.f, 0.f, 1.f };
		renderer.draw_text("Testing Arial", position, color, 0.1f, 0);*/



		renderer.begin_batch();
		if (scene != nullptr)
		{
			if (scene->get_state() == Scene::SceneState::play)
				toggleAnim = true;
			else
				toggleAnim = false;

			int count = 0;

			for (Layer& layer : editorSys->getLayers()->SortLayers()->GetSortingLayers())
			{
				int layerID{ 0 };
				int gameObjectCount{ 0 };
				for (GameObject* gameObject : layer.gameObjects)
				{
					if (gameObject == nullptr)
					{
						//PRINT("null");
						continue;

					}

					if (!gameObject->isActive())
					{
						//PRINT("game object is not active");
						continue;

					}

					// If the object isnt within the frustum
					if (!camera->withinFrustum(gameObject->transform.position, gameObject->transform.scale))
						continue;

					for (Component* component : gameObject->getComponents<SpriteRenderer>())
					{
						if (!component->Enabled())
							continue;

						Transform& t = gameObject->transform;
						SpriteRenderer* rc = reinterpret_cast<SpriteRenderer*>(component);
						Sprite& sr = rc->get_sprite_renderer();
						glm::vec2 size(t.scale.x, t.scale.y);
						float rotation = t.rotation.z;

						if (gameObject->transform.hasParent())
						{
							Transform& t1 = *gameObject->transform.parent;
							Copium::Math::Matrix3x3 rot;
							Copium::Math::matrix3x3_rotdeg(rot, t1.rotation.z);
							Copium::Math::Vec3 intermediate = (rot * t.position);

							renderer.draw_quad(intermediate + t1.position, size, rotation + t1.rotation.z, sr);
						}
						else
						{
							renderer.draw_quad(t.position, size, rotation, sr);
						}

					}
					for (Component* component : gameObject->getComponents<ImageComponent>())
					{
						if (!component->Enabled())
							continue;

						Transform& t = gameObject->transform;
						ImageComponent* rc = reinterpret_cast<ImageComponent*>(component);
						Sprite& sr = rc->get_sprite_renderer();
						glm::vec2 size(t.scale.x, t.scale.y);
						float rotation = t.rotation.z;

						if (gameObject->transform.hasParent())
						{
							Transform& t1 = *gameObject->transform.parent;
							Copium::Math::Matrix3x3 rot;
							Copium::Math::matrix3x3_rotdeg(rot, t1.rotation.z);
							Copium::Math::Vec3 intermediate = (rot * t.position);

							renderer.draw_quad(intermediate + t1.position, size, rotation + t1.rotation.z, sr);
						}
						else
						{
							renderer.draw_quad(t.position, size, rotation, sr);
						}

						//renderer.draw_quad({ rc->Offset(),t.position.z }, size, rotation, sr);
					}
					for (Component* component : gameObject->getComponents<Text>())
					{
						if (!component->Enabled())
							continue;

						Text* text = reinterpret_cast<Text*>(component);
						text->render(camera);
					}
					for (Component* component : gameObject->getComponents<Animator>())
					{
						Animator* animator = reinterpret_cast<Animator*>(component);
						Animation* anim = animator->GetCurrentAnimation();

						//if(anim)
						//	PRINT("bloopbloppp");

						if (!anim)
							continue;


						unsigned int sid = anim->spriteSheet.spriteID - 1;
						// The index of the texture must be less than the size of textures
						if (sid != -1 && sid < assets->get_textures().size())
						{
							anim->spriteSheet.texture = assets->get_texture(sid);
						}
						else
						{

							anim->spriteSheet.spriteID = 0;
							anim->spriteSheet.texture = nullptr;
						}

						if (!anim || !anim->spriteSheet.GetTexture())
							continue;
						//PRINT("bloopbloop");
						Transform& t = gameObject->transform;
						unsigned int textureID = anim->spriteSheet.GetTexture()->get_id();
						glm::vec2 size(t.scale.x, t.scale.y);

						GLuint nid = 0;
						for (GLuint i = 0; i < assets->get_textures().size(); ++i)
						{
							if (assets->get_textures()[i].get_object_id() == anim->spriteSheet.texture->get_object_id())
								nid = i + 1;
						}
						//renderer.draw_quad(t.position, size, t.rotation.z, nid);
						renderer.draw_quad(t.position, size, t.rotation.z, anim->spriteSheet, anim->currentFrameIndex, nid, anim->frameCount);

					}

					count++;
					gameObjectCount++;

				}
				//PRINT("Layer ID:" << layerID << " | No. of GameObjects:" << gameObjectCount);
				++layerID;
			}
		}
		renderer.end_batch();
		renderer.flush();
	}

	void Draw::debug()
	{
		renderer.begin_batch();
		// Bean: Temporary green dot in the centre of the scene
		glm::vec4 color = { 0.1f, 1.f, 0.1f, 1.f };
		glm::vec2 worldNDC{ 0 };
		glm::vec2 scale = { 0.01f, 0.01f };
		glm::vec2 cameraPos = editorSys->get_camera()->get_eye();
		float zoom = editorSys->get_camera()->get_zoom();

		worldNDC = { cameraPos.x, cameraPos.y };
		scale *= zoom;

		renderer.end_batch();
		renderer.flush();

		renderer.begin_batch();

		// Button Colliders
		Scene* scene = sm->get_current_scene();
		renderer.set_line_width(1.5f);
		if (scene != nullptr)
		{
			for (GameObject* gameObject : scene->gameObjects)
			{
				// If the object isnt within the frustum
				if (!camera->withinFrustum(gameObject->transform.position, gameObject->transform.scale))
					continue;

				for (Component* component : gameObject->getComponents<BoxCollider2D>())
				{
					if (!component->Enabled())
						continue;

					BoxCollider2D* collider = reinterpret_cast<BoxCollider2D*>(component);
					AABB bounds = collider->getBounds();
					glm::vec3 pos0_1 = { bounds.min.to_glm(), 0.f };
					glm::vec3 pos1_1 = { bounds.max.x, bounds.min.y, 0.f };
					glm::vec3 pos2_1 = { bounds.max.to_glm() , 0.f };
					glm::vec3 pos3_1 = { bounds.min.x, bounds.max.y, 0.f };

					renderer.draw_line(pos0_1, pos1_1, color);
					renderer.draw_line(pos1_1, pos2_1, color);
					renderer.draw_line(pos2_1, pos3_1, color);
					renderer.draw_line(pos3_1, pos0_1, color);
				}

				for (Component* component : gameObject->getComponents<Button>())
				{
					(void) component;
					Transform& t = gameObject->transform;

					glm::vec3 position = t.position;
					glm::vec2 size(t.scale.x, t.scale.y);
					float rotation = t.rotation.z;

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

					color = { 0.3f, 1.f, 0.3f, 1.f };

					glm::vec4 pos0 = transform * glm::vec4(-size.x / 2, -size.y / 2, 1.f, 1.f);
					glm::vec4 pos1 = transform * glm::vec4(size.x / 2, -size.y / 2, 1.f, 1.f);
					glm::vec4 pos2 = transform * glm::vec4(size.x / 2, size.y / 2, 1.f, 1.f);
					glm::vec4 pos3 = transform * glm::vec4(-size.x / 2, size.y / 2, 1.f, 1.f);

					float minX = fminf(pos0.x, fminf(pos1.x, fminf(pos2.x, pos3.x)));
					float minY = fminf(pos0.y, fminf(pos1.y, fminf(pos2.y, pos3.y)));
					float maxX = fmaxf(pos0.x, fmaxf(pos1.x, fmaxf(pos2.x, pos3.x)));
					float maxY = fmaxf(pos0.y, fmaxf(pos1.y, fmaxf(pos2.y, pos3.y)));

					glm::vec3 pos0_1 = { minX, minY, 0.f };
					glm::vec3 pos1_1 = { maxX, minY, 0.f };
					glm::vec3 pos2_1 = { maxX, maxY, 0.f };
					glm::vec3 pos3_1 = { minX, maxY, 0.f };

					renderer.draw_line(pos0_1, pos1_1, color);
					renderer.draw_line(pos1_1, pos2_1, color);
					renderer.draw_line(pos2_1, pos3_1, color);
					renderer.draw_line(pos3_1, pos0_1, color);
				}
			}
		}

		renderer.draw_quad({ worldNDC.x, worldNDC.y , 1.f }, scale, 0.f, color);

		renderer.end_batch();
		renderer.flush();
	}

	void Draw::development()
	{
		// Bean : Testing Animations
		renderer.begin_batch();

		// Bean: Enable if depth testing is disabled
		/*Scene* scene = sm->get_current_scene();
		if (scene != nullptr)
		{
			for (GameObject* gameObject : scene->gameObjects)
			{
				for (Component* component : gameObject->getComponents<Text>())
				{
					if (!component->Enabled())
						continue;

					Text* text = reinterpret_cast<Text*>(component);
					text->render(camera);
				}
			}
		}*/

		glm::vec2 scale = glm::vec2(1.f, 1.f);
		glm::vec3 pos = glm::vec3(-10.f, 3.f, 0.f);
		glm::vec4 color = glm::vec4(1.f, 1.f, 1.f, 1.f);

		static int count = 0;

		if (inputSystem->is_key_held(68))
			count++;

		int posX = -100, posY = 100;
		for (int i = 0; i < count; i++)
		{
			posX += 3;

			if (posX > 100)
			{
				posX = -100;
				posY -= 3;
			}
			pos = glm::vec3((float)posX, (float)posY, 0.f);
			renderer.draw_circle(pos, 1.f, 0.f, color);
		}

		//PRINT("Count: " << count);
		renderer.end_batch();

		renderer.flush();

		/*Font* font = Font::getFont("corbel");
		font->draw_text("Lorem ipsum dolor sit amet", pos, color, 0.3f, 0, camera);*/
	}
}
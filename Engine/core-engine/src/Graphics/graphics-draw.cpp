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
#include "Editor/editor-system.h"
#include "Debugging/frame-rate-controller.h"

// Bean: remove this after NewManagerInstance is moved
#include "Animation/animation-system.h"
#include "Math/math-library.h"
#include "glm/gtc/matrix_transform.hpp"
#include <GameObject/components.h>
#include <GameObject/game-object.h>
#include <SceneManager/scene.h>
#include "Events/events-system.h"

namespace Copium
{
	namespace
	{
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

		glm::vec4 clr = camera->get_bg_color();
		glClearColor(clr.r, clr.g, clr.b, clr.a);

		// Clear the screen bits
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT);


		if(drawMode[DRAW::EDITOR])
			editor();

		if (drawMode[DRAW::WORLD])
			world();

		if (drawMode[DRAW::EDITOR])
			editor(1);

		if (drawMode[DRAW::DEBUG])
			debug();

		if (drawMode[DRAW::DEVELOPMENT])
			development();
	}

	void Draw::exit()
	{
		PRINT("DRAW EXITTED!");
		camera = nullptr;
		renderer.shutdown();
	}


	void Draw::ResetRenderer()
	{
		glClear(GL_COLOR_BUFFER_BIT);
		renderer.end_batch();
		renderer.flush();
		renderer.begin_batch();
	}

	void Draw::editor(int _index)
	{
		// Bean: this should be the background color of the camera
		//glClearColor(0.278f, 0.278f, 0.278f, 1.f);
		renderer.begin_batch();

		if (_index == 0)
		{
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
		}

		if (_index == 1)
		{
			// Colliders
			Scene* pScene{ MySceneManager.get_current_scene() };
			if (pScene)
			{
				glm::vec4 color = { 1.f, 1.f, 1.f, 0.4f };
				color = { 0.3f, 1.f, 0.3f, 1.f };
				for (BoxCollider2D& collider : pScene->componentArrays.GetArray<BoxCollider2D>())
				{
					if (!collider.enabled || !collider.gameObj.IsActive())
						continue;
					Transform& transform = collider.gameObj.transform;
					AABB bounds = collider.bounds.GetRelativeBounds(transform.GetWorldPosition(), transform.GetWorldScale());
					glm::vec3 pos0_1 = { bounds.min.to_glm(), 0.f };
					glm::vec3 pos1_1 = { bounds.max.x, bounds.min.y, 0.f };
					glm::vec3 pos2_1 = { bounds.max.to_glm(), 0.f };
					glm::vec3 pos3_1 = { bounds.min.x, bounds.max.y, 0.f };

					renderer.draw_line(pos0_1, pos1_1, color);
					renderer.draw_line(pos1_1, pos2_1, color);
					renderer.draw_line(pos2_1, pos3_1, color);
					renderer.draw_line(pos3_1, pos0_1, color);
				}

				for (Button& button : pScene->componentArrays.GetArray<Button>())
				{
					if (!button.enabled || !button.gameObj.IsActive())
						continue;
					Transform& transform = button.gameObj.transform;
					AABB bounds = button.bounds.GetRelativeBounds(transform.GetWorldPosition(), transform.GetWorldScale());
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
		//Scene loaded
		Scene* pScene{ MySceneManager.get_current_scene() };
		if (pScene)
		{
			//if (scene->get_state() == Scene::SceneState::play)
			//	toggleAnim = true;
			//else
			//	toggleAnim = false;

			int count = 0;
			// Draw non layered game objects first, followed by game objects in layers in the order of the layer

			for (SpriteRenderer& spriteRenderer : pScene->componentArrays.GetArray<SpriteRenderer>())
			{
				if (!spriteRenderer.enabled || !spriteRenderer.gameObj.IsActive())
					continue;

				//if (spriteRenderer.gameObj.HasComponent<SortingGroup>())
				//	continue;

				Transform& t = spriteRenderer.gameObj.transform;
				Sprite& sr = spriteRenderer.sprite;
				glm::vec2 size(t.scale.x, t.scale.y);
				float rotation = t.rotation.z;

				if (t.HasParent())
				{
					glm::vec3 updatedPos = t.position.glmVec3;
					glm::vec3 updatedScale = t.scale.glmVec3;
					float updatedRot = t.rotation.z;
					UpdateTransform(t, updatedPos, updatedRot, updatedScale);

					// If the object isnt within the frustum
					if (!camera->withinFrustum(updatedPos, updatedScale))
						continue;

					renderer.draw_quad(updatedPos, { updatedScale.x, updatedScale.y }, updatedRot, sr);
				}
				else
				{
					// If the object isnt within the frustum
					if (!camera->withinFrustum(t.position, t.scale))
						continue;

					renderer.draw_quad(t.position, size, rotation, sr);
				}
			}
			for (Image& image: pScene->componentArrays.GetArray<Image>())
			{
				if (!image.enabled || !image.gameObj.IsActive())
					continue;

				if (image.gameObj.HasComponent<SortingGroup>())
					continue;

				Transform& t = image.gameObj.transform;
				Sprite& sr = image.sprite;
				glm::vec2 size(t.scale.x, t.scale.y);
				float rotation = t.rotation.z;

				if (t.HasParent())
				{
					glm::vec3 updatedPos = t.position.glmVec3;
					glm::vec3 updatedScale = t.scale.glmVec3;
					float updatedRot = t.rotation.z;
					UpdateTransform(t, updatedPos, updatedRot, updatedScale);

					// If the object isnt within the frustum
					if (!camera->withinFrustum(updatedPos, updatedScale))
						continue;

					renderer.draw_quad(updatedPos, { updatedScale.x, updatedScale.y }, updatedRot, sr);
				}
				else
				{
					// If the object isnt within the frustum
					if (!camera->withinFrustum(t.position, t.scale))
						continue;

					renderer.draw_quad(t.position, size, rotation, sr);
				}
			}
			for (Animator& animator: pScene->componentArrays.GetArray<Animator>())
			{
				if (!animator.enabled || !animator.gameObj.IsActive())
					continue;

				if (animator.gameObj.HasComponent<SortingGroup>())
					continue;

				// If the object isnt within the frustum
				if (!camera->withinFrustum(animator.gameObj.transform.position, animator.gameObj.transform.scale))
					continue;

				Animation* anim = animator.GetCurrentAnimation();

				if (!anim || !anim->spriteSheet.GetTexture())
					continue;

				Transform& t = animator.gameObj.transform;
				glm::vec2 size(t.scale.x, t.scale.y);

				if (t.HasParent())
				{
					glm::vec3 updatedPos = t.position.glmVec3;
					glm::vec3 updatedScale = t.scale.glmVec3;
					float updatedRot = t.rotation.z;
					UpdateTransform(t, updatedPos, updatedRot, updatedScale);

					// If the object isnt within the frustum
					if (!camera->withinFrustum(updatedPos, updatedScale))
						continue;

					renderer.draw_quad(updatedPos, { updatedScale.x, updatedScale.y }, updatedRot, anim->spriteSheet, anim->currentFrameIndex, anim->frameCount);
				}
				else
				{
					// If the object isnt within the frustum
					if (!camera->withinFrustum(t.position, t.scale))
						continue;

					renderer.draw_quad(t.position, size, t.rotation.z, anim->spriteSheet, anim->currentFrameIndex, anim->frameCount);
				}
			}
			for (Text& text : pScene->componentArrays.GetArray<Text>())
			{
				if (!text.enabled || !text.gameObj.IsActive())
					continue;

				if (text.gameObj.HasComponent<SortingGroup>())
					continue;

				// If the object isnt within the frustum
				if (!camera->withinFrustum(text.gameObj.transform.position, text.gameObj.transform.scale))
					continue;

				Transform& t = text.gameObj.transform;

				Math::Vec3 pos{ t.position };
				float scale = t.scale.x * 0.1f;
				if (scale > t.scale.y * 0.1f)
					scale = t.scale.y * 0.1f;

				scale *= text.fSize;
				glm::vec2 dimensions{ text.font->getDimensions(text.content, scale, text.wrapper) };

				switch (text.get_hAlign())
				{
				case HorizontalAlignment::Center:
					pos.x -= dimensions.x / 2.f;
					break;
				case HorizontalAlignment::Right:
					pos.x -= dimensions.x;
					break;
				}
				switch (text.get_vAlign())
				{
				case VerticalAlignment::Top:
					pos.y -= dimensions.y;
					break;
				case VerticalAlignment::Center:
					pos.y -= dimensions.y / 2.f;
					break;
				}

				if (t.HasParent())
				{
					glm::vec3 updatedPos = pos;
					glm::vec3 updatedScale = t.scale.glmVec3;
					float updatedRot = t.rotation.z;
					UpdateTransform(t, updatedPos, updatedRot, updatedScale);

					float updatedSize = updatedScale.x * text.fSize * 0.1f;

					renderer.draw_text(text.content, updatedPos, text.get_color(), scale, text.wrapper, text.font);
				}
				else
				{
					renderer.draw_text(text.content, pos, text.get_color(), scale, text.wrapper, text.font);
				}
			}
			++count;
			//PRINT("Num of Rendered GO: " << count);
		}
		renderer.end_batch();
		renderer.flush();

		// Gameobjects with Sorting Layers
		renderer.begin_batch();

		if (pScene)
		{
			int count = 0;
			for (Layer& layer : MyEditorSystem.getLayers()->SortLayers()->GetSortingLayers())
			{
				for (GameObject* go : layer.gameObjects)
				{
					if (!go || !go->IsActive())
						continue;

					// If the object isnt within the frustum
					if (!camera->withinFrustum(go->transform.position, go->transform.scale))
						continue;


					if (go->HasComponent<SpriteRenderer>())
					{
						SpriteRenderer& spriteRenderer = *go->GetComponent<SpriteRenderer>();
						if (!spriteRenderer.enabled || !spriteRenderer.gameObj.IsActive())
							continue;

						Transform& t = spriteRenderer.gameObj.transform;
						Sprite& sr = spriteRenderer.sprite;
						glm::vec2 size(t.scale.x, t.scale.y);
						float rotation = t.rotation.z;

						if (t.HasParent())
						{
							glm::vec3 updatedPos = t.position.glmVec3;
							glm::vec3 updatedScale = t.scale.glmVec3;
							float updatedRot = t.rotation.z;
							UpdateTransform(t, updatedPos, updatedRot, updatedScale);

							// If the object isnt within the frustum
							if (!camera->withinFrustum(updatedPos, updatedScale))
								continue;

							renderer.draw_quad(updatedPos, { updatedScale.x, updatedScale.y }, updatedRot, sr);
						}
						else
						{
							// If the object isnt within the frustum
							if (!camera->withinFrustum(t.position, t.scale))
								continue;

							renderer.draw_quad(t.position, size, rotation, sr);
						}
					}

					if (go->HasComponent<Image>())
					{
						Image& image = *go->GetComponent<Image>();
						if (!image.enabled || !image.gameObj.IsActive())
							continue;

						Transform& t = image.gameObj.transform;
						Sprite& sr = image.sprite;
						glm::vec2 size(t.scale.x, t.scale.y);
						float rotation = t.rotation.z;

						if (t.HasParent())
						{
							glm::vec3 updatedPos = t.position.glmVec3;
							glm::vec3 updatedScale = t.scale.glmVec3;
							float updatedRot = t.rotation.z;
							UpdateTransform(t, updatedPos, updatedRot, updatedScale);

							// If the object isnt within the frustum
							if (!camera->withinFrustum(updatedPos, updatedScale))
								continue;

							renderer.draw_quad(updatedPos, { updatedScale.x, updatedScale.y }, updatedRot, sr);
						}
						else
						{
							// If the object isnt within the frustum
							if (!camera->withinFrustum(t.position, t.scale))
								continue;

							renderer.draw_quad(t.position, size, rotation, sr);
						}
					}

					if (go->HasComponent<Animator>())
					{
						Animator& animator = *go->GetComponent<Animator>();

						if (!animator.enabled || !animator.gameObj.IsActive())
							continue;

						Animation* anim = animator.GetCurrentAnimation();

						if (!anim || !anim->spriteSheet.GetTexture())
							continue;

						Transform& t = animator.gameObj.transform;
						glm::vec2 size(t.scale.x, t.scale.y);

						if (t.HasParent())
						{
							glm::vec3 updatedPos = t.position.glmVec3;
							glm::vec3 updatedScale = t.scale.glmVec3;
							float updatedRot = t.rotation.z;
							UpdateTransform(t, updatedPos, updatedRot, updatedScale);

							// If the object isnt within the frustum
							if (!camera->withinFrustum(updatedPos, updatedScale))
								continue;

							renderer.draw_quad(updatedPos, { updatedScale.x, updatedScale.y }, updatedRot, anim->spriteSheet, anim->currentFrameIndex, anim->frameCount);
						}
						else
						{
							// If the object isnt within the frustum
							if (!camera->withinFrustum(t.position, t.scale))
								continue;

							renderer.draw_quad(t.position, size, t.rotation.z, anim->spriteSheet, anim->currentFrameIndex, anim->frameCount);
						}
					}

					if (go->HasComponent<Text>())
					{
						Text& text = *go->GetComponent<Text>();
						if (!text.enabled || !text.gameObj.IsActive())
							continue;

						Transform& t = text.gameObj.transform;

						Math::Vec3 pos{ t.position };
						float scale = t.scale.x * 0.1f;
						if (scale > t.scale.y * 0.1f)
							scale = t.scale.y * 0.1f;

						scale *= text.fSize;
						glm::vec2 dimensions{ text.font->getDimensions(text.content, scale, text.wrapper) };

						switch (text.get_hAlign())
						{
						case HorizontalAlignment::Center:
							pos.x -= dimensions.x / 2.f;
							break;
						case HorizontalAlignment::Right:
							pos.x -= dimensions.x;
							break;
						}
						switch (text.get_vAlign())
						{
						case VerticalAlignment::Top:
							pos.y -= dimensions.y;
							break;
						case VerticalAlignment::Center:
							pos.y -= dimensions.y / 2.f;
							break;
						}

						if (t.HasParent())
						{
							glm::vec3 updatedPos = pos;
							glm::vec3 updatedScale = t.scale.glmVec3;
							float updatedRot = t.rotation.z;
							UpdateTransform(t, updatedPos, updatedRot, updatedScale);

							float updatedSize = updatedScale.x * text.fSize * 0.1f;

							renderer.draw_text(text.content, updatedPos, text.get_color(), scale, text.wrapper, text.font);
						}
						else
						{
							renderer.draw_text(text.content, pos, text.get_color(), scale, text.wrapper, text.font);
						}
					}

				}
			}
		}

		renderer.end_batch();
		renderer.flush();

		// Only For Text
		if (pScene != nullptr)
		{
			//for (Text& text : pScene->componentArrays.GetArray<Text>())
			//{
			//	GameObject& gameObject{ text.gameObj };
			//	if (!text.enabled || !gameObject.IsActive())
			//		continue;

			//	// If the object isnt within the frustum
			//	if (!camera->withinFrustum(gameObject.transform.GetWorldPosition(), gameObject.transform.GetWorldScale()))
			//		continue;
			//	text.render(camera);
			//}

			//for (Layer& layer : MyEditorSystem.getLayers()->SortLayers()->GetSortingLayers())
			//{
			//	// Only For Text
			//	for (GameObject* go : layer.gameObjects)
			//	{
			//		if (go == nullptr || !go->IsActive())
			//			continue;

			//		// If the object isnt within the frustum
			//		if (!camera->withinFrustum(go->transform.position, go->transform.scale))
			//			continue;

			//		if (go->HasComponent<Text>())
			//		{
			//			Text& text = *go->GetComponent<Text>();
			//			if (!text.enabled || !text.gameObj.IsActive())
			//				continue;

			//			Transform& t = text.gameObj.transform;
			//			text.render(camera);
			//		}
			//	}
			//}

		}

		
	}

	void Draw::debug()
	{
		//renderer.begin_batch();
		// Bean: Temporary green dot in the centre of the scene
		glm::vec4 color = { 0.1f, 1.f, 0.1f, 1.f };
		glm::vec2 worldNDC{ 0 };
		glm::vec2 scale = { 0.01f, 0.01f };
		glm::vec2 cameraPos = MyEditorSystem.get_camera()->get_eye();
		float zoom = MyEditorSystem.get_camera()->get_zoom();

		//renderer.end_batch();
		//renderer.flush();

		renderer.begin_batch();

		// Button Colliders
		renderer.set_line_width(1.5f);
		Scene* pScene{ MySceneManager.get_current_scene() };
		if (pScene)
		{

			for (BoxCollider2D& boxCol: pScene->componentArrays.GetArray<BoxCollider2D>())
			{
				GameObject& gameObject{ boxCol.gameObj};
				Transform& transform{gameObject.transform};
				if (!boxCol.enabled || !gameObject.IsActive() ||
					// If the object isnt within the frustum
					!camera->withinFrustum(transform.GetWorldPosition(), transform.GetWorldScale()))
					continue;
				AABB bounds = boxCol.bounds.GetRelativeBounds(transform.GetWorldPosition(),transform.GetWorldScale());
				glm::vec3 pos0_1 = { bounds.min.to_glm(), 0.f };
				glm::vec3 pos1_1 = { bounds.max.x, bounds.min.y, 0.f };
				glm::vec3 pos2_1 = { bounds.max.to_glm() , 0.f };
				glm::vec3 pos3_1 = { bounds.min.x, bounds.max.y, 0.f };

				renderer.draw_line(pos0_1, pos1_1, color);
				renderer.draw_line(pos1_1, pos2_1, color);
				renderer.draw_line(pos2_1, pos3_1, color);
				renderer.draw_line(pos3_1, pos0_1, color);
			}

			for (Button& button : pScene->componentArrays.GetArray<Button>())
			{
				Transform& t = button.gameObj.transform;
				if (!button.enabled || !button.gameObj.IsActive() ||
					// If the object isnt within the frustum
					!camera->withinFrustum(t.GetWorldPosition(), t.GetWorldScale()))
					continue;
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

		//renderer.draw_quad({ worldNDC.x, worldNDC.y , 1.f }, scale, 0.f, color);

		renderer.end_batch();
		renderer.flush();
	}

	void Draw::development()
	{
		// Bean : Testing Animations
		renderer.begin_batch();

		// Bean: Enable if depth testing is disabled
		/*Scene* scene = MySceneManager.get_current_scene();
		if (scene != nullptr)
		{
			for (GameObject* gameObject : scene->gameObjects)
			{
				for (Component* component : gameObject->GetComponents<Text>())
				{
					if (!component.enabled)
						continue;

					Text* text = reinterpret_cast<Text*>(component);
					text->render(camera);
				}
			}
		}*/

		// Bean : Testing Circles
		glm::vec2 scale = glm::vec2(1.f, 1.f);
		glm::vec3 pos = glm::vec3(-10.f, 3.f, 0.f);
		glm::vec4 color = glm::vec4(1.f, 1.f, 1.f, 1.f);

		static int count = 0;

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

	void Draw::UpdateTransform(const Transform& _transform, glm::vec3& _position, float& _rotation, glm::vec3& _scale)
	{
		Transform* tempObj = _transform.parent;

		/*glm::mat4 translate = glm::translate(glm::mat4(1.f), _position);
		glm::mat4 rotation = {
		glm::vec4(cos(_rotation), sin(_rotation), 0.f, 0.f),
		glm::vec4(-sin(_rotation), cos(_rotation), 0.f, 0.f),
		glm::vec4(0.f, 0.f, 1.f, 0.f),
		glm::vec4(0.f, 0.f, 0.f, 1.f)
		};

		glm::mat4 scale = {
			glm::vec4(_scale.x, 0.f, 0.f, 0.f),
			glm::vec4(0.f, _scale.y, 0.f, 0.f),
			glm::vec4(0.f, 0.f, 1.f, 0.f),
			glm::vec4(0.f, 0.f, 0.f, 1.f)
		};
		glm::mat4 transform = translate * rotation * scale;*/

		while (tempObj)
		{
			glm::vec3 tempPos = tempObj->position.glmVec3;
			glm::mat4 pTranslate = glm::translate(glm::mat4(1.f), tempPos);

			float rot = glm::radians(tempObj->rotation.z);
			glm::mat4 pRotate = {
			glm::vec4(cos(rot), sin(rot), 0.f, 0.f),
			glm::vec4(-sin(rot), cos(rot), 0.f, 0.f),
			glm::vec4(0.f, 0.f, 1.f, 0.f),
			glm::vec4(0.f, 0.f, 0.f, 1.f)
			};

			glm::vec3 size = tempObj->scale.glmVec3;
			glm::mat4 pScale = {
				glm::vec4(size.x, 0.f, 0.f, 0.f),
				glm::vec4(0.f, size.y, 0.f, 0.f),
				glm::vec4(0.f, 0.f, 1.f, 0.f),
				glm::vec4(0.f, 0.f, 0.f, 1.f)
			};

			glm::mat4 pTransform = pTranslate * pRotate * pScale;

			_position = glm::vec3(pTransform * glm::vec4(_position, 1.f));

			_scale *= tempObj->scale.glmVec3;
			_rotation += tempObj->rotation.z;

			tempObj = tempObj->parent;
		}
	}
}
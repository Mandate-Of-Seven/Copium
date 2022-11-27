/*!***************************************************************************************
\file			game-nonscript.cpp
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			27/11/2022

\brief
    This file implements the bullets shooting in the game

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#include "pch.h"

#include "../Game/game-nonscript.h"
#include "../SceneManager/sm.h"
#include "../GameObject/Components/script-component.h"
#include "../GameObject/Components/renderer-component.h"
#include "../GameObject/Components/physics-components.h"
#include "../GameObject/Components/collider-components.h"
#include "../Editor/editor-system.h"
#include "../Files/assets-system.h"
#include "../Debugging/frame-rate-controller.h"

namespace Copium
{
    AssetsSystem* assets = AssetsSystem::Instance();
    EditorSystem* editorSys = EditorSystem::Instance();
    NewSceneManager* sm = NewSceneManager::Instance();
    MessageSystem* messageSystem = MessageSystem::Instance();

    GameObject* playerBullet;
    GameObject* enemyBullet;
    GameObject* player;

    void Game::init()
    {
        messageSystem->subscribe(MESSAGE_TYPE::MT_COLLISION_ENTER, this);

        Scene* scene = sm->get_current_scene();
        if (scene != nullptr && scene->get_state() == Scene::SceneState::play)
        {
            for (auto gameObj : scene->gameObjects)
            {
                // Get reference to a bullet
                if (!gameObj->get_name().compare("Player Bullet"))
                {
                    playerBullet = gameObj;
                }
                else if (!gameObj->get_name().compare("Enemy Bullet"))
                {
                    enemyBullet = gameObj;
                }
                else if (!gameObj->get_name().compare("player"))
                {
                    player = gameObj;
                }
            }
        }
    }

    void Game::update()
    {
        Scene* scene = sm->get_current_scene();
        if(scene != nullptr && scene->get_state() == Scene::SceneState::play)
        {
            //for(auto gameObj = scene->gameObjects.begin(); gameObj != scene->gameObjects.end(); ++gameObj)
            for (auto gameObj : scene->gameObjects)
            {
                for (auto component : gameObj->getComponents<Script>())
                {
                    // If the gameobject contains a unit body
                    if (!component->Name().compare("UnitBody")) 
                    {
                        // Run functions
                        if (unit_body(gameObj))
                            return;
                    }
                    // If the gameobject contains a combat bullet body
                    else if (!component->Name().compare("CombatBulletBody"))
                    {
                        if (combat_bullet_body())
                            return;
                    }
                }

                if(gameObj->transform.position.y > 100.f || gameObj->transform.position.y < -100.f)
                    MyGOF.destroy(gameObj);

                if (!gameObj->active)
                {
                    MyGOF.destroy(gameObj);
                    return;
                }
            }
        }
    }

    void Game::exit()
    {
        
    }

    void Game::handleMessage(MESSAGE_TYPE _mType)
    {
        if (_mType == MESSAGE_TYPE::MT_COLLISION_ENTER)
        {
            GameObject* collided = MESSAGE_CONTAINER::collisionEnter.collided;

            collided->active = false;
        }
    }

    bool Game::unit_body(GameObject* _gameObj)
    {
        if (!editorSys->get_game_view()->is_window_focused())
            return false;

        // Shoot bullets
        if (_gameObj->transform.position.y > 0.f) // Enemy
        {
            static double timer = 0;
            timer += MyFrameRateController.getDt();

            if (enemyBullet != nullptr && timer >= 0.5)
            {
                GameObject* go = MyGOF.instantiate(*enemyBullet);
                go->set_name("Enemy Bullet (Clone)");
                
                go->transform.position = _gameObj->transform.position;
                go->transform.position.y -= _gameObj->transform.scale.y * 0.5f;;
                Rigidbody2D* rb = go->getComponent<Rigidbody2D>();
                if (rb != nullptr)
                    rb->set_vel(Math::Vec2(0, -0.25f));

                timer = 0.0;

                return true;
            }
        }
        else if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !_gameObj->get_name().compare("player")) // Left mouse button
        {
            if (playerBullet != nullptr)
            {
                GameObject* go = MyGOF.instantiate(*playerBullet);
                go->set_name("Player Bullet (Clone)");
                if (player != nullptr)
                {
                    go->transform.position = player->transform.position;
                    go->transform.position.y += 0.6f;
                    Rigidbody2D* rb = go->getComponent<Rigidbody2D>();
                    if (rb != nullptr)
                        rb->set_vel(Math::Vec2(0, 0.25f));
                }

                return true;
            }
        }

        return false;
    }

    bool Game::combat_bullet_body()
    {
        return false;
    }
}
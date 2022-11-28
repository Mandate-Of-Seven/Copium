/*!***************************************************************************************
\file			message-types.h
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			27/09/2022

\brief
    This file holds the definitions of message types and containers

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#include <cstdint>
#include <vector>

#ifndef MESSAGE_TYPES_H
#define MESSAGE_TYPES_H

namespace Copium
{
    class GameObject;
    enum class MESSAGE_TYPE
    {
        //MT = MESSAGE_TYPE
        MT_MOUSE_MOVED,
        MT_MOUSE_CLICKED,
        MT_KEY_PRESSED,
        MT_COLLIDED,
        MT_SCRIPTING_UPDATED,
        MT_CREATE_CS_GAMEOBJECT,
        MT_REFLECT_CS_GAMEOBJECT,
        MT_START_PREVIEW,
        MT_STOP_PREVIEW,
        MT_TOGGLE_PERFORMANCE_VIEW,
        MT_WINDOW_MINIMIZE,
        MT_SCENE_OPENED,
        MT_SCENE_DESERIALIZED,
        MT_RELOAD_ASSETS,
        MT_ENGINE_INITIALIZED,
        MT_WAIT_SYSTEMS_TO_PREVIEW,
        MT_COLLISION_ENTER,
        MT_NONE // END, DO NOT INSERT BEYOND
    };

    namespace MESSAGE_CONTAINER
    {
        //MC = MESSAGE_CONTAINER
        struct MC_MOUSE_MOVED
        {
            double x, y;
        };

        ////Bitwise operator flags for key modifiers
        //enum class KEY_MOD
        //{
        //    L_CTRL      = 0b00000001,
        //    R_CTRL      = 0b00000010,
        //    L_ALT       = 0b00000100,
        //    R_ALT       = 0b00001000,
        //    L_SHIFT     = 0b00010000,
        //    R_SHIFT     = 0b00100000,
        //    HOLD        = 0b01000000,
        //    RELEASED    = 0b10000000,
        //};

        //struct KEY_PRESSED
        //{
        //    uint8_t  flags;
        //    uint32_t keyCode;
        //};

        struct FILESYSTEM_MUTEX
        {
            bool acquired;
        };

        struct REFLECT_CS_GAMEOBJECT
        {
            uint64_t gameObjID;
            std::vector<uint64_t> componentIDs;
        };

        struct ADD_OR_DELETE_GAMEOBJECT
        {
            uint64_t gameObjID;
        };

        struct COLLISION_ENTER
        {
            GameObject* collided;
            GameObject* collidee;
        };

        struct ADD_OR_DELETE_COMPONENT
        {
            uint64_t gameObjID;
            uint64_t componentID;
        };

        extern REFLECT_CS_GAMEOBJECT reflectCsGameObject;
        extern ADD_OR_DELETE_GAMEOBJECT addOrDeleteGameObject;
        extern ADD_OR_DELETE_COMPONENT addOrDeleteComponent;
        extern FILESYSTEM_MUTEX fileSystemMutex;
        extern COLLISION_ENTER collisionEnter;
    }
    //static std::vector<MESSAGE_CONTAINERS::MOUSE_MOVED> QUEUE_MOUSE_MOVED;
    //static std::vector<MESSAGE_CONTAINERS::KEY_PRESSED> QUEUE_KEY_PRESSED;
}
#endif // !MESSAGE_TYPES_H
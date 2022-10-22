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

#ifndef MESSAGE_TYPES_H
#define MESSAGE_TYPES_H

namespace Copium::Message
{
    enum class MESSAGE_TYPE
    {
        //MT = MESSAGE_TYPE
        MT_MOUSE_MOVED,
        MT_MOUSE_CLICKED,
        MT_KEY_PRESSED,
        MT_COLLIDED,
        MT_SCRIPTING_UPDATED,
        MT_REFLECT_CS_GAMEOBJECT,
        MT_NONE // END, DO NOT INSERT BEYOND
    };

    namespace MESSAGE_CONTAINER
    {
        //MC = MESSAGE_CONTAINER
        struct MC_MOUSE_MOVED
        {
            double x, y;
        };

        //Bitwise operator flags for key modifiers
        enum class KEY_MOD
        {
            L_CTRL      = 0b00000001,
            R_CTRL      = 0b00000010,
            L_ALT       = 0b00000100,
            R_ALT       = 0b00001000,
            L_SHIFT     = 0b00010000,
            R_SHIFT     = 0b00100000,
            HOLD        = 0b01000000,
            RELEASED    = 0b10000000,
        };

        struct KEY_PRESSED
        {
            uint8_t  flags;
            uint32_t keyCode;
        };

        struct REFLECT_CS_GAMEOBJECT
        {
            unsigned long ID;
        };

        static REFLECT_CS_GAMEOBJECT reflectCsGameObject;
    }
    //static std::vector<MESSAGE_CONTAINERS::MOUSE_MOVED> QUEUE_MOUSE_MOVED;
    //static std::vector<MESSAGE_CONTAINERS::KEY_PRESSED> QUEUE_KEY_PRESSED;
}
#endif // !MESSAGE_TYPES_H
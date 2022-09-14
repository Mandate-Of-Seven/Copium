#pragma once

namespace Copium::Message
{
    enum class MESSAGE_TYPE
    {
        MOUSE_MOVED = 0,
        MOUSE_CLICKED,
        KEY_PRESSED,
        COLLIDED,
        NONE // END, DO NOT INSERT BEYOND
    };

    namespace MESSAGE_CONTAINERS
    {
        struct MOUSE_MOVED
        {
            double x, y;
        };

        //Bitwise operator flags for key modifiers
        enum class KEY_MOD
        {
            L_CTRL      = 0x00000001,
            R_CTRL      = 0x00000010,
            L_ALT       = 0x00000100,
            R_ALT       = 0x00001000,
            L_SHIFT     = 0x00010000,
            R_SHIFT     = 0x00100000,
            HOLD        = 0x01000000,
            RELEASED    = 0x10000000,
        };

        struct KEY_PRESSED
        {
            uint8_t  flags;
            uint32_t keyCode;
        };


    }

    static std::vector<MESSAGE_CONTAINERS::MOUSE_MOVED> QUEUE_MOUSE_MOVED;
    static std::vector<MESSAGE_CONTAINERS::KEY_PRESSED> QUEUE_KEY_PRESSED;
}
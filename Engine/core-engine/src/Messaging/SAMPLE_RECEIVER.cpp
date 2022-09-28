#include "../Messaging/SAMPLE_RECEIVER.h"

namespace Copium::Message
{

    DUMMY_RECEIVER::DUMMY_RECEIVER()
    {
        MessageSystem::Instance()->subscribe(MESSAGE_TYPE::MT_MOUSE_CLICKED,this);
    }

    void DUMMY_RECEIVER::handleMessage(Message::MESSAGE_TYPE mType)
    {
        switch (mType)
        {
            case MESSAGE_TYPE::MT_MOUSE_CLICKED:
            {
                PRINT("MOUSE_CLCIKED");
                //Behaviour to handle mouseclicked

                //Create your own structs to store messageData under message-system.h, MESSAGE_CONTAINERS namespace
            }
        }

    }
}
/*!***************************************************************************************
\file			message-system.cpp
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			27/09/2022

\brief
    This file holds the definition of functions for message-system

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#include "pch.h"
#include "Messaging/message-system.h"

namespace Copium::Message
{

    MessageSystem::MessageSystem() 
    {

    }

    void Message::MessageSystem::subscribe(MESSAGE_TYPE mType, IReceiver* pReceiver)
    {
        messageTypeListeners[mType].push_back(pReceiver);
    }


    void MessageSystem::dispatch(MESSAGE_TYPE mType)
    {
        for (IReceiver* receiver : messageTypeListeners[mType])
        {
            receiver->handleMessage(mType);
        }
    }

    void MessageSystem::init()
    {
        for (int i = 0; i < int(MESSAGE_TYPE::MT_NONE); ++i)
        {
            messageTypeListeners.insert({ MESSAGE_TYPE(i),std::vector<IReceiver*>() });
        }
    }

    void MessageSystem::update()
    {

    }

    void MessageSystem::exit()
    {

    }
}
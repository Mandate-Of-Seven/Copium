/*!***************************************************************************************
\file			message-system.cpp
\project
\author			Zacharie Hong

\par			Course: GAM250
\par			Section:
\date			27/09/2022

\brief
    This file holds the definition of functions for message-system

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#include "pch.h"
#include "Messaging/message-system.h"

namespace Copium
{

    MessageSystem::MessageSystem() 
    {

    }

    void MessageSystem::subscribe(MESSAGE_TYPE mType, IReceiver* pReceiver)
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

    void MessageSystem::unsubscribe(MESSAGE_TYPE mType, IReceiver* pReceiver)
    {
        auto& listeners = messageTypeListeners[mType];
        auto it = listeners.begin();
        while (it != listeners.end())
        {
            if (pReceiver == *it)
            {
                messageTypeListeners[mType].erase(it);
                return;
            }
            ++it;
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
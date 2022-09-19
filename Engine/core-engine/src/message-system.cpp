#include "pch.h"
#include "message-system.h"

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
        using typeToListeners = std::pair<MESSAGE_TYPE, std::vector<IReceiver*>>;

        for (IReceiver* receiver : messageTypeListeners[mType])
        {
            receiver->handleMessage(mType);
        }
        messageTypeListeners.clear();
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
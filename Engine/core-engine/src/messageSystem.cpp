#include "pch.h"
#include "message-system.h"

namespace Copium::Message
{
    std::map<MESSAGE_TYPE, std::vector<IReceiver*>> MessageSystem::messageTypeListeners{};

    MessageSystem::MessageSystem()
    {
        
    }

    void MessageSystem::init() 
    {
        for (int i = 0; i < int(MESSAGE_TYPE::NONE); ++i)
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
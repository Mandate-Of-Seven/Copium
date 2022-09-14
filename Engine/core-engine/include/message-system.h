#include "system-interface.h"
#include "message-types.h"

#ifndef MESSAGE_SYSTEM_H

#define MESSAGE_SYSTEM_H


namespace Copium::Message
{

    class IReceiver
    {
        public:
            virtual void handleMessage(MESSAGE_TYPE mType) = 0;
    };

    CLASS_SYSTEM(MessageSystem)
    {
    public:
        void init();
        void update();
        void exit();

        MessageSystem();

        static void subscribe(MESSAGE_TYPE mType, IReceiver* pReceiver)
        {
            messageTypeListeners[mType].push_back(pReceiver);
        }


        static void dispatch(MESSAGE_TYPE mType)
        {
            using typeToListeners = std::pair<MESSAGE_TYPE, std::vector<IReceiver*>>;
            
            for (IReceiver* receiver : messageTypeListeners[mType])
            {
                receiver->handleMessage(mType);
            }
            messageTypeListeners.clear();
        }

    private:
        static std::map<MESSAGE_TYPE, std::vector<IReceiver*>> messageTypeListeners;
    };
}

#endif //!MESSAGE_SYSTEM_H
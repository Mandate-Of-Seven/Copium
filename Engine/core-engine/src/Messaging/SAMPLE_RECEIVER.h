#include "message-system.h"


namespace Copium::Message
{
    //Receiving messages from MessageSystem, MUST INHERIT FROM RECEIVER
    class DUMMY_RECEIVER : public IReceiver
    {
        public:
            //Constructor
            DUMMY_RECEIVER();
            void handleMessage(Message::MESSAGE_TYPE mType);
    };
}

/*!***************************************************************************************
\file			message-system.h
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			27/09/2022

\brief
    This file holds the declaration of functions for message-system.cpp.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#include "system-interface.h"
#include "message-types.h"

#ifndef MESSAGE_SYSTEM_H

#define MESSAGE_SYSTEM_H
namespace Copium::Message
{

    class IReceiver
    {
        public:
            /**************************************************************************/
            /*!
              \brief
                Interface function for MessageSystem to call for IReceivers to handle
                a messageType
            */
            /**************************************************************************/
            virtual void handleMessage(MESSAGE_TYPE mType) = 0;
    };

    CLASS_SYSTEM(MessageSystem)
    {
    public:
        /**************************************************************************/
        /*!
          \brief
            Default constructor that does nothing
        */
        /**************************************************************************/
        MessageSystem();

        /**************************************************************************/
        /*!
          \brief
            Iterates through MESSAGE_TYPE in message-types.h and stores them as
            a key to empty vectors a std::map named messageTypeListeners
        */
        /**************************************************************************/
        void init();

        /**************************************************************************/
        /*!
          \brief
            Handles all the messages sent out by iterating through each message type
            key and its receivers and calling their handleMessage function.
        */
        /**************************************************************************/
        void update();

        /**************************************************************************/
        /*!
          \brief
            Cleanup the system for exit
        */
        /**************************************************************************/
        void exit();

        /**************************************************************************/
        /*!
          \brief
            Subscribes IReceiver to a type of message
          \param mType
            Type of message
          \param pReceiver
            Receiver to be subscribed
            
        */
        /**************************************************************************/
        void subscribe(MESSAGE_TYPE mType, IReceiver * pReceiver);

        /**************************************************************************/
        /*!
        * NOTE PARTIAL IMPLEMENTATION ONLY, right now when an event is dispatched,
        * it is handled immediately
          \brief
            Dispatches a type of message for update to handle with the receivers
          \param mType
            Type of message dispatched

        */
        /**************************************************************************/
        void dispatch(MESSAGE_TYPE mType);

    private:
        std::map<MESSAGE_TYPE, std::vector<IReceiver*>> messageTypeListeners;
    };
}

#endif //!MESSAGE_SYSTEM_H
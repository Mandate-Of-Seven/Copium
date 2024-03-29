﻿/*!***************************************************************************************
\file			events-system.h
\project		GAM200
\author			Zacharie Hong

\par			Course: GAM250
\par			Section: 
\date

\brief
    This file contains the declarations of the following:
    1. IEventHandler
    2. MemberFunctionHandler
    3. Event System

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/



#ifndef EVENTS_SYSTEM_H

#define EVENTS_SYSTEM_H

#include <Events/events.h>
#include <typeindex>
#include <CopiumCore/system-interface.h>

#define AcquireMutex(MutexType) bool mutex{false}; while(!mutex)MyEventSystem.publish(new Copium::AcquireMutexEvent{MutexType,mutex});
#define ReturnMutex(MutexType) MyEventSystem.publish(new Copium::ReturnMutexEvent{MutexType});

#define MyEventSystem Copium::EventSystem::Instance()

namespace Copium
{

    class IEventHandler
    {
    public:
        void exec(IEvent* evnt)
        {
            call(evnt);
        }
        virtual ~IEventHandler() {};
    private:
        // Implemented by MemberFunctionHandler
        virtual void call(IEvent* evnt) = 0;
    };

    template<class T, class EventType>
    class MemberFunctionHandler : public IEventHandler
    {
    public:
        typedef void (T::* MemberFunction)(EventType*);

        MemberFunctionHandler(T* instance, MemberFunction memberFunction) : instance{ instance }, memberFunction{ memberFunction } {};

        void call(IEvent* evnt) {
            // Cast event to the correct type and call member function
            (instance->*memberFunction)(static_cast<EventType*>(evnt));
        }
    private:
        // Pointer to class instance
        T* instance;

        // Pointer to member function
        MemberFunction memberFunction;
    };

    typedef std::list<IEventHandler*> HandlerList;
    CLASS_SYSTEM(EventSystem) {
    private:
        std::map<std::type_index, HandlerList*> subscribers;
    public:

        /*******************************************************************************
        /*!
        *
        \brief
            Init the IEventHandler

        \return
            void
        */
        /*******************************************************************************/
        void init() {}
        /*******************************************************************************
        /*!
        *
        \brief
            Update the IEventHandler

        \return
            void
        */
        /*******************************************************************************/
        void update() {}
        /*******************************************************************************
        /*!
        *
        \brief
            Cleanup the IEventHandler

        \return
            void
        */
        /*******************************************************************************/
        void exit()
        {
            for (auto& keyPair : subscribers)
            {
                auto it{ keyPair.second->begin() };
                while (it != keyPair.second->end())
                {
                    delete* it;
                    ++it;
                }
                delete keyPair.second;
            }
        }

        /*******************************************************************************
        /*!
        *
        \brief
            Sends a signal that an event has occurred and also sends the container with the
            variables

        \param evnt
            where the parameters that are to be passed on are stored

        \return
            void
        */
        /*******************************************************************************/
        template<typename EventType>
        void publish(EventType * evnt)
        {
            HandlerList* handlers = subscribers[typeid(EventType)];

            if (handlers == nullptr) {
                return;
            }

            for (auto handler : *handlers) {
                if (handler != nullptr) {
                    handler->exec(evnt);
                }
            }
            delete evnt;
        }

        /*******************************************************************************
        /*!
        *
        \brief
            Become a listener to the event specified

        \param instance
            the object that owns the specified member function
            
        \param memberFunction
            the member function of the instance

        \return
            void
        */
        /*******************************************************************************/
        template<class T, class EventType>
        void subscribe(T * instance, void (T:: * memberFunction)(EventType*)) {
            HandlerList* handlers = subscribers[typeid(EventType)];

            //First time initialization
            if (handlers == nullptr) {
                handlers = new HandlerList();
                subscribers[typeid(EventType)] = handlers;
            }

            handlers->push_back(new MemberFunctionHandler<T, EventType>(instance, memberFunction));
        }
    };



}

#endif // !EVENTS_SYSTEM_H

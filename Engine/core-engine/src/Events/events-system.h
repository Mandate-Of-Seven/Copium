/*!***************************************************************************************
****
\file			
\project		
\author			
\co-authors		// DELETE IF NOT APPLICABLE

\par			Course: GAM200
\par			Section: 
\date			//DATE LAST MODIFIED

\brief
	// BRIEF DESCRIPTION OF FILE PURPOSE

All content © 2023 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************
****/



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
        void init() {}
        void update() {}
        void exit()
        {
            PRINT("FREEING EVENTS");
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

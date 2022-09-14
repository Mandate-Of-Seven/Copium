#include "pch.h"

#ifndef SYSTEM_INTERFACE_H

#define SYSTEM_INTERFACE_H

#define CLASS_SYSTEM(TYPE) class TYPE : public ISystem, public Singleton<TYPE>

namespace Copium
{
    //Derived Class = T
    template <typename T>
    class Singleton
    {
    public:
        Singleton<T>(Singleton<T>& other)   = delete;
        void operator=(const Singleton<T>&) = delete;

        Singleton<T>()
        {
            static bool exists{false};
            if (exists)
            {
                PRINT("Singleton of " << typeid(T).name() << " already exists!");
                abort();
            }
            PRINT("Singleton of " << typeid(T).name() << " created!");
            exists = true;
        }
    };

    class ISystem
    {
    public:
        virtual void init()     = 0;
        virtual void update()   = 0;
        virtual void exit()     = 0;
    };


}


#endif // !SYSTEM_INTERFACE_H

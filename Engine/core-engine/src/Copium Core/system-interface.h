#include "pch.h"

#ifndef SYSTEM_INTERFACE_H

#define SYSTEM_INTERFACE_H

/**************************************************************************/
/*!
  \brief
    Macro to define start of the class to abstract implementation of Singleton
    and ISystem into a single call
  \param TYPE
    Class name
*/
/**************************************************************************/
#define CLASS_SYSTEM(TYPE) class TYPE final : public ISystem, public Singleton<TYPE>

namespace Copium
{
    /**************************************************************************/
    /*!
      \brief
        Type based Singleton, restrict instantiating a Singleton of same types
        so it is suitable for single-instance classes like the different
        systems
    */
    /**************************************************************************/
    //Derived Class = T
    template <typename T>
    class Singleton
    {
    public:
        Singleton<T>(Singleton<T>& other)   = delete;
        void operator=(const Singleton<T>&) = delete;

    /**************************************************************************/
    /*!
      \brief
        Gets the unique instance of type T and returns it as a pointer

      \return
        Pointer to unique class T.
    */
    /**************************************************************************/
        static T* Instance()
        {   
            static T instance;
            return &instance;
        }
    protected:
        Singleton<T>() 
        { 
            PRINT("SINGLETON OF " << typeid(T).name() << " CREATED!");
        }
    };


    /**************************************************************************/
    /*!
      \brief
        Pure virtual class for systems to implement
    */
    /**************************************************************************/
    class ISystem
    {
    public:
        virtual void init()     = 0;
        virtual void update()   = 0;
        virtual void exit()     = 0;
    };


}


#endif // !SYSTEM_INTERFACE_H

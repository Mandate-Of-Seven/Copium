/*!***************************************************************************************
\file			system-interface.h
\project
\author			Zacharie Hong
\co-authors

\par			Course: GAM200
\par			Section:
\date			18/09/2022

\brief
    Contains declarations for ISystem and Singleton<T> for other systems to inherit
    off to have single instanced systems and overridden member functions

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/

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
        /**************************************************************************/
        /*!
          \brief
            Runs when engine starts
        */
        /**************************************************************************/
        virtual void init()     = 0;
        /*!
          \brief
            Runs every frame of the engine
        */
        /**************************************************************************/
        virtual void update()   = 0;
        /*!
          \brief
            Run when the engine exits
        */
        /**************************************************************************/
        virtual void exit()     = 0;
    };


}


#endif // !SYSTEM_INTERFACE_H

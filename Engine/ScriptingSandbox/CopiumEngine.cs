/*!***************************************************************************************
****
\file			CopiumEngine.cs
\project
\author			Zacharie Hong
\co-authors

\par			Course: GAM200
\par			Section:
\date			26/09/2022

\brief
    BaseClass for other C# scripts to inherit off

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/

using System;
using System.Runtime.CompilerServices;

namespace CopiumEngine
{
    public class CopiumScript
    {
        public static class Debug
        {

            /*******************************************************************************
            /*!
            *
                \brief
                    Logs a message to Engine Console
            */
            /*******************************************************************************/
            public static void Log()
            {
                Console.WriteLine("Debug.Log called");
            }

            /*******************************************************************************
            /*!
            *
                \brief
                    Logs a warning message to Engine Console
            */
            /*******************************************************************************/
            public static void LogWarning()
            {

            }

            /*******************************************************************************
            /*!
            *
                \brief
                    Logs an error message to Engine Console and stops/prevents play mode
            */
            /*******************************************************************************/
            public static void LogError()
            {

            }
        }

        public static class Input
        {
            /*******************************************************************************
            /*!
            *
                \brief
                    Checks if a key was pressed

                \param keyCode
                    KeyCode to listen for

                \return
                    True if key was pressed
            */
            /*******************************************************************************/
            [MethodImplAttribute(MethodImplOptions.InternalCall)]  
            public static extern bool GetKey(int keyCode);

            /*******************************************************************************
            /*!
            *
                \brief
                    Checks if a key just pressed

                \param keyCode
                    KeyCode to listen for

                \return
                    True if key was just pressed
            */
            /*******************************************************************************/
            [MethodImplAttribute(MethodImplOptions.InternalCall)]  
            public static extern bool GetKeyDown(int keyCode);

            /*******************************************************************************
            /*!
            *
                \brief
                    Checks if a key just released

                \param keyCode
                    KeyCode to listen for

                \return
                    True if key was just released
            */
            /*******************************************************************************/
            [MethodImplAttribute(MethodImplOptions.InternalCall)]  
            public static extern bool GetKeyUp(int keyCode);
        }

        public class Vector2
        {
            public double x,y;
        }

        public class Vector3
        {
            public double x,y,z;
            public Vector3(double _x=0,double _y=0,double _z=0)
            {
                x = _x; y = _y; z = _z;
            }
        }

        // public class Component
        // {

        // }

        public class GameObject
        {
            //Query
            // public Component GetComponent<T>()
            // {
            //     if (!(T is Component))
            //         return null;
            // }
        }

        public class Transform
        {
            public Vector3 position;
            //public Vector3 scale;
            //Query
            // public Component GetComponent<T>()
            // {
            //     if (!(T is Component))
            //         return null;
            // }
        }

        protected Transform transform;
    }
}
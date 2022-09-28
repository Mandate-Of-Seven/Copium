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

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/

using System;

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
            public static bool GetKey(int keyCode)
            {
                return false;
            }

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
            public static bool GetKeyDown(int keyCode)
            {
                return false;
            }

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
            public static bool GetKeyUp(int keyCode)
            {
                return false;
            }
        }

        public static class Vector2
        {

        }

        public static class Vector3
        {

        }
    }

}
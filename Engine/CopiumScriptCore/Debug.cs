/*!***************************************************************************************
\file			Debug.cs
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			1/11/2022

\brief
	This file encapsulates the debugging features of the Engine

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/
using System;

namespace CopiumEngine
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
}

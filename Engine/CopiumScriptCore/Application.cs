﻿/*!***************************************************************************************
****
\file			Application.cs
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			28/11/2022

\brief
    Application related functions

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CopiumEngine
{
    static class Application
    {
        /*******************************************************************************
        /*!
        * 
        \brief
	        Quits the application
        */
        /*******************************************************************************/
        public static void Quit()
        {
            InternalCalls.QuitGame();
        }
    }
}
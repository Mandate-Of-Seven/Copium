/*!***************************************************************************************
\file			Input.cs
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			1/11/2022

\brief
	This file encapsulates the Input of the Engine

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

namespace CopiumEngine
{
    public class Input
    {
        public static bool GetKey(KeyCode keyCode) { return InternalCalls.GetKey((int)keyCode); }

        public static bool GetKeyDown(KeyCode keyCode) { return InternalCalls.GetKeyDown((int)keyCode); }

        public static bool GetKeyUp(KeyCode keyCode) { return InternalCalls.GetKeyUp((int)keyCode); }
    }
}

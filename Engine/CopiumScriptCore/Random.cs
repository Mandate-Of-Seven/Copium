/*!***************************************************************************************
\file			Random.cs
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			1/11/2022

\brief
	This file contains definitions for C# Random

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

using System;

namespace CopiumEngine
{

    public static class RNG
    {

        private static System.Random rnd = new System.Random();
        public static float Range (float min, float max)
        {
            return (float)((max - min) * rnd.NextDouble() + min);
        }

        public static int RandInt(int min, int max)
        {
            return rnd.Next(min, max + 1);
        }
    }
}

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

namespace CopiumEngine
{
    public class CopiumScript
    {
        
        private void OnCreate(ulong ID)
        {
            Console.WriteLine("GameObject Created in CS: " + ID);
            if (gameObject != null && ID == gameObject.ID)
                return;
            gameObject = GameObject.FindByID(ID);
            transform = gameObject.transform;
        }
        protected Transform transform;
        protected GameObject gameObject;
    }
}
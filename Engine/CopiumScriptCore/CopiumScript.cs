/*!***************************************************************************************
****
\file			CopiumScript.cs
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
    public class CopiumScript : Component
    {
        private void OnCreate(ulong ID)
        {
            if (gameObject != null && ID == gameObject.ID)
                return;
            Console.WriteLine(this.GetType().Name + " Script linked with GameObject of ID: (" + ID + ") in C#");
            gameObject = GameObject.FindByID(ID);
            transform = gameObject.transform;
        }

        public static T Instantiate<T>(T original, Vector3 pos, Vector3 rotation)
        {
            return original;
        }

        private Component FindComponentByID(ulong componentID, ulong gameObjectID)
        {
            Console.WriteLine("FINDING COMPONENT BY ID IN C#______________________________________________");
            foreach (Component component in GameObject.components)
            {
                if (component.ID == componentID)
                {
                    return component;
                }
            }
            Component component1 = new Component();
            component1.Initialize(GameObject.FindByID(gameObjectID), componentID);
            GameObject.components.Add(component1);
            return component1;
        }

        public static void Destroy(GameObject gameObj)
        {

        }
    }
}
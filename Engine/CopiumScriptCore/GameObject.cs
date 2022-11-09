/*!***************************************************************************************
\file			GameObject.cs
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			1/11/2022

\brief
	This file encapsulates the GameObject of the Engine

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

using System.Collections.Generic;
using System;

namespace CopiumEngine
{

    public class GameObject
    {
        GameObject()
        {
            transform = new Transform();
            transform.gameObject = this;
            gameObjects.Add(this);
        }
        public Transform transform;
        public ulong ID;
        private void setID(ulong _ID)
        {
            ID = _ID;
        }
        private static List<GameObject> gameObjects = new List<GameObject>(25000);
        public static GameObject FindByID(ulong _ID)
        {
            foreach (GameObject gameObj in gameObjects)
            {
                if (gameObj.ID == _ID)
                {
                    return gameObj;
                }
            }
            return null;
        }
        public static GameObject FindByName(string name)
        {
            ulong _ID = InternalCalls.FindGameObjByName(name);
            foreach (GameObject gameObj in gameObjects)
            {
                if (gameObj.ID == _ID)
                {
                    return gameObj;
                }
            }
            return null;
        }
        public bool HasComponent<T>() where T : Component, new()
        {
            Type componentType = typeof(T);
            return InternalCalls.HasComponent(ID, componentType);
        }


        //COME BACK AND OPTIMISE THIS BECAUSE ITS CREATING A NEW COMPONENT EVERYTIME
        public T GetComponent<T>() where T : Component, new()
        {
            if (!HasComponent<T>())
                return null;

            T component = new T() { gameObject = this };
            return component;
        }
    }
}

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
            transform.Initialize(this, 0);
            gameObjects.Add(this);
        }
        public Transform transform;
        public ulong ID;
        private void setID(ulong _ID)
        {
            ID = _ID;
        }

        public bool activeSelf
        {
            get
            {
                return InternalCalls.GetActive(ID);
            }
        }

        private static List<GameObject> gameObjects = new List<GameObject>(25000);
        private List<Component> components = new List<Component>(5);

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
            {
                Console.WriteLine("DOES NOT HAVE COMPONENT!: " + ID);
                return null;
            }

            T component = new T() { gameObject = this };
            return component;
        }

        private void AttachComponentByID(ulong componentID)
        {
            foreach (Component component in components)
            {
                if (component.ID == componentID)
                    return;
            }
            Component component1 = new Component();
            component1.Initialize(this, componentID);
            components.Add(component1);
        }

        private void RemoveComponentByID(ulong componentID)
        {
            foreach (Component component in components)
            {
                if (component.ID == componentID)
                {
                    components.Remove(component);
                    return;
                }
            }
        }

        public void SetActive(bool _active)
        {
            InternalCalls.SetActive(ID, _active);
        }
    }
}

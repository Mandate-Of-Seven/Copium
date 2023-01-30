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
        public GameObject()
        {
            transform = new Transform();
            transform.Initialize(this, 0);
            Instances.gameObjects.Add(this);
        }
        public ulong ID;
        public Transform transform;
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
        public static GameObject FindByID(ulong _ID)
        {
            foreach (GameObject gameObj in Instances.gameObjects)
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
            foreach (GameObject gameObj in Instances.gameObjects)
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

        public T AddComponent<T>() where T : Component, new()
        {
            T component = new T() { gameObject = this };
            component.ID = InternalCalls.AddComponent(ID, typeof(T));
            return component;
        }

        private void RemoveComponentByID(ulong componentID)
        {
            foreach (Component component in Instances.components)
            {
                if (component.ID == componentID)
                {
                    Instances.components.Remove(component);
                    return;
                }
            }
        }

        public void SetActive(bool _active)
        {
            InternalCalls.SetActive(ID, _active);
        }
    }

    public static class Instances
    {
        public static List<GameObject> gameObjects = new List<GameObject>(25000);
        public static List<Component> components = new List<Component>(125000);
    }
}

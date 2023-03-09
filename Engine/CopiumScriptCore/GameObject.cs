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

        public bool HasComponent<T>() where T : Component, new()
        {
            Type componentType = typeof(T);
            return InternalCalls.HasComponent(ID, componentType);
        }

        //COME BACK AND OPTIMISE THIS BECAUSE ITS CREATING A NEW COMPONENT EVERYTIME
        public T GetComponent<T>() where T : Component, new()
        {
            return InternalCalls.GetComponent(ID, typeof(T)) as T;
        }

        public T AddComponent<T>() where T : Component, new()
        {
            T component = new T() { gameObject = this };
            component.ID = InternalCalls.AddComponent(ID, typeof(T));
            return component;
        }

        public void SetActive(bool _active)
        {
            InternalCalls.SetActive(ID, _active);
        }
    }
}

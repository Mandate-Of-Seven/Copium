/*!***************************************************************************************
****
\file			Components.cs
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			28/11/2022

\brief
    Components reflections to C++ components

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CopiumEngine
{
    public enum ButtonState 
    {
        OnHover,
		OnClick,
		OnHeld,
		OnRelease,
		None,
	};

    public class Component
    {
        public ulong ID;
        public GameObject gameObject;
        public Transform transform;
        public void Initialize (GameObject _gameObject, ulong _ID)
        {
            gameObject = _gameObject;
            transform = gameObject.transform;
            ID = _ID;
        }

        public bool enabled
        {
            get
            {
                return InternalCalls.GetComponentEnabled(gameObject.ID, ID);
            }
            set
            {
                InternalCalls.SetComponentEnabled(gameObject.ID,ID,value);
            }
        }

        void SetID(ulong _ID)
            { ID = _ID; }

        public T GetComponent<T>() where T : Component, new()
        {
            return gameObject.GetComponent<T>();
        }
    }

    public class Transform : Component
    {
        public Vector3 position
        {
            get
            {
                InternalCalls.GetTranslation(gameObject.ID, out Vector3 vec3);
                return vec3;
            }
            set
            {
                InternalCalls.SetTranslation(gameObject.ID, ref value);
            }
        }

        public Vector3 localScale
        {
            get
            {
                InternalCalls.GetLocalScale(gameObject.ID, out Vector3 vec3);
                return vec3;
            }
            set
            {
                InternalCalls.SetLocalScale(gameObject.ID, ref value);
            }
        }

        public Vector3 localPosition
        {
            get
            {
                InternalCalls.GetTranslation(gameObject.ID, out Vector3 vec3);
                return vec3;
            }
            set
            {
                InternalCalls.SetTranslation(gameObject.ID, ref value);
            }
        }

        public Vector3 rotation { get; internal set; }
    }

    public class Rigidbody2D : Component
    {
        public Vector2 velocity
        {
            get
            {
                InternalCalls.RigidbodyGetVelocity(gameObject.ID, out Vector2 vec2);
                return vec2;
            }
            set
            {
                InternalCalls.RigidbodySetVelocity(gameObject.ID, ref value);
            }
        }

        public void AddForce(Vector2 force, ForceMode2D forceMode)
        {
            InternalCalls.RigidbodyAddForce(gameObject.ID, ref force);
        }
    }

    public class Camera : Component
    {
    }

    public class SpriteRenderer : Component
    {
        public Color color
        {
            get
            {
                InternalCalls.GetSpriteRendererColor(gameObject.ID, out Color color);
                return color;
            }
            set
            {
                InternalCalls.SetSpriteRendererColor(gameObject.ID, ref value);
            }
        }
    }

    public class Collider2D : Component
    {
    }

    public class Button : Component
    {
        public ButtonState state
        {
            get { return (ButtonState)InternalCalls.GetButtonState(gameObject.ID); }
        }
        public bool interactable
        {
            get;
            set;
        }
    }

    public class Text : Component
    {
        public string text
        {
            get
            {
                InternalCalls.GetTextString(gameObject.ID, ID,out string text);
                return text;
            }
            set
            {
                InternalCalls.SetTextString(gameObject.ID, ID, value);
            }
        }
    }

    public class AudioSource : Component
    {
        public void Play()
        {
            InternalCalls.AudioSourcePlay(gameObject.ID);
        }
    }

    public class Image : Component
    {

    }

    public class SortingGroup : Component
    {

    }
}
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CopiumEngine
{
	public abstract class Component
	{
		public GameObject gameObject { get; internal set; }
        public Transform transform { get; internal set; }

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
            get;
            set;
        }
    }

    public class Collider2D : Component
    {

    }

    namespace UI
    {
        public class Button : Component
        {
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
                get;
                set;
            }
        }
    }
}
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
    }

    public class Rigidbody2D : Component
    {
        public void AddForce(Vector2 force)
        {
            InternalCalls.RigidbodyAddForce(gameObject.ID, ref force);
        }
    }
}
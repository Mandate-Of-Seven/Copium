
using System;

namespace CopiumEngine
{

    public class Transform
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

        public Transform()
        {

        }

        public GameObject gameObject;
    }
}

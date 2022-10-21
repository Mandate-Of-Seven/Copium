
using System.Collections.Generic;

namespace CopiumEngine
{
    public class Transform
    {
        public Vector3 position;
        public Transform()
        {
            position = Vector3.zero;
            transform = this;
        }

        public Transform transform;
        public GameObject gameObject;
    }
    public class GameObject
    {
        GameObject()
        {
            gameObject = this;
            transform = new Transform();
            transform.gameObject = this;
            gameObjects.Add(this);
        }
        public Transform transform;
        public GameObject gameObject;
        public ulong ID;
        private void setID(ulong _ID)
        {
            ID = _ID;
        }
        private static List<GameObject> gameObjects = new List<GameObject>(25000);
        public static GameObject Create()
        {
            return new GameObject();
        }
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
    }
}

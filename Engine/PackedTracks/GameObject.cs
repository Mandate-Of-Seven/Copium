
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
    }
}

using System.Collections;
using System.Collections.Generic;
using System;

namespace CopiumEngine
{
    public class Scene
    {
        List<GameObject> gameObjects = new List<GameObject>();
        List<Component> components = new List<Component>();

        GameObject ReflectGameObject(ulong id)
        {
            foreach (GameObject gameObj in gameObjects)
            {
                if (gameObj.ID == id)
                {
                    return gameObj;
                }
            }
            GameObject gameObject = new GameObject();
            gameObject.ID = id;
            return gameObject;
        }

        public void ReflectComponent(Component component,ulong ComponentID, ulong GameObjectID)
        {
            component.ID = ComponentID;
            component.Initialize(ReflectGameObject(GameObjectID), ComponentID);
            components.Add(component);
        }

        static Scene CurrentScene;
    }
}
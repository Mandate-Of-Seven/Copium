using System.Collections;
using System.Collections.Generic;

namespace CopiumEngine
{
    public class Scene
    {
        List<GameObject> gameObjects = new List<GameObject>();
        List<Component> components = new List<Component>();
        List<CopiumScript> scripts = new List<CopiumScript>();

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
        /*
                Component ReflectComponent(ulong componentID, ulong gameObjectID)
                {
                    Console.WriteLine("finding component");
                    foreach (Component component in Instances.components)
                    {
                        if (component.ID == componentID)
                        {
                            return component;
                        }
                    }
                    Instances.components.Add(this);
                    return this;
                }*/
    }
}
/*!***************************************************************************************
\file			Random.cs
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			1/11/2022

\brief
	This file contains definitions for C# Random

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

using System;

namespace CopiumEngine
{
    public struct Collision2D
    {
        Collision2D(ulong _gameObjectID)
        {
            gameObject = null;
            foreach (GameObject gameObj in Instances.gameObjects)
            {
                if (gameObj.ID == _gameObjectID)
                {
                    gameObject = gameObj;
                    break;
                }
            }
            if (gameObject == null)
            {
                gameObject = new GameObject();
            }
            //rigidbody = gameObject.GetComponent<Rigidbody2D>();
            transform = gameObject.transform;
        }
        public GameObject gameObject;
        public Transform transform;
        //public Rigidbody2D rigidbody;
    }
}

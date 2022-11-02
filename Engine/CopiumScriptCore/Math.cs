/*!***************************************************************************************
\file			Math.cs
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			1/11/2022

\brief
	This file contains definitions for C# Vector2 and Vector3

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

namespace CopiumEngine
{
    public struct Vector2
    {
        public float x, y;
        public static Vector2 Zero => new Vector2(0.0f);

        public Vector2(float scalar)
        {
            x = scalar; y = scalar;
        }
        public Vector2(float _x, float _y)
        {
            x = _x; y = _y;
        }
    }
    
    public struct Vector3
    {
        public float x, y, z;
        public static Vector3 zero => new Vector3(0.0f);
        public Vector3(float scalar)
        {
            x = scalar; y = scalar; z = scalar;
        }
        public Vector3(float _x = 0, float _y = 0, float _z = 0)
        {
            x = _x; y = _y; z = _z;
        }

        public static Vector3 operator +(Vector3 a, Vector3 b)
        {
            return new Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
        }

        public static Vector3 operator *(Vector3 vector, float scalar)
        {
            return new Vector3(vector.x * scalar, vector.y * scalar, vector.z * scalar);
        }
    }
}

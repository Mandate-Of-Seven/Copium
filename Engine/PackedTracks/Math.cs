

namespace CopiumEngine
{
    public struct Vector2
    {
        public double x, y;
        public static Vector2 Zero => new Vector2(0.0f);

        public Vector2(double scalar)
        {
            x = scalar; y = scalar;
        }
        public Vector2(double _x, double _y)
        {
            x = _x; y = _y;
        }
    }
    
    public struct Vector3
    {
        public double x, y, z;
        public static Vector3 zero => new Vector3(0.0f);
        public Vector3(double scalar)
        {
            x = scalar; y = scalar; z = scalar;
        }
        public Vector3(double _x = 0, double _y = 0, double _z = 0)
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

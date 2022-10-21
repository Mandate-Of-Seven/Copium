using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CopiumEngine
{
    public class Vector2
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

    public class Vector3
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
    }
}

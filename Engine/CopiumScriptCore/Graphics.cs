using System.Collections;
using System.Collections.Generic;

namespace CopiumEngine
{
    public struct Color
    {
        public float r, g, b;
        public float a;
        public Color(float _r,float _g, float _b, float _a = 1f)
        {
            r = _r;
            g = _g;
            b = _b;
            a = _a;
        }

        static public Color red = new Color(1f, 0f, 0f, 1f);
    }
}

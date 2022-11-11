using System.Collections;
using System.Collections.Generic;

namespace CopiumEngine
{
    public class Color
    {
        private float r, g, b, a;
        public Color(float r,float g, float b, float a = 1f)
        {

        }

        static public Color red = new Color(1f, 0f, 0f, 1f);
    }
}

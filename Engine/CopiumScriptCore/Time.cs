using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CopiumEngine
{
    public static class Time
    {
        public static float deltaTime{ get { return InternalCalls.GetDeltaTime(); } }
    }
}

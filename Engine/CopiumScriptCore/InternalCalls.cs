using System.Runtime.CompilerServices;

namespace CopiumEngine
{
    public static class InternalCalls
    {
/*        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool HasComponent(ulong ID, Type componentType);*/

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void GetTranslation(ulong ID, out Vector3 translation);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void SetTranslation(ulong ID, ref Vector3 translation);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static ulong FindGameObjByName(string name);

/*        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        internal extern static void RigidBodyAddForce(ulong entityID, ref Vector2 force);
*/
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool GetKey(int keyCode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool GetKeyDown(int keyCode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool GetKeyUp(int keyCode);
    }
}

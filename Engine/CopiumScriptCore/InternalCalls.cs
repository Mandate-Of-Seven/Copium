using System.Runtime.CompilerServices;

namespace CopiumEngine
{
    public static class InternalCalls
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void GetTranslation(ulong ID, out Vector3 translation);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void SetTranslation(ulong ID, ref Vector3 translation);


        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool GetKey(int keyCode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool GetKeyDown(int keyCode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool GetKeyUp(int keyCode);
    }
}

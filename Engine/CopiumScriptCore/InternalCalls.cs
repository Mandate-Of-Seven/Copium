/*!***************************************************************************************
\file			InternalCalls.cs
\project
\author			Zacharie Hong

\par			Course: GAM200
\par			Section:
\date			1/11/2022

\brief
	This file encapsulates the InternalCalls of the Engine

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

using System;
using System.Runtime.CompilerServices;

namespace CopiumEngine
{
    public static class InternalCalls
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool HasComponent(ulong ID, Type componentType);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float GetDeltaTime();

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void GetLocalScale(ulong ID, out Vector3 scale);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void SetLocalScale(ulong ID, ref Vector3 scale);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void GetTranslation(ulong ID, out Vector3 translation);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void SetTranslation(ulong ID, ref Vector3 translation);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void SetActive(ulong ID, bool _active);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool GetActive(ulong ID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static ulong FindGameObjByName(string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void RigidbodyAddForce(ulong ID, ref Vector2 force);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void RigidbodyGetVelocity(ulong entIDityID, out Vector2 velocity);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void RigidbodySetVelocity(ulong ID, ref Vector2 velocity);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool GetKey(int keyCode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool GetKeyDown(int keyCode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool GetKeyUp(int keyCode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void SetUICallback(string funcName);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void QuitGame();
    }
}

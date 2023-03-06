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
        internal extern static float GetDeltaTime();

        #region ANIMATION
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void PauseAllAnimation();
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void PlayAllAnimation();

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void PlayAnimation(ulong ID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void PauseAnimation(ulong ID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void SetAnimatorDelay(ulong ID,float timeDelay);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static float GetAnimatorDelay(ulong ID);
        #endregion

        #region COMPONENT
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool GetComponentEnabled(ulong gameObjID,ulong compId);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void SetComponentEnabled(ulong gameObjID, ulong compId, bool enabled);


        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static ulong GetComponent(ulong gameObjID, Type componentType);
        #endregion

        #region GAMEOBJECT
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool HasComponent(ulong ID, Type componentType);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void SetActive(ulong ID, bool _active);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool GetActive(ulong ID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static ulong CloneGameObject(ulong ID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static ulong InstantiateGameObject();

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void DestroyGameObject(ulong ID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static ulong AddComponent(ulong gameObjID, Type componentType);

        #endregion

        #region TRANSFORM

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void GetLocalScale(ulong ID, out Vector3 scale);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void SetLocalScale(ulong ID, ref Vector3 scale);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void GetTranslation(ulong ID, out Vector3 translation);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void SetTranslation(ulong ID, ref Vector3 translation);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void GetRotation(ulong ID, out Vector3 translation);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void SetRotation(ulong ID, ref Vector3 translation);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void SetParent(ulong newParentID, ulong childID);
        #endregion

        #region RIGIDBODY2D

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void RigidbodyAddForce(ulong ID, ref Vector2 force);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void RigidbodyGetVelocity(ulong entIDityID, out Vector2 velocity);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void RigidbodySetVelocity(ulong ID, ref Vector2 velocity);
        #endregion

        #region SPRITE_RENDERER
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void GetSpriteRendererColor(ulong ID, out Color color);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void SetSpriteRendererColor(ulong ID, ref Color color);

        #endregion

        #region IMAGE
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void GetImageColor(ulong ID, out Color color);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void SetImageColor(ulong ID, ref Color color);

        #endregion

        #region SCENE
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void LoadScene(string sceneName);
        #endregion

        #region AUDIO_SOURCE
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void AudioSourcePlay(ulong ID);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void AudioSourceStop(ulong ID);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void AudioSourceSetVolume(ulong ID, float volume);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern float AudioSourceGetVolume(ulong ID);
        #endregion	

        #region UI
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void SetUIInteractable(bool isInteractable);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool GetUIInteractable();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern char GetButtonState(ulong buttonID);

        #endregion

        #region INPUT
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool GetKey(int keyCode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool GetKeyDown(int keyCode);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern bool GetKeyUp(int keyCode);
        #endregion

        #region TEXT
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void GetTextString(ulong gameObjID, ulong compID,out string str);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void SetTextString(ulong gameObjID, ulong compID,string str);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void GetTextColor(ulong ID, out Color color);

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void SetTextColor(ulong ID, ref Color color);
        #endregion

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void SetUICallback(string funcName);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void QuitGame();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern float GetFPS();
    }
}

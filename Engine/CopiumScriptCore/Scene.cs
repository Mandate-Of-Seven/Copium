using System.Collections;
using System.Collections.Generic;
using System;

namespace CopiumEngine
{
    public static class SceneManager
    {
        public static void LoadScene(string sceneName)
        {
            InternalCalls.LoadScene(sceneName);
        }
    }
}
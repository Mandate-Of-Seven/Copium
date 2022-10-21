
using System.Runtime.CompilerServices;

namespace CopiumEngine
{
    public static class Input
    {
        /*******************************************************************************
        /*!
        *
            \brief
                Checks if a key is held

            \param keyCode
                KeyCode to listen for

            \return
                True if key was held
        */
        /*******************************************************************************/
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern bool GetKey(int keyCode);

        /*******************************************************************************
        /*!
        *
            \brief
                Checks if a key just pressed

            \param keyCode
                KeyCode to listen for

            \return
                True if key was just pressed
        */
        /*******************************************************************************/
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern bool GetKeyDown(int keyCode);

        /*******************************************************************************
        /*!
        *
            \brief
                Checks if a key just released

            \param keyCode
                KeyCode to listen for

            \return
                True if key was just released
        */
        /*******************************************************************************/
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public static extern bool GetKeyUp(int keyCode);
    }
}

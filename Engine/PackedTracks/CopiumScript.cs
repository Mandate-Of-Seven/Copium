/*!***************************************************************************************
****
\file			CopiumEngine.cs
\project
\author			Zacharie Hong
\co-authors

\par			Course: GAM200
\par			Section:
\date			26/09/2022

\brief
    BaseClass for other C# scripts to inherit off

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/


namespace CopiumEngine
{
    public class CopiumScript
    {
        void OnCreate(ulong ID)
        {
            gameObject = GameObject.FindByID(ID);
            transform = gameObject.transform;
        }
        protected Transform transform;
        protected GameObject gameObject;
    }
}
/*!***************************************************************************************
\file			game-nonscript.h
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			27/11/2022

\brief
    This file contains the declarations of the bullet functions of bullets shooting in 
    the game

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#include "Messaging/message-system.h"

namespace Copium
{
    class GameObject;

    class Game : public IReceiver
    {
    public:
        void init();

        void update();

        void exit();

        void handleMessage(MESSAGE_TYPE _mType);

        bool unit_body(GameObject* _gameObj);

        bool combat_bullet_body();
    };
}
/*!***************************************************************************************
\file			game-nonscript.h
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			27/11/2022

\brief
    This file contains the declarations of the bullet functions of bullets shooting in 
    the game.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#include "Messaging/message-system.h"

namespace Copium
{
    class GameObject;

    class Game : public IReceiver
    {
    public:
        /***************************************************************************/
        /*!
        \brief
            Initiliazes part of the game's objects
        */
        /**************************************************************************/
        void init();

        /***************************************************************************/
        /*!
        \brief
            Updates part of the game's logic
        */
        /**************************************************************************/
        void update();

        /***************************************************************************/
        /*!
        \brief
            Exits the game logic
        */
        /**************************************************************************/
        void exit();

        /**************************************************************************/
        /*!
        \brief
            Interface function for MessageSystem to call for IReceivers to handle
            a messageType
        */
        /**************************************************************************/
        void handleMessage(MESSAGE_TYPE _mType);

        /**************************************************************************/
        /*!
        \brief
            Instantiates a bullet for either the enemy or the player
        \param _gameObj
            The refernce to the gameobject
        \return
            Returns true if the logic within the function creates a bullet
        */
        /**************************************************************************/
        bool unit_body(GameObject* _gameObj);
    };
}
/*!***************************************************************************************
****
\file			gamestate-sandbox.h
\project
\author			Matthew Lau
\co-authors

\par			Course: GAM200
\par			Section:
\date			28/07/2022

\brief
	Contains declarations for the Sandbox Scene which inherits from Scene class.
	The Scene contains:
		1. load, init, update, draw, free, unload functions

	Note: load, init, free and unload functions MUST be defined by scene sub-classes

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************
****/
#pragma once
#include "state-manager.h"
#include "scene.h"

class SceneSandbox : public Scene {
public:
	SceneSandbox(std::string& _filepath);
	/*******************************************************************************
	/*!
	*
	\brief
		LOAD function for Sandbox Scene

	\return
		void
	*/
	/*******************************************************************************/
	void loadScene() override;
	/*******************************************************************************
	/*!
	*
	\brief
		INIT function for Sandbox Scene

	\return
		void
	*/
	/*******************************************************************************/
	void initScene() override;
	/*******************************************************************************
	/*!
	*
	\brief
		UPDATE function for Sandbox Scene

	\return
		void
	*/
	/*******************************************************************************/
	void updateScene() override;
	/*******************************************************************************
	/*!
	*
	\brief
		DRAW function for Sandbox Scene

	\return
		void
	*/
	/*******************************************************************************/
	void drawScene() override;
	/*******************************************************************************
	/*!
	*
	\brief
		FREE function for Sandbox Scene

	\return
		void
	*/
	/*******************************************************************************/
	void freeScene() override;
	/*******************************************************************************
	/*!
	*
	\brief
		UNLOAD function for Sandbox Scene

	\return
		void
	*/
	/*******************************************************************************/
	void unloadScene() override;

	/*******************************************************************************
	/*!
	*
	\brief
		Initializes the shader program of the application for the Sandbox Scene

	\return
		void
	*/
	/*******************************************************************************/
	void setupShaderProgram();
};

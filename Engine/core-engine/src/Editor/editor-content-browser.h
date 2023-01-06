/*!***************************************************************************************
\file			editor-content-browser.h
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			01/11/2022

\brief
	Contains function declarations for the editor content browser, where users can view
	and interact with the items in the assets folder.


All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/

#ifndef EDITOR_CONTENT_BROWSER_H
#define EDITOR_CONTENT_BROWSER_H

namespace Copium
{
	// Forward declaration of a directory
	class Directory;

	class EditorContentBrowser
	{
	public:
		/***************************************************************************/
		/*!
		\brief
			Initializes the editor content browser
		*/
		/***************************************************************************/
		void Init();

		/***************************************************************************/
		/*!
		\brief
			Updates the editor content browser
		*/
		/***************************************************************************/
		void Update();

		/***************************************************************************/
		/*!
		\brief
			Exits the editor content browser
		*/
		/***************************************************************************/
		void Exit();

		/***************************************************************************/
		/*!
		\brief
			Selects items within the content browser and respond accordingly based 
			on whether it is a file or directory
		*/
		/***************************************************************************/
		void inputs();

		Directory* get_current_directory() { return currentDirectory; }

	private:
		Directory* currentDirectory;
	};
}

#endif // !EDITOR_CONTENT_BROWSER_H


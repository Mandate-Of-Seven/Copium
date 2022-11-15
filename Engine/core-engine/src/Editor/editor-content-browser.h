/*!***************************************************************************************
\file			editor-content-browser.h
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			01/11/2022

\brief
	Contains function declarations for the editor content browser

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
		void init();

		/***************************************************************************/
		/*!
		\brief
			Updates the editor content browser
		*/
		/***************************************************************************/
		void update();

		/***************************************************************************/
		/*!
		\brief
			Exits the editor content browser
		*/
		/***************************************************************************/
		void exit();

		void inputs();

		Directory* get_current_directory() { return currentDirectory; }

	private:
		Directory* currentDirectory;
	};
}

#endif // !EDITOR_CONTENT_BROWSER_H


/*!***************************************************************************************
\file			Windows/windows-system.h
\project
\author			Sean Ngo

\par			Course: GAM200
\par			Section:
\date			16/09/2022

\brief
	This file contains the declaration functions that are required to create an OpenGL 
	context using GLFW; use GLEW to load OpenGL extensions; initialize OpenGL state; 
	and finally initialize the OpenGL application by calling initalization functions 
	associated with objects participating in the application.

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#ifndef WINDOW_SYSTEMS_H
#define WINDOW_SYSTEMS_H

#include <GL/glew.h> // for access to OpenGL API declarations 
#include <GLFW/glfw3.h>
#include <string>
#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#include "CopiumCore/system-interface.h"

#define MyWindowSystem (*Copium::WindowsSystem::Instance())

namespace Copium
{
	CLASS_SYSTEM(WindowsSystem)
	{
	public:
		
		// Constructors
		/***************************************************************************/
		/*!
		\brief
			Setup OpenGL context and the window
		*/
		/***************************************************************************/
		void init();

		/***************************************************************************/
		/*!
		\brief
			Setup OpenGL context and the window
		\param _width
			The width of the window to set to
		\param _height
			The height of the window to set to
		\param _title
			The title of the window
		*/
		/***************************************************************************/
		void init_system(int _width = 1600, int _height = 900, std::string _title = "Copium");

		/***************************************************************************/
		/*!
		\brief
			Checks for poll events if the window is not closed
		*/
		/***************************************************************************/
		void update();

		/***************************************************************************/
		/*!
		\brief
			Displays the window titles and swap the buffers of the window
		*/
		/***************************************************************************/
		void draw();

		/***************************************************************************/
		/*!
		\brief
			Terminates GLFW
		*/
		/***************************************************************************/
		void exit();

		// Frames and Delta Time
		/***************************************************************************/
		/*!
		\brief
			Updates the time of the system, it uses the GLFW's time functions to 
			compute the interval in seconds between each frame and the frames per
			second every "fps calculated interval" seconds
		\param _fpsInterval
			The interval (in seconds) at which fps is to be calculated
		*/
		/***************************************************************************/
		void update_time(double _fpsInterval = 1.0);

		// Window Modes
		/***************************************************************************/
		/*!
		\brief
			Toggles the window mode of the engine to either be in fullscreen or 
			maximized. Note that this function only takes in the primary window of
			the system, and does not apply fullscreen on the monitor that it is
			currently on if said monitor is a secondary monitor
		\param _enable
			If true, enable fullscreen, else maxmize the window
		\param _width
			The width of the window
		\param _height
			The height of the window
		*/
		/***************************************************************************/
		void Fullscreen(bool _enable, int _width, int _height);

		// Callbacks
		/***************************************************************************/
		/*!
		\brief
			Define the error callback of the windows
		\param _error
			The GLFW error code
		\param _description
			The description of the error
		*/
		/***************************************************************************/
		static void error_callback(int _error, char const* _description);

		/***************************************************************************/
		/*!
		\brief
			This function is called when the window is resized and receives the new
			size of the windows in pixels
		\param _window
			The handle to window that is being resized
		\param _width
			The width to change to
		\param _height
			The height to change to
		*/
		/***************************************************************************/
		static void framebuffer_size_callback(GLFWwindow* _window, int _width, int _height);

		/***************************************************************************/
		/*!
		\brief
			This function is called when the window is loses or gains focus
		\param _window
			The handle to window that is being focused or unfocused
		\param _focused
			The current focus of the window
		*/
		/***************************************************************************/
		static void window_focus_callback(GLFWwindow* _window, int _focused);

		/***************************************************************************/
		/*!
		\brief
			This function is called when an item is dropped into the window
		\param _window
			The handle to window that is being dropped onto
		\param _dropFun
			The current focus of the window
		*/
		/***************************************************************************/
		static void window_drop_callback(GLFWwindow* _window, int _pathCount, const char* _paths[]);

		// Accessing Properties

		void set_window_dimensions(int _width, int _height) { windowWidth = _width; windowHeight = _height; }

		std::string get_title() { return title; }

		int get_window_width() { return windowWidth; }
		void set_width(int _width) { windowWidth = _width; }

		int get_window_height() { return windowHeight; }
		void set_height(int _height) { windowHeight = _height; }

		const bool& get_window_focused() const { return windowFocused; }

		GLFWwindow* get_window() { return window; }

		static bool load_config(std::string& _filename, GLint& _w, GLint& _h);

	private:
		/* Properties of a Window *******************************************************/
		std::string title;

		static int windowWidth;
		static int windowHeight;
		static bool windowFocused;
		static bool windowFullscreen;

		GLFWwindow* window = nullptr;
	};
}

#endif /* WINDOW_SYSTEMS_H */

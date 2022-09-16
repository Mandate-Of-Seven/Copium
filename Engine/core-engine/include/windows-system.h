/*!***************************************************************************************
\file			windows-system.h
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

namespace Copium
{
	class WindowsSystem // Inherits from ISystem
	{
	public:
		// Setup OpenGL context
		void init(int _width, int _height, std::string _title);

		void update();

		void draw();

		void exit();

		// Callbacks
		static void error_callback(int _error, char const * _description);

		static void framebuffer_size_callback(GLFWwindow * _window, int _width, int _height);

		// Get and Set Functions

		void set_window_dimensions(int _width, int _height) { screenWidth = _width; screenHeight = _height; }

		std::string get_title() { return title; }

		static int get_window_width() { return screenWidth; }

		static int get_window_height() { return screenHeight; }

		static GLFWwindow * get_window() { return window; }

	private:
		/* Properties of a Window *******************************************************/
		std::string title;

		static int screenWidth;
		static int screenHeight;

		static GLFWwindow * window;
	};

	static WindowsSystem windowsSystem;

	struct GLHelper
		/*! GLHelper structure to encapsulate initialization stuff ...
		*/
	{
		static void update_time(double fpsCalcInt = 1.0); // (Should be moved to core engine)
		
		static GLdouble fps;
		static GLdouble delta_time; // time taken to complete most recent game loop
	};
}

#endif /* WINDOW_SYSTEMS_H */

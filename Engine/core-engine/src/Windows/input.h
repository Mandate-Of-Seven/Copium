/*!***************************************************************************************
\file			input.h
\project
\author			Shawn Tanary

\par			Course: GAM200
\par			Section:
\date			16/09/2022

\brief
	Contains functions to check if certain mouse or keyboard inputs are detected

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
*****************************************************************************************/

#ifndef INPUT_H
#define INPUT_H

#include <utility> 

//Checks for mouse or keyboard inputs
class Input
{
	// Forward declaration for type definition
	typedef struct GLFWwindow GLFWwindow;

	public:
		short* keys =nullptr;//contains the current state of all keys
		short* mouseButtons = nullptr;//contains the current state of both mouse buttons
		double mouseScrollOffset;

		virtual ~Input() = default;

		/***************************************************************************/
		/*!
		\brief
			Initialises all the needed variables
		*/
		/**************************************************************************/
		virtual void init() = 0;

		/***************************************************************************/
		/*!
		\brief
			Deletes the input system instance, CALLED AT END OF PROGRAM
		*/
		/**************************************************************************/
		static void destroy()
		{
			delete[] inputInstance->keys;
			delete[] inputInstance->mouseButtons;
			delete inputInstance;
		}

		/***************************************************************************/
		/*!
		\brief
			Gets a reference of the input instance
		\return
			The input instance
		*/
		/**************************************************************************/
		static Input* get_input_instance()
		{
			return inputInstance;
		}

		/***************************************************************************/
		/*!
		\brief
			Checks if the given key is being pressed
		\param keycode
			the key to check for
		\return
			true if the key is pressed or false if not
		*/
		/**************************************************************************/
		static bool is_key_pressed(int keycode) { return inputInstance->is_key_pressed_impl(keycode); }

		/***************************************************************************/
		/*!
		\brief
			Checks if the given key is being held
		\param keycode
			the key to check for
		\return
			true if the key is held or false if not
		*/
		/**************************************************************************/
		static bool is_key_held(int keycode) { return inputInstance->is_key_held_impl(keycode); }


		/***************************************************************************/
		/*!
		\brief
			Checks if the given mouse button is being pressed
		\param button
			the mouse button to check for
		\return
			true if the key is pressed or false if not
		*/
		/**************************************************************************/
		static bool is_mousebutton_pressed(int button) { return inputInstance->is_mousebutton_pressed_impl(button); }

		/***************************************************************************/
		/*!
		\brief
			Returns the position of the mouse cursor based on the top left of the game window
		\return
			the mouse position
		*/
		/**************************************************************************/
		static std::pair<float,float> get_mouse_position() { return inputInstance->get_mouseposition_impl(); }

		/***************************************************************************/
		/*!
		\brief
			Returns the x coordinate of the mouse cursor
		\return
			x coordinate of the mouse cursor
		*/
		/**************************************************************************/
		static bool get_mouseX() { return inputInstance->get_mouseX_impl(); }
		/***************************************************************************/
		/*!
		\brief
			Returns the y coordinate of the mouse cursor
		\return
			y coordinate of the mouse cursor
		*/
		/**************************************************************************/
		static bool get_mouseY() { return inputInstance->get_mouseY_impl(); }

		/***************************************************************************/
		/*!
		\brief
			Returns the y offset of mouse scroll
		\return
			y offset of the mouse scroll
		*/
		/**************************************************************************/
		static double get_mousescroll() { return inputInstance->get_mousescroll_impl(); }

		/***************************************************************************/
		/*!
		\brief
			This function is called when keyboard buttons are pressed.

		 Param window
			Handle to window that is receiving event

		 Param key
			the keyboard key that was pressed or released

		 Param scancode
			Platform-specific scancode of the key

		 Param action
			GLFW_PRESS, GLFW_REPEAT or GLFW_RELEASE
			action will be GLFW_KEY_UNKNOWN if GLFW lacks a key token for it,
			for example E-mail and Play keys.

		 Param mods
			bit-field describing which modifier keys (shift, alt, control)
			were held down
		*/
		/**************************************************************************/
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

		/***************************************************************************/
		/*!
		\brief
			This function is called when mouse buttons are pressed.

		 Param window
			Handle to window that is receiving event

		 Param button
			the mouse button that was pressed or released
			GLFW_MOUSE_BUTTON_LEFT and GLFW_MOUSE_BUTTON_RIGHT specifying left and right

		 Param action
			action is either GLFW_PRESS or GLFW_RELEASE

		 Param mods
			bit-field describing which modifier keys (shift, alt, control)
			were held down
		*/
		/**************************************************************************/
		static void mousebutton_callback(GLFWwindow* window, int button, int action, int mods);


		/***************************************************************************/
		/*!
		\brief
			This function is called when the user scrolls, whether with a mouse wheel or
			touchpad gesture. Although the function receives 2D scroll offsets, a simple
			mouse scroll wheel, being vertical, provides offsets only along the Y-axis.

		 Param window
			Handle to window that is receiving event

		 Param xOffset
			Scroll offset along X-axis

		 Param yOffset
			Scroll offset along Y-axis
		*/
		static void mousescroll_callback(GLFWwindow* window, double xOffset, double yOffset);

		/***************************************************************************/
		/*!
		\brief
			This functions receives the cursor position, measured in screen coordinates 
			but relative to the top-left corner of the window client area.

		 Param window
			Handle to window that is receiving event

		 Param xPos
			new cursor x-coordinate, relative to the left edge of the client area

		 Param yPos
			new cursor y-coordinate, relative to the top edge of the client area
		*/
		static void mousepos_callback(GLFWwindow* window, double xPos, double yPos);


	protected:
		/***************************************************************************/
		/*!
		\brief
			The implementaion of is_key_pressed - Must be implemented by derived class
		/**************************************************************************/
		virtual bool is_key_pressed_impl(int keycode) = 0;

		/***************************************************************************/
		/*!
		\brief
			The implementaion of is_key_held - Must be implemented by derived class
		/**************************************************************************/
		virtual bool is_key_held_impl(int keycode) = 0;


		/***************************************************************************/
		/*!
		\brief
			The implementaion of is_mousebutton_pressed - Must be implemented
													    by derived class
		/**************************************************************************/
		virtual bool is_mousebutton_pressed_impl(int button) = 0;

		/***************************************************************************/
		/*!
		\brief
			The implementaion of get_mouse_position - Must be implemented
													by derived class
		/**************************************************************************/
		virtual std::pair<float, float> get_mouseposition_impl() = 0;

		/***************************************************************************/
		/*!
		\brief
			The implementaion of get_mouseX - Must be implemented
											 by derived class
		/**************************************************************************/
		virtual float get_mouseX_impl() = 0;

		/***************************************************************************/
		/*!
		\brief
			The implementaion of get_mouseY - Must be implemented
											 by derived class
		/**************************************************************************/
		virtual float get_mouseY_impl() = 0;

		/***************************************************************************/
		/*!
		\brief
			The implementaion of get_mousescroll_impl - Must be implemented
														by derived class
		/**************************************************************************/
		virtual double get_mousescroll_impl() = 0;
	private:
		static Input* inputInstance;
};
#endif // !INPUT_H

/* !
@file    glapp.h
@author  pghali@digipen.edu
@date    10/11/2016

This file contains the declaration of namespace GLApp that encapsulates the
functionality required to implement an OpenGL application including 
compiling, linking, and validating shader programs
setting up geometry and index buffers, 
configuring VAO to present the buffered geometry and index data to
vertex shaders,
configuring textures (in later labs),
configuring cameras (in later labs), 
and transformations (in later labs).
*//*__________________________________________________________________________*/

/*                                                                      guard
----------------------------------------------------------------------------- */
#ifndef GLAPP_H
#define GLAPP_H

/*                                                                   includes
----------------------------------------------------------------------------- */
#include <glslshader.h>

struct GLApp {
  static void init();
  static void update();
  static void draw();
  static void cleanup();
};

// Encapsulates state required to render a geometrical model
struct GLModel
{
	GLenum		primitive_type; // Which OpenGL primitive to be rendered?
	GLSLShader	shdr_pgm;		// Which shader program?
	GLuint		vaoid;			// Handle to VAO
	GLuint		idx_elem_cnt;	// How many elements of primitive of type
								// primitive_type are to be rendered

	// Member functions defined in glapp.cpp
	void setup_vao();
	void setup_shdrpgm();
	void draw();
};

// Data member to represen geometric model to be rendered
// C++ requires this object to have a definition in glapp.cpp!!!
static GLModel mdl;

#endif /* GLAPP_H */

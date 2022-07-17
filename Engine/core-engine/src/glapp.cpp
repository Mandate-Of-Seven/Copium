/*!
@file    glapp.cpp
@author  pghali@digipen.edu
@date    10/11/2016

This file implements functionality useful and necessary to build OpenGL
applications including use of external APIs such as GLFW to create a
window and start up an OpenGL context and to extract function pointers
to OpenGL implementations. 

*//*__________________________________________________________________________*/

/*                                                                   includes
----------------------------------------------------------------------------- */
#include <glapp.h>
#include <glhelper.h>
#include <array>
#include <iostream>
#include <sstream>
#include <iomanip>

/*                                                   objects with file scope
----------------------------------------------------------------------------- */

void GLApp::init() 
{
	// Part 1 : Clear the colorbuffer with RGBA value in glClearColor ...
	glClearColor(0.f, 1.f, 0.f, 1.f);

	// Part 2 : Use the entire window as viewport ...
	glViewport(0, 0, GLHelper::width, GLHelper::height);

	// Part 3 : Initialise VAO and create shader program
	mdl.setup_vao();
	mdl.setup_shdrpgm();
}

void GLApp::update() 
{
	
	static GLfloat color;

	static bool reset;

	GLfloat timer = 0.f;
	timer += (GLfloat)GLHelper::delta_time * 0.5f;

	if (color >= 1.f || color <= 0.f)
		reset = !reset;

	color = (reset) ? color - timer : color + timer;

	glClearColor(color, 0.2f, 0.4f, 1.f);
}

void GLApp::draw() {
	// Clear back buffer 
	glClear(GL_COLOR_BUFFER_BIT);

	// Now render rectangular model from NDC coordinates to viewport
	mdl.draw();
	
	// Printing to Windows Title Bar
	std::stringstream sstr;
	sstr << std::fixed << std::setprecision(2) << GLHelper::title 
		<< " | " "Sean Ngo" << " | " << GLHelper::fps;
	glfwSetWindowTitle(GLHelper::ptr_window, sstr.str().c_str());
}

void GLApp::cleanup() {
  // empty for now
}

void GLModel::setup_vao()
{
	// We'll define a rectangle in normalized device coordinates (NDC)
	// coordinates that has one-fourth the area of the window.
	// The NDC coordinates for a window range from [-1, 1] along both
	// both the X- and Y-axes. Therefore, the rectangle's (x, y) position
	// coordinates are in range [-0.5, 0.5]
	// We're using NDC coordinates, because we don't want to specify
	// a "model-to-world-to-view-to-clip" transform to the vertex shader.

	// Define vertex position and color attributes
	std::array<glm::vec2, 4> pos_vtx{
		glm::vec2(0.5f, -0.5f), glm::vec2(0.5f, 0.5f),
		glm::vec2(-0.5f, 0.5f), glm::vec2(-0.5f, -0.5f)
	};

	std::array < glm::vec3, 4> clr_vtx{
		glm::vec3(1.f, 0.f, 0.f), glm::vec3(0.f, 1.f, 0.f),
		glm::vec3(0.f, 0.f, 1.f), glm::vec3(1.f, 1.f, 1.f)
	};

	// Transfer vertex position and color attributes to VBO

	// A buffer object can be created with a call to glCreateBuffers:
	GLuint vbo_hdl;	// Vertex buffer object used to hold primitives
	glCreateBuffers(1, &vbo_hdl);

	// Client-side data is copied to the buffer object's data store using glNamedBufferSubData:
	glNamedBufferStorage(vbo_hdl, 
		sizeof(glm::vec2) * pos_vtx.size() + sizeof(glm::vec3) * clr_vtx.size(),
		nullptr, GL_DYNAMIC_STORAGE_BIT);
	glNamedBufferSubData(vbo_hdl, 0,
		sizeof(glm::vec2) * pos_vtx.size(), pos_vtx.data());
	glNamedBufferSubData(vbo_hdl, sizeof(glm::vec2) * pos_vtx.size(), 
		sizeof(glm::vec3) * clr_vtx.size(), clr_vtx.data());

	// Encapsulate information about contents of VBO and VBO handle
	// to another object called VAO
	glCreateVertexArrays(1, &vaoid);

	// For vertex position array, we use vertex attribute index 8
	// and for vertex buffer we use binding(making the object become "alive") point 3
	glEnableVertexArrayAttrib(vaoid, 8);
	glVertexArrayVertexBuffer(vaoid, 3, vbo_hdl, 0, sizeof(glm::vec2));
	glVertexArrayAttribFormat(vaoid, 8, 2, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vaoid, 8, 3);

	// For vertex color array, we use vertex attribute index 9
	// and vertex buffer binding point 4

	glEnableVertexArrayAttrib(vaoid, 9);
	glVertexArrayVertexBuffer(vaoid, 4, vbo_hdl,
		sizeof(glm::vec2) * pos_vtx.size(), sizeof(glm::vec3));
	glVertexArrayAttribFormat(vaoid, 9, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexArrayAttribBinding(vaoid, 9, 4);

	primitive_type = GL_TRIANGLES;

	// Represents indices of vertices that will define 2 triangles with
	// counterclockwise winding
	std::array<GLushort, 6> idx_vtx{
		0, 1, 2,	// 1st triangle with counterclockwise winding is specified by
					// vertices in VBOs with indices 0, 1, 2
		2, 3, 0		// 2nd triangle with counterclockwise winding
	};

	idx_elem_cnt = idx_vtx.size();

	GLuint ebo_hdl;
	glCreateBuffers(1, &ebo_hdl);
	glNamedBufferStorage(ebo_hdl,
		sizeof(GLushort) * idx_elem_cnt,
		reinterpret_cast<GLvoid *>(idx_vtx.data()),
		GL_DYNAMIC_STORAGE_BIT);
	glVertexArrayElementBuffer(vaoid, ebo_hdl);
	glBindVertexArray(0);
}

void GLModel::setup_shdrpgm()
{
	std::vector<std::pair<GLenum, std::string>> shdr_files;
	shdr_files.emplace_back(std::make_pair(GL_VERTEX_SHADER, "../shaders/my-tutorial-1.vert"));
	shdr_files.emplace_back(std::make_pair(GL_FRAGMENT_SHADER, "../shaders/my-tutorial-1.frag"));
	shdr_pgm.CompileLinkValidate(shdr_files);

	if (GL_FALSE == shdr_pgm.IsLinked())
	{
		std::cout << "Unable to compile/link/validate shader programs\n";
		std::cout << shdr_pgm.GetLog() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

void GLModel::draw()
{
	// There are many shader programs initialized - here we are saying 
	// which specific shader program should be used to render geometry
	shdr_pgm.Use();

	// There are many models, each with their own initialized VAO object
	// here, we are saying which VAO's state should be used to set up pipe
	glBindVertexArray(vaoid);

	// Here, we are saying what primitive is to be rendered and how many
	// such primitives exist.
	// The graphics driver knows where to get the indices because the VAO
	// containing this state information has been made current ...
	glDrawElements(primitive_type, idx_elem_cnt, GL_UNSIGNED_SHORT, NULL);

	// After completing the rendering, we tell the driver that VAO 
	// vaoid and current shader	program are no longer current
	glBindVertexArray(0);
	shdr_pgm.UnUse();

}

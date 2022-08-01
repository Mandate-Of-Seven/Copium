/*!***************************************************************************************
****
\file			gamestate-sandbox.cpp
\project
\author			Matthew Lau
\co-authors

\par			Course: GAM200
\par			Section:
\date			28/07/2022

\brief
	Contains definitions for the Sandbox Scene which inherits from Scene class.
	The Scene contains:
		1. load, init, update, draw, free, unload functions

	Note: load, init, free and unload functions MUST be defined by scene sub-classes

All content © 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************
****/
#include "gamestate-sandbox.h"

<<<<<<< Updated upstream
#include <glhelper.h>
#include <renderer.h>
#include <input.h>

// Global variables
GLfloat movement_x = 0.f, movement_y = 0.f;


SceneSandbox::SceneSandbox(std::string& _filepath) : Scene(_filepath) {
=======
SceneSandbox::SceneSandbox(std::string& _filepath) : Scene(_filepath) 
{
>>>>>>> Stashed changes

}

void SceneSandbox::load_scene() 
{
	std::cout << "load sandbox" << std::endl;
}
void SceneSandbox::init_scene() 
{
	std::cout << "init sandbox" << std::endl;

	glClearColor(1.f, 1.f, 1.f, 1.f);

	glViewport(0, 0, GLHelper::width, GLHelper::height);

	s_Data.shaderProgram.Use();
	GLuint loc = glGetUniformLocation(s_Data.shaderProgram.GetHandle(), "uTextures");
	GLint samplers[maxTextures];

	for (GLuint i = 0; i < maxTextures; i++)
		samplers[i] = i;

	glUniform1iv(loc, maxTextures, samplers);

	// Init Renderer
	Renderer::init();
}
<<<<<<< Updated upstream

void SceneSandbox::updateScene() {
=======
void SceneSandbox::update_scene() 
{
>>>>>>> Stashed changes
	std::cout << "update sandbox" << std::endl;

	if (Input::isKeyPressed(GLFW_KEY_A))
		movement_x -= GLHelper::delta_time;
	else if (Input::isKeyPressed(GLFW_KEY_D))
		movement_x += GLHelper::delta_time;

	if (Input::isKeyPressed(GLFW_KEY_W))
		movement_y += GLHelper::delta_time;
	else if (Input::isKeyPressed(GLFW_KEY_S))
		movement_y -= GLHelper::delta_time;

	glClearColor(1.f, 1.f, 1.f, 1.f);
}
<<<<<<< Updated upstream

void SceneSandbox::drawScene() {
=======
void SceneSandbox::draw_scene() 
{
>>>>>>> Stashed changes
	std::cout << "draw sandbox" << std::endl;

	glClear(GL_COLOR_BUFFER_BIT);

	s_Data.shaderProgram.Use();

	Renderer::reset_stats();

	Renderer::begin_batch();

	for (GLfloat y = -10.f; y < 10.f; y += 0.25f)
	{
		for (GLfloat x = -10.f; x < 10.f; x += 0.25f)
		{
			glm::vec4 color = { (x + 10) / 20.f, 0.2f, (y + 10) / 20.f, 1.f };
			Renderer::draw_quad({ x + movement_x,y + movement_y}, { 0.1f, 0.1f }, color);
		}
	}

	Renderer::end_batch();

	Renderer::flush();

	s_Data.shaderProgram.UnUse();
}
void SceneSandbox::free_scene() 
{
	std::cout << "free sandbox" << std::endl;
}
void SceneSandbox::unload_scene() 
{
	std::cout << "unload sandbox" << std::endl;

	Renderer::shutdown();
}

void SceneSandbox::setupShaderProgram()
{
	std::vector<std::pair<GLenum, std::string>> shdr_files;
	shdr_files.emplace_back(std::make_pair(GL_VERTEX_SHADER, "../core-engine/Assets/shaders/shader-glsl.vert"));
	shdr_files.emplace_back(std::make_pair(GL_FRAGMENT_SHADER, "../core-engine/Assets/shaders/shader-glsl.frag"));
	s_Data.shaderProgram.CompileLinkValidate(shdr_files);

	if (GL_FALSE == s_Data.shaderProgram.IsLinked())
	{
		std::cout << "Unable to compile/link/validate shader programs\n";
		std::cout << s_Data.shaderProgram.GetLog() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

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
#include <pch.h>
#include "gamestate-sandbox.h"

#include <glhelper.h>
#include <renderer.h>
#include <input.h>
#include <serializer.h>
#include <rapidjson/prettywriter.h>

// Global variables
GLfloat movement_x = 0.f, movement_y = 0.f;
Vector3 v1(1, 2, 3), v2(5,1,10);
Copium::Math::Vec2 vv(1, 10);

GameObject g1(v2, v1, v1);
GameObject g2(v1, v1, v2);

Serializer ss;


SceneSandbox::SceneSandbox(std::string& _filepath) : Scene(_filepath) 
{

}

void SceneSandbox::load_scene() 
{
	std::cout << "load sandbox" << std::endl;
	/*
	deserialize_scene("sandbox.dat", this);
	std::cout << "Number of Game Objects deserialised: " << this->get_gameobjcount() << std::endl;
	for (size_t i{ 0 }; i < this->get_gameobjcount(); ++i)
	{
		GameObject* tmp = this->get_gameobjectvector()[i];
		std::cout << "Game Object ID: " << tmp->get_id() << std::endl;
		std::cout << "Game Object Name: " << tmp->get_name() << std::endl;
		Transform t(tmp->Trans());
		std::cout << "Position: " << t.Position()[0] << ',' << t.Position()[1] << ',' << t.Position()[2] << std::endl;
	}*/



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

	//Serialization Testingf
	this->get_gameobjectvector().push_back(new GameObject(v2,v2,v2));
	this->get_gameobjectvector().push_back(new GameObject(v1,v1,v1));
	this->get_gameobjectvector().push_back(new GameObject(v2, v2, v1));

	GameObject* dad = get_gameobjectvector()[0];
	GameObject* kid = get_gameobjectvector()[2];
	dad->attach_child(kid);
	GameObjectID g{ 5 };
	kid->set_id(g);

	dad->set_name("Rex");

	Copium::Math::Vec2 b1(1,1);
	std::cout << b1/2;

}
void SceneSandbox::update_scene() 
{

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
void SceneSandbox::draw_scene() 
{
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
	//for(size_t i{ 0 }; i < this->get_gameobjcount(); ++i)
	//{
	//}
}
void SceneSandbox::unload_scene() 
{
	std::cout << "unload sandbox" << std::endl;
	std::cout << "does file exist: " << does_file_exist("sandbox.dat") << std::endl;
	serialize_scene("sandbox.dat", this);
	//JSON
	std::ofstream os("Data\\sandbox.json", std::ios::out);
	if (!os)
		std::cout << "file not open\n";
	Copium::Math::Vec2 tester(3, 4);
	ss.serialize(os, tester);
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

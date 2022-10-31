/*!***************************************************************************************
\file			glslshader.h
\project
\author         Prasanna Ghali
\co-author		Abdul Hadi

\par			Course: GAM200
\par			Section:
\date			28/09/2022

\brief
    This file contains the declaration of class GLSLShader that encapsulates the
    functionality required to load shader source; compile shader source; link
    shader objects into a program object, validate program object; log msesages
    from compiling linking, and validation steps; install program object in
    appropriate shaders; and pass uniform variables from the client to the 
    program object.

All content � 2022 DigiPen Institute of Technology Singapore. All rights reserved.
******************************************************************************************/

#ifndef GLSLSHADER_H
#define GLSLSHADER_H

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <map>

#define GL_FALSE 0
#define GL_TRUE 1

// Forward declaration of type definitions
typedef char GLchar;
typedef unsigned char GLboolean;
typedef int GLint;
typedef unsigned int GLenum;
typedef unsigned int GLuint;
typedef float GLfloat;

namespace Copium
{
    class GLSLShader
    {
    public:

        
        /*******************************************************************************
        /*!
        \brief
            Default constructor that is required to initialize GLSLShader object
        */
        /*******************************************************************************/
        GLSLShader() : pgm_handle(0), is_linked(GL_FALSE)
        {
        }

        /*******************************************************************************
        /*!
        \brief
            This function not only compiles individual shader sources but links
            multiple shader objects to create an exectuable shader program.
            For each shader source, the function requires the full path to the file 
            (containing shader source) and the type of shader program (vertex,
            fragment, geometry, a tessellation type). This information is combined
            as an std::pair object and the multiple pairs are supplied in a
            std::vector object.
            For each shader file, the function implements the six steps described in T
            CompileShaderFromFile(). After the shader objects are created, a call to
            Link() will create a shader executable program. This is followed by a call
            to Validate() ensuring the program can execute in the current OpenGL state
        \return
            True if the function compiles successfully
        */
        /*******************************************************************************/
        GLboolean CompileLinkValidate(std::vector<std::pair<GLenum, std::string>>);

        
        /*******************************************************************************
        /*!
        \brief
            This function creates a shader program object and load shader source code 
            from the file parameter. It also compiles the shader source and check
            compilation status.
        \param shader_type
            The type of shader to compile
        \param file_name
            The name of the file to load from
        \return
            True if the function compiles successfully
        */
        /*******************************************************************************/
        GLboolean CompileShaderFromFile(GLenum shader_type, std::string const& file_name);

        /*******************************************************************************
        /*!
        \brief
            This function creates a shader program object and load shader source code
            from the string parameter. It also compiles the shader source and check
            compilation status.
        \param shader_type
            The type of shader to compile
        \param shader_src
            The name of the string to load from
        \return
            True if the function compiles successfully
        */
        /*******************************************************************************/
        GLboolean CompileShaderFromString(GLenum shader_type, std::string const& shader_src);

        /*******************************************************************************
        /*!
        \brief
            Link shader objects attached to handle pgm_handle. This member function
            will also verify the status of the link operation (successful or not?).
            If the shader objects did not link into a program object, then the
            member function must retrieve and write the program object's information
            log to data member log_string. 
        \return
            True if the shader links successfully
        */
        /*******************************************************************************/
        GLboolean Link();

        /*******************************************************************************
        /*!
        \brief
            Install the shader program object whose handle is encapsulated by member 
            pgm_handle
        */
        /*******************************************************************************/
        void Use();

        /*******************************************************************************
        /*!
        \brief
            De-install previously installed shader program object using Use().
            More correctly, after the execution of this member function, the
            current rendering state is referring to an invalid program object.
        */
        /*******************************************************************************/
        void UnUse();

        /*******************************************************************************
        /*!
        \brief
            check whether the executable shader program object can execute given the
            current OpenGL state ...
        \return
            The function returns true if validatation succeeded
        */
        /*******************************************************************************/
        GLboolean Validate();

        /*******************************************************************************
        /*!
        \brief
            Returns the handle to the shader program object
        \return
            The unsigned integer
        */
        /*******************************************************************************/
        GLuint GetHandle() const;

        /*******************************************************************************
        /*!
        \brief
            Checks if the different object code are linked into a shader program
        \return
            True if it is linked
        */
        /*******************************************************************************/
        GLboolean IsLinked() const;

        /*******************************************************************************
        /*!
        \brief
            This function returns the logged information from the GLSL compiler and 
            linker and validation information obtained after calling Validate() ...
        \return
            The string log
        */
        /*******************************************************************************/
        std::string GetLog() const;

        /*******************************************************************************
        /*!
        \brief
            Use an OpenGL API function to dynamically associate a generic vertex 
            attribute index with a named in attribute variable.
        \param index
            The index to bind to
        \param name
            The name of the binding attribute
        */
        /*******************************************************************************/
        void BindAttribLocation(GLuint index, GLchar const* name);

        /*******************************************************************************
        /*!
        \brief
            Use an OpenGL API function to dynamically associate a fragment shader 
            index location that a user-defined out variable will write to.
        \param color_number
            The color number to bind to
        \param name
            The name of the binding attribute
        */
        /*******************************************************************************/
        void BindFragDataLocation(GLuint color_number, GLchar const* name);

        /*******************************************************************************
        /*!
        \brief
            This function deletes a program object
        */
        /*******************************************************************************/
        void DeleteShaderProgram();

        // A family of functions to specify values of uniform
        // variables of different types for the current program object
        void SetUniform(GLchar const* name, GLboolean val);
        void SetUniform(GLchar const* name, GLint val);
        void SetUniform(GLchar const* name, GLfloat val);
        void SetUniform(GLchar const* name, GLfloat x, GLfloat y);
        void SetUniform(GLchar const* name, GLfloat x, GLfloat y, GLfloat z);
        void SetUniform(GLchar const* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
        void SetUniform(GLchar const* name, glm::vec2 const& val);
        void SetUniform(GLchar const* name, glm::vec3 const& val);
        void SetUniform(GLchar const* name, glm::vec4 const& val);
        void SetUniform(GLchar const* name, glm::mat3 const& val);
        void SetUniform(GLchar const* name, glm::mat4 const& val);

        /*******************************************************************************
        /*!
        \brief
            Display the list of active vertex attributes used by vertex shader
        */
        /*******************************************************************************/
        void PrintActiveAttribs() const;

        /*******************************************************************************
        /*!
        \brief
            Display the list of active uniform variables
        */
        /*******************************************************************************/
        void PrintActiveUniforms() const;

    private:
        GLuint pgm_handle = 0;  // handle to linked shader program object
        GLboolean is_linked = GL_FALSE; // has the program successfully linked?
        std::string log_string; // log for OpenGL compiler and linker messages

    private:
        // use OpenGL API to return the location of an uniform variable with
        // name "name" using program handle encapsulated by object of this class type
        GLint GetUniformLocation(GLchar const* name);

        // return true if file (given in relative path) exists, false otherwise
        GLboolean FileExists(std::string const& file_name);
    };
}

#endif /* GLSLSHADER_H */

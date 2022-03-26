#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace our {

    class ShaderProgram {

    private:
        //Shader Program Handle
        GLuint program;

    public:
        void create();
        void destroy();

        ShaderProgram(){ program = 0; }
        ~ShaderProgram(){ destroy(); }

        bool attach(const std::string &filename, GLenum type) const;

        bool link() const;

        void use() { 
            //DONE: call opengl to use the program identified by this->program
            // void glUseProgram(GLuint program)
            // program: ID/Handle of the program object.
            glUseProgram(this->program); 
        }

        GLuint getUniformLocation(const std::string &name) {
            //DONE: call opengl to get the uniform location for the uniform defined by name from this->program
            // GLint glGetUniformLocation(GLuint program, const GLchar * name)
            // program: Program object to be get location from.
            // name: String of the name of the uniform variable we want to get its location.
            return  glGetUniformLocation(this->program, name.c_str());
        }

        void set(const std::string &uniform, GLfloat value) {
            //DONE: call opengl to set the value to the uniform defined by name
            // void glUniform1f(GLint location, GLfloat v0)
            // location: Location of the uniform variable to be set.
            // v0: New value for the specified uniform variable to be set.
            glUniform1f(getUniformLocation(uniform),value);
        }

        void set(const std::string &uniform, glm::vec2 value) {
            //DONE: call opengl to set the value to the uniform defined by name
            // void glUniform2fv(GLint location, GLsizei count, const GLfloat * value)
            // location: Location of the uniform variable to be set.
            // count: Number of elements to set. Will be set to 1 if the uniform variable is not an array.
            // value: Pointer to an array of count values to be used to set the located uniform variable.
            glUniform2fv(getUniformLocation(uniform), 1, (GLfloat*)&value); 

        }

        void set(const std::string &uniform, glm::vec3 value) {
            //DONE: call opengl to set the value to the uniform defined by name
            // void glUniform3fv(GLint location, GLsizei count, const GLfloat * value)
            // location: Location of the uniform variable to be set.
            // count: Number of elements to set. Will be set to 1 if the uniform variable is not an array.
            // value: Pointer to an array of count values to be used to set the located uniform variable.
            glUniform3fv(getUniformLocation(uniform), 1, (GLfloat*)&value);
        }

        void set(const std::string &uniform, glm::vec4 value) {
            //DONE: call opengl to set the value to the uniform defined by name
            // void glUniform4fv(GLint location, GLsizei count, const GLfloat * value)
            // location: Location of the uniform variable to be set.
            // count: Number of elements to set. Will be set to 1 if the uniform variable is not an array.
            // value: Pointer to an array of count values to be used to set the located uniform variable.
            glUniform4fv(getUniformLocation(uniform), 1, (GLfloat*)&value);
        }


        //DONE: Delete the copy constructor and assignment operator
        ShaderProgram(const ShaderProgram&) = delete;
        ShaderProgram& operator=(const ShaderProgram&) = delete;
        //Question: Why do we do this? Hint: Look at the deconstructor
        // Answer: Because the deconstructor destroys the program. So in case of enabling the copy constructor, if the object copy is terminated,
        // the program will be deleted in the copy while the original is still using it. It results in an error since the original object can't use 
        // a deleted program. In case of enabling the assignment operator, the object that is being assigned to will have the same program as
        // the object assigned to it. So, if either one is terminated, an error will occur in the other that is still using it.
        // So, to avoid all these errors it's best to disable/delete them.
    };

}

#endif
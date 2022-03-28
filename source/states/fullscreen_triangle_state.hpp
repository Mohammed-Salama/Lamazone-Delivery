#pragma once

#include <application.hpp>
#include <shader/shader.hpp>

// This function allows us to read glm vectors from json 
namespace glm {
    template<length_t L, typename T, qualifier Q>
    void from_json(const nlohmann::json& j, vec<L, T, Q>& v){
        for(length_t index = 0; index < L; ++index)
            v[index] = j[index].get<T>();
    }
}

class FullscreenTriangleState: public our::State {
    
    our::ShaderProgram program;
    //DONE: Add a variable in which we will store the name (ID) for a vertex array
    GLuint ID;
    // onInitialize() function is called once before the state starts
    void onInitialize() override {
        
        // Here we get the json of the scene configuration
        const auto& config = getApp()->getConfig()["scene"];

        // Then we get the path for vertex and fragment shader
        // We used "assets/shaders/fullscreen.vert" and "assets/shaders/circle.frag" as the defaults if the paths are not defined
        std::string vertex_shader_path = config.value("vertex-shader", "assets/shaders/fullscreen.vert");
        std::string fragment_shader_path = config.value("fragment-shader", "assets/shaders/circle.frag");

        // Here we create a shader, attach the vertex and fragment shader to it then link
        program.create();
        program.attach(vertex_shader_path, GL_VERTEX_SHADER);
        program.attach(fragment_shader_path, GL_FRAGMENT_SHADER);
        program.link();

        // We call use() since we will send uniforms to this program
        program.use();
        // We loop over every uniform in the configuration and send to the program
        if(const auto& uniforms = config["uniforms"]; uniforms.is_object()){
            for(auto& [name, uniform] : uniforms.items()){
                std::string type = uniform.value("type", "");
                if(type == "float"){
                    float value = uniform.value("value", 0.0f);
                    program.set(name, value);
                } else if(type == "vec2"){
                    glm::vec2 value = uniform.value("value", glm::vec2(0,0));
                    program.set(name, value);
                } else if(type == "vec4"){
                    glm::vec4 value = uniform.value("value", glm::vec4(0,0,0,0));
                    program.set(name, value);
                }
            }
        }

        //DONE: Create a vertex Array
        // void glGenVertexArrays(GLsizei n, GLuint *arrays)
        // n: Number of vertex array object names.
        // arrays: ID/Name of the array where the vertex array object names are stored.
        glGenVertexArrays(1, &ID); // n=1 since we need to create only one Vertex Array.
        // We set the clear color to be black
        glClearColor(0.0, 0.0, 0.0, 1.0);
    }

    // onDraw(deltaTime) function is called every frame 
    void onDraw(double deltaTime) override {
        //At the start of frame we want to clear the screen. Otherwise we would still see the results from the previous frame.
        glClear(GL_COLOR_BUFFER_BIT);

        //DONE: Draw a triangle using the vertex array and the program
        // void glBindVertexArray(GLuint array)
        // array: ID/Name of the vertex array to bind.
        glBindVertexArray(ID);
        // void glDrawArrays(GLenum	mode, GLint first, GLsizei count)
        // mode: Primitives type. e.x: GL_POINTS, GL_LINES, GL_TRIANGLES.
        // first: Number of vertices to be rendered.
        glDrawArrays(GL_TRIANGLES, 0, 3); // first=0 since we want to start with first element in the vertex array.
        // count=3 as the number of vertices in the fullscreen.vert are 3.
    }

    // onInitialize() function is called once after the state ends
    void onDestroy() override {
        //DONE: Delete the vertex Array
        // void glDeleteVertexArrays(GLsizei n, const GLuint *arrays)
        // n: Number of vertex array objects to be deleted.
        // arrays: Address of the array containing the n objects to be deleted.
        glDeleteVertexArrays(1, &ID); // n=1 since we created only one Vertex Array.
    }
};
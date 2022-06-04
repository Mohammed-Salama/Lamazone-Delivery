#pragma once

#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

namespace our {

    #define ATTRIB_LOC_POSITION 0
    #define ATTRIB_LOC_COLOR    1
    #define ATTRIB_LOC_TEXCOORD 2
    #define ATTRIB_LOC_NORMAL   3

    struct MiniVertex {
        float x, y, z;
        uint8_t r, g, b, a;
};

    class Bar {
        float maxHeight;
        float maxWidth;
        float width;
        float x;
        float y;
        // Here, we store the object names of the 3 main components of a mesh:
        // A vertex array object, A vertex buffer and an element buffer
        unsigned int VBO, EBO;
        unsigned int VAO;
        // We need to remember the number of elements that will be draw by glDrawElements 
        GLsizei elementCount;
    public:
      
        Bar(float x, float y, float maxHeight, float maxWidth)
        {
            this->maxHeight = maxHeight;
            this->maxWidth = maxWidth;
            this->width = maxWidth;
            this->x = x;
            this->y = y;
            this->elementCount = 6;
        }

        // this function should render the Bar
        void draw() 
        {
            std::cout<<"Should draw the energy bar"<<std::endl;
            std::cout<<"x="<<x<<" y="<<y<<std::endl;
            std::cout<<"width="<<width<<" maxWidth="<<maxWidth<<std::endl;
            std::cout<<"maxHeight="<<maxHeight<<std::endl;


            glGenVertexArrays(1, &VAO);

            //Binding the vertex array
            //glBindVertexArray(vertex array name);
            glBindVertexArray(VAO);

            //creating one vertex buffer 
            //glGenBuffers(number of vertex buffer, vertex buffer name);
            glGenBuffers(1, &VBO);

            //Binding the vertex buffer
            //glBindVertexArray(Binding Target , vertex buffer name);
            glBindBuffer(GL_ARRAY_BUFFER , VBO);

            MiniVertex vertices[] = {
                { x,                 y, 0.0f,   0,   0,   0, 255},
                { x,       y+maxHeight, 0.0f,   0,   0,   0, 255},
                { x+width, y+maxHeight, 0.0f,   0,   0,   0, 255},
                { x+width,           y, 0.0f,   0,   0,   0, 255},
            };

            if(width*100/maxWidth > 70)
                {
                    vertices[0].g = 255;
                    vertices[1].g = 255;
                    vertices[2].g = 255;
                    vertices[3].g = 255;
                }
            else if(width*100/maxWidth > 25)
                {
                    vertices[0].r = 255;
                    vertices[1].r = 255;
                    vertices[2].r = 255;
                    vertices[3].r = 255;
                    vertices[0].g = 255;
                    vertices[1].g = 255;
                    vertices[2].g = 255;
                    vertices[3].g = 255;
                }
            else
                {
                    vertices[0].r = 255;
                    vertices[1].r = 255;
                    vertices[2].r = 255;
                    vertices[3].r = 255;
                }

            GLsizei verticesCount = 4;
            glBufferData(GL_ARRAY_BUFFER, verticesCount*sizeof(MiniVertex), vertices, GL_STATIC_DRAW);
            // glBufferData(GL_ARRAY_BUFFER, verticesCount*sizeof(MiniVertex), &vertices[0], GL_STATIC_DRAW);

            uint16_t elements[] = {
                0, 1, 2,
                2, 3, 0
            };

            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(uint16_t), elements, GL_STATIC_DRAW);
            
            // glDepthFunc(GL_LESS);

            glDisable(GL_DEPTH_TEST);
            // glDepthMask(true);
            // glColorMask(true, true, true, true);

            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, (void*)0);

        }

        void updateWidth(float value, float max)
        {
            width = maxWidth * value / max;
        }

        ~Bar(){
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
        }

        Bar(Bar const &) = delete;
        Bar &operator=(Bar const &) = delete;
    };

}
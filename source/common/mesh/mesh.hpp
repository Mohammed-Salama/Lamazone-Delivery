#pragma once

#include <glad/gl.h>
#include "vertex.hpp"

namespace our {

    #define ATTRIB_LOC_POSITION 0
    #define ATTRIB_LOC_COLOR    1
    #define ATTRIB_LOC_TEXCOORD 2
    #define ATTRIB_LOC_NORMAL   3

    class Mesh {
        // Here, we store the object names of the 3 main components of a mesh:
        // A vertex array object, A vertex buffer and an element buffer
        unsigned int VBO, EBO;
        unsigned int VAO;
        // We need to remember the number of elements that will be draw by glDrawElements 
        GLsizei elementCount;
    public:
        double raduis =0;

       double calculateRadius(const std::vector<Vertex>& vertices)
      {
               for(int i=0;i<vertices.size();i++)
               {
                   for(int j=i+1;j<vertices.size();j++)
                   {
                        double x ,y,z;
                        x= vertices[i].position.x-vertices[j].position.x;
                        y= vertices[i].position.y-vertices[j].position.y;
                        z = vertices[i].position.z-vertices[j].position.z;
                        double value = sqrt((x*x)+(y*y)+(z*z));
                        value/=2;
                        raduis = raduis > value ? raduis : value;
                   }
               }
               return raduis;
       }
      


        // The constructor takes two vectors:
        // - vertices which contain the vertex data.
        // - elements which contain the indices of the vertices out of which each rectangle will be constructed.
        // The mesh class does not keep a these data on the RAM. Instead, it should create
        // a vertex buffer to store the vertex data on the VRAM,
        // an element buffer to store the element data on the VRAM,
        // a vertex array object to define how to read the vertex & element buffer during rendering 
        Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& elements)
        {

             calculateRadius(vertices);
            //DONE: (Req 1) Write this function
            // remember to store the number of elements in "elementCount" since you will need it for drawing
            // For the attribute locations, use the constants defined above: ATTRIB_LOC_POSITION, ATTRIB_LOC_COLOR, etc

            //Creating 1 vertex array 
            //glGenVertexArrays(number of vertex arrays, vertex array name);
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
            GLsizei verticesCount = GLsizei(vertices.size());
            
            // glBufferData(target, size,data, usage);
            glBufferData(GL_ARRAY_BUFFER, verticesCount*sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

            // Enabling vetex atrribute
            // glEnableVertexAttribArray(the index of the generic vertex attribute)
            glEnableVertexAttribArray(ATTRIB_LOC_POSITION);
            glVertexAttribPointer(ATTRIB_LOC_POSITION, 3, GL_FLOAT, false, sizeof(Vertex), (void*)0);

            glEnableVertexAttribArray(ATTRIB_LOC_COLOR);
            glVertexAttribPointer(ATTRIB_LOC_COLOR, 4, GL_UNSIGNED_BYTE, true, sizeof(Vertex), (void*)offsetof(Vertex, color));

            glEnableVertexAttribArray(ATTRIB_LOC_TEXCOORD);
            glVertexAttribPointer(ATTRIB_LOC_TEXCOORD, 2, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, tex_coord));

            glEnableVertexAttribArray(ATTRIB_LOC_NORMAL);
            glVertexAttribPointer(ATTRIB_LOC_NORMAL, 3, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, normal));

            glGenBuffers(1, &EBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            elementCount = GLsizei(elements.size());

            glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementCount*sizeof(unsigned int), &elements[0], GL_STATIC_DRAW);


            
        }

        // this function should render the mesh
        void draw() 
        {
            //DONE: (Req 1) Write this function
            glBindVertexArray(VAO);

            //render primitives from array data
            //glDrawElements(mode, count,type,indices)
            glDrawElements(GL_TRIANGLES, elementCount, GL_UNSIGNED_INT, (void*)0);
        }

        // this function should delete the vertex & element buffers and the vertex array object
        ~Mesh(){
            //DONE: (Req 1) Write this function
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers (1, &EBO);
        }

        Mesh(Mesh const &) = delete;
        Mesh &operator=(Mesh const &) = delete;
    };

}
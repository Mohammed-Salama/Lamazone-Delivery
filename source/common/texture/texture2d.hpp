#pragma once

#include <glad/gl.h>

namespace our {
    //GLenum TexUnits[5] = {GL_TEXTURE0,GL_TEXTURE1,GL_TEXTURE2,GL_TEXTURE3,GL_TEXTURE4};
    // This class defined an OpenGL texture which will be used as a GL_TEXTURE_2D
    class Texture2D {
        // The OpenGL object name of this texture 
        GLuint name = 0;
    public:
        // This constructor creates an OpenGL texture and saves its object name in the member variable "name" 
        Texture2D() {
            //DONE: (Req 4) Complete this function
            glGenTextures(1, &name);
        };

        // This deconstructor deletes the underlying OpenGL texture
        ~Texture2D() { 
            //DONE: (Req 4) Complete this function
            glDeleteTextures(1, &name);
        }

        // Get the internal OpenGL name of the texture which is useful for use with framebuffers
        GLuint getOpenGLName() {
            return name;
        }

        // This method binds this texture to GL_TEXTURE_2D
        void bind() const {
            //DONE: (Req 4) Complete this function
            glBindTexture(GL_TEXTURE_2D, name);
        }

        // This static method ensures that no texture is bound to GL_TEXTURE_2D
        static void unbind(){
            //DONE: (Req 4) Complete this function
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        void activeTexture(GLenum texUnit){
            glActiveTexture(texUnit);
        }
        Texture2D(const Texture2D&) = delete;
        Texture2D& operator=(const Texture2D&) = delete;
    };
    
}
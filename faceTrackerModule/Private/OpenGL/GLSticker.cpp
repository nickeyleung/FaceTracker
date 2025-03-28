//
//  GLSticker.cpp
//  ffmpeg5
//
//  Created by Leung on 2025/1/24.
//

#include "GLSticker.hpp"
#include <iostream> 
#include <fstream>
#include <sstream>
#include <list>
#include "CCGLTexture.h"
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>

GLSticker:: GLSticker() {
    
}

GLSticker:: ~GLSticker() {
    
}

void GLSticker:: setupPath(std::string path) {
    this->path = path;
    
    CCGLTexture glTexture;
    m_texID = glTexture.CreateGLTextureFromFile(path,"item_stickerrabit.png");
    
    loadShader();
}

//void GLSticker:: clearBuffer() {
//    
//    glDeleteRenderbuffers(1, &renderBuffer);
//    renderBuffer = 0;
//    
//    glDeleteFramebuffers(1, &frameBuffer);
//    frameBuffer = 0;
//}
//
//void GLSticker:: createRenderBuffer() {
//    
//    glGenRenderbuffers(1, &renderBuffer);
//    glBindRenderbuffer(GLenum(GL_RENDERBUFFER), renderBuffer);
//
//}

//void GLSticker:: createFrameBuffer() {
//    
//    glGenFramebuffers(1, &frameBuffer);
//    glBindFramebuffer(GLenum(GL_FRAMEBUFFER), frameBuffer);
//    glFramebufferRenderbuffer(GLenum(GL_FRAMEBUFFER), GLenum(GL_COLOR_ATTACHMENT0), GLenum(GL_RENDERBUFFER), renderBuffer);
//}

void GLSticker:: loadShader() {
    
    GLint status = 0;
    
    const char* fragStr = this->fragStr;
    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragStr, NULL);
    glCompileShader(fragShader);
    
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &status);
    
    if (status == GL_FALSE) {
        char           message[512];
        glGetShaderInfoLog(fragShader, 512, NULL, message);
        std::cout << "compline fragment fail: " << message << std::endl;
        return;
    }
    
    const char* vertexStr = this->vertexStr;
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexStr, NULL);
    glCompileShader(vertexShader);
    
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
    
    if (status == GL_FALSE) {
        char           message[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, message);
        std::cout << "compline vertex fail: " << message << std::endl;
        return;
    }
    
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);
    
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    
    if (status == GL_FALSE) {
        std::cout << "link program fail" << std::endl;
        return;
    }
    
    this->program = program;
    
    std::cout << "sticker program id: " << program << std::endl;
}


//void GLSticker:: loadVertex() {
//    
//    const GLfloat planeVertexs[]  = {
//              -0.5,-0.5, 0.5 ,  0.0, 0.0 ,
//              -0.5, 0.5, 0.5 ,  0.0, 1.0 ,
//        0.5, -0.5,  0.5 , 1.0, 0.0 ,
//        0.5, 0.5, 0.5 ,  1.0, 1.0 ,
//    };
//
//    const short planeIndexs[]= {
//            0, 1, 2,  1, 3, 2
//    };
//    
//    //vao
//    glGenVertexArrays(1, &m_vaoId);
//    glBindVertexArray(m_vaoId);
//    
//    //vbo
//    GLuint m_vboID = 0;
//    glGenBuffers(1, &m_vboID);
//    glBindBuffer(GL_ARRAY_BUFFER, m_vboID);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertexs), planeVertexs, GL_STATIC_DRAW);
//   
//    //ebo
//    GLuint m_eboID = 0;
//    glGenBuffers(1, &m_eboID);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eboID);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(planeIndexs), planeIndexs, GL_STATIC_DRAW);
//    
//    int offset = 0;
//
//    GLuint vertexLocation = glGetAttribLocation(program, "position");
//    glEnableVertexAttribArray(vertexLocation);
//    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void *)offset);
//    
//    offset += 3 * sizeof(float);
//    
//    GLuint coordLocation = glGetAttribLocation(program, "coord");
//    glEnableVertexAttribArray(coordLocation);
//    glVertexAttribPointer(coordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (void *)offset);
//
//    
//    glBindVertexArray(0);
//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//    
//    
//    std:: cout << "sticker vao id: " << m_vaoId << "vbo: " << m_vboID << "ebo: " << m_vboID << "coordlocation: " << coordLocation << std::endl;
//}

//void GLSticker:: loadVertex() {
//    
//    const GLfloat vertexArray[]  = {
//        -0.5, 0.5, 0.1,
//        0.5, -0.5, 0.1,
//         -0.5, -0.5, 0.1,
//         
//         -0.5, 0.5, 0.1,
//        0.5, 0.5, 0.1,
//        0.5, -0.5, 0.1
//    };
//
//    const short planeIndexs[]= {
//            0, 1, 2,  1, 3, 2
//    };
//    
//    const GLfloat coordArray[] = {
//        0.0,0.0,
//        1.0,1.0,
//        0.0,1.0,
//        
//        0.0,0.0,
//        1.0,0.0,
//        1.0,1.0
//    };
//    
//    GLuint vertex = 0;
//    glGenBuffers(1, &vertex);
//    glBindBuffer(GL_ARRAY_BUFFER, vertex);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);
//    
//    GLuint vertexLocation = glGetAttribLocation(program, "position");
//    glEnableVertexAttribArray(vertexLocation);
//    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, 0);
//    
//    GLuint coord = 0;
//    glGenBuffers(1, &coord);
//    glBindBuffer(GL_ARRAY_BUFFER, coord);
//    glBufferData(GL_ARRAY_BUFFER, sizeof(coordArray), coordArray, GL_STATIC_DRAW);
//    
//    GLuint coordLocation = glGetAttribLocation(program, "coord");
//    glEnableVertexAttribArray(coordLocation);
//    glVertexAttribPointer(coordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, 0);
//    
//    std:: cout << "sticker vertex id: " << vertex << "vertexLocation: " << vertexLocation << "coord: " << coord << "coordlocation: " << coordLocation << std::endl;
//}


void GLSticker:: display(int width, int height) {

    glUseProgram(program);
        
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_texID);
    GLuint texLocation = glGetUniformLocation(program, "texture");
    glUniform1ui(texLocation, m_texID);
    
    glm::mat4x4  objectMat = glm::scale(glm::mat4(1.0f),glm::vec3(0.4f, 0.4f * 0.6f, 1.0) );

    glUniformMatrix4fv(glGetUniformLocation(program, "mat") , 1 , GL_FALSE , glm::value_ptr(objectMat));
    
    std::cout << "sticker m_texID id: " << m_texID  << std::endl;
    glDrawArrays(GL_TRIANGLES, 0, 6);

}



void GLSticker:: clear(int width, int height) {
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.5, 1, 1, 1);
    glViewport(0, 0, width, height);
}

void GLSticker:: startRender() {
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
//    std::cout << "render" << std::endl;
}

GLuint GLSticker:: getProgramID() {
    return this->program;
}


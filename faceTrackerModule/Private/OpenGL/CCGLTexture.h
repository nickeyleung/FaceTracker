//
// Created by chenchao on 2021/8/12.
//

#ifndef CCOPENGLES_CCGLTEXTURE_H
#define CCOPENGLES_CCGLTEXTURE_H

#include "CCImage.h"
#include <OpenGLES/ES3/gl.h>
#include <stdio.h>
#include <string>

class CCGLTexture {

public:
    CCGLTexture();
    ~CCGLTexture();

    GLuint  GetTextureID();
    GLuint  CreateGLTextureFromFile(const std::string &pathString, const char* fileName);

private:
    GLuint  generateTexture(const std::string &pathString, const char* fileName);
    GLuint  createOpenGLTexture(CCImage* pImg);

private:
    GLuint m_texID;
};


#endif //CCOPENGLES_CCGLTEXTURE_H

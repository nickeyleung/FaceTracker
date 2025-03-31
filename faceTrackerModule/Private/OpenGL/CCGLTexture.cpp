//
// Created by chenchao on 2021/8/12.
//

#include "CCGLTexture.h"
#include "iostream"

CCGLTexture::CCGLTexture()
{
}

CCGLTexture::~CCGLTexture()
{
}

GLuint CCGLTexture::GetTextureID()
{
    return m_texID;
}

GLuint  CCGLTexture::CreateGLTextureFromFile(const std::string &pathString, const char* fileName)
{
    m_texID = generateTexture(pathString,fileName);
    return m_texID;
}

GLuint  CCGLTexture::generateTexture(const std::string &pathString, const char* fileName)
{

    //printf("CCGLTexture Load Image file: %s \n", pathString.c_str());
    
    std::string imagePath = pathString;

    CCImage* glImage = new CCImage();
    glImage->ReadFromFile((unsigned char*)imagePath.c_str());
    GLuint texID = createOpenGLTexture(glImage);


    delete glImage;

    return texID;
}

GLuint CCGLTexture::createOpenGLTexture(CCImage* pImg)
{
    if(pImg == NULL){
        return -1;
    }

    GLuint textureID;
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1,&textureID);//产生纹理索引
    glBindTexture(GL_TEXTURE_2D,textureID);//绑定纹理索引，之后的操作都针对当前纹理索引

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,pImg->GetWidth(),pImg->GetHeight(),0,GL_RGBA,GL_UNSIGNED_BYTE,pImg->GetData());//指定参数，生成纹理

    return textureID;
}

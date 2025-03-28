//
//  GLYUV.hpp
//  ffmpeg5
//
//  Created by Leung on 2024/12/27.
//

#ifndef GLYUV_hpp
#define GLYUV_hpp

#include <stdio.h>
#include <string>

#include <OpenGLES/ES3/gl.h>
#include <CoreVideo/CoreVideo.h>

class GLYUV {
    
public:
    GLYUV();
    ~GLYUV();
    void setupPath(std:: string path);
    void display(uint8_t *y, uint8_t *u, uint8_t *v, int width, int height, int viewPortW, int viewPortH);
    void display(CVOpenGLESTextureRef sampleY, CVOpenGLESTextureRef sampleUV, int width, int height);
    void display(CVOpenGLESTextureCacheRef cache, CVImageBufferRef imageBuffer, int width, int height);
    void display(GLuint program, CVOpenGLESTextureRef sampleY, CVOpenGLESTextureRef sampleUV, int width, int height);
    
    GLuint getProgramID();
    
    const char *vertexStr;
    const char *fragStr;
    GLuint program = -1;

private:
    
    GLuint renderBuffer;
    GLuint frameBuffer;
    GLuint textures[3];
    GLuint vertex = 0;
    GLuint coord = 0;
    std:: string path;
    
    void clearBuffer();
    void createRenderBuffer();
    void createFrameBuffer();
    void loadShader();
    void loadVertex();

    void clear(int width, int height);
    void startRender();
    const char* loadFile(std:: string fName);
};

#endif /* GLYUV_hpp */

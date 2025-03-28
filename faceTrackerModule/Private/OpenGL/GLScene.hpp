//
//  GLScene.hpp
//  ffmpeg5
//
//  Created by Leung on 2024/12/27.
//

#ifndef GLScene_hpp
#define GLScene_hpp

#include <stdio.h>
#include <string>
#include "GLYUV.hpp"
#include <CoreVideo/CoreVideo.h>
#include "GLSticker.hpp"

using namespace std;

class GLScene {
public:
    GLScene();
    ~GLScene();
    void setBundlePath(string path, const char *vertexStr, const char *fragStr, const char *stickerVertexStr, const char *stockerFragStr);
    void display(uint8_t *y, uint8_t *u, uint8_t *v, int width, int height, int viewPortW, int viewPortH);
    void display(CVOpenGLESTextureRef sampleY, CVOpenGLESTextureRef sampleUV, int width, int height);
    void display(CVOpenGLESTextureCacheRef cache, CVImageBufferRef imageBuffer, int width, int height);
    void updateSticker(float x, float y, int width, int height);
    GLuint getProgramID();
private:
    
    string pathString;
    GLYUV yuv;
    GLSticker sticker;
    
    void initYUV(string path, const char *vertexStr, const char *fragStr);
    void initSticker(string path, const char *vertexStr, const char *fragStr);
};

#endif /* GLScene_hpp */

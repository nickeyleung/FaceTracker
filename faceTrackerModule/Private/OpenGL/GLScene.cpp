//
//  GLScene.cpp
//  ffmpeg5
//
//  Created by Leung on 2024/12/27.
//

#include "GLScene.hpp"
#include "GLYUV.hpp"

GLScene:: GLScene() {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
}

GLScene:: ~GLScene() {
    
}

void GLScene:: setBundlePath(string path, const char *vertexStr, const char *fragStr, const char *stickerVertexStr, const char *stockerFragStr) {
    this->pathString = path;
    this->initYUV(path, vertexStr, fragStr);
    this->initSticker(path, stickerVertexStr, stockerFragStr);
}

void GLScene:: initYUV(string path, const char *vertexStr, const char *fragStr) {
    yuv = GLYUV();
    yuv.vertexStr = vertexStr;
    yuv.fragStr = fragStr;
    yuv.setupPath(path);
}

void GLScene:: initSticker(string path, const char *vertexStr, const char *fragStr) {
    sticker = GLSticker();
    sticker.vertexStr = vertexStr;
    sticker.fragStr = fragStr;
    sticker.setupPath(path);
}

void GLScene:: display(uint8_t *y, uint8_t *u, uint8_t *v, int width, int height, int viewPortW, int viewPortH) {
    yuv.display(y, u, v, width, height, viewPortW, viewPortH);
}

void GLScene:: display(CVOpenGLESTextureRef sampleY, CVOpenGLESTextureRef sampleUV, int width, int height) {
    yuv.display(sampleY, sampleUV, width, height);
}

void GLScene:: display(CVOpenGLESTextureCacheRef cache, CVImageBufferRef imageBuffer, int width, int height) {
    yuv.display(cache, imageBuffer, width, height);
}

void GLScene:: updateSticker(float x, float y, float width, float height, float rotateX, float rotateY, float rotateZ) {
    sticker.display(x, y, width, height, rotateX, rotateY, rotateZ);
}

GLuint GLScene:: getProgramID() {
    return  yuv.getProgramID();
}

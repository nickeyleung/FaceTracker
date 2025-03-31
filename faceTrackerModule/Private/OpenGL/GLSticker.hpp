//
//  GLSticker.hpp
//  ffmpeg5
//
//  Created by Leung on 2025/1/24.
//

#ifndef GLSticker_hpp
#define GLSticker_hpp

#include <stdio.h>
#include <string>

#include <OpenGLES/ES2/gl.h>
#include <CoreVideo/CoreVideo.h>

class GLSticker {
public:
    GLSticker();
    ~GLSticker();
    
    void setupPath(std:: string path);
    void display(float centerX, float centerY, float width, float height, float rotateX, float rotateY, float rotateZ);
    
    GLuint getProgramID();
    
    const char *vertexStr;
    const char *fragStr;
    GLuint program = -1;
private:
    
    GLuint renderBuffer;
    GLuint frameBuffer;
    std:: string path;
    GLuint m_texID;
    GLuint m_vaoId = 0;

    
    void clearBuffer();
    void createRenderBuffer();
    void createFrameBuffer();
    void loadShader();
    void loadVertex();

    void clear(int width, int height);
    void startRender();
};

#endif /* GLSticker_hpp */



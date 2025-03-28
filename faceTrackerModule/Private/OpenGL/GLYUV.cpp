//
//  GLYUV.cpp
//  ffmpeg5
//
//  Created by Leung on 2024/12/27.
//

#include "GLYUV.hpp"
#include <iostream> 
#include <fstream>
#include <sstream>
#include <list>

GLYUV:: GLYUV() {
    
    glGenTextures(3, textures);

}

GLYUV:: ~GLYUV() {
    
}

void GLYUV:: setupPath(std::string path) {
    this->path = path;
    
    clearBuffer();
    createRenderBuffer();
    createFrameBuffer();
    loadShader();
    loadVertex();
}

void GLYUV:: clearBuffer() {
    
    glDeleteRenderbuffers(1, &renderBuffer);
    renderBuffer = 0;
    
    glDeleteFramebuffers(1, &frameBuffer);
    frameBuffer = 0;
}

void GLYUV:: createRenderBuffer() {
    
    glGenRenderbuffers(1, &renderBuffer);
    glBindRenderbuffer(GLenum(GL_RENDERBUFFER), renderBuffer);

}

void GLYUV:: createFrameBuffer() {
    
    glGenFramebuffers(1, &frameBuffer);
    glBindFramebuffer(GLenum(GL_FRAMEBUFFER), frameBuffer);
    glFramebufferRenderbuffer(GLenum(GL_FRAMEBUFFER), GLenum(GL_COLOR_ATTACHMENT0), GLenum(GL_RENDERBUFFER), renderBuffer);
}

void GLYUV:: loadShader() {
    
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
    
    glUseProgram(program);
    this->program = program;

    std::cout << "YUV program id: " << program << std::endl;
}

const char* GLYUV::loadFile(std::string fName) {
    
    std::string shaderPath = this->path +"/"+ fName;
    std::string     shaderContent = "";
    
    printf("CompileShader !!!%s\n",shaderPath.c_str());
    std::ifstream        shaderFile;
    std::stringstream    fileSStream;
    
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    try
    {
        shaderFile.open(shaderPath);
        if (!shaderFile.is_open())
        {
            throw("Open file error");
        }

        fileSStream << shaderFile.rdbuf();
        
        shaderFile.close();

        shaderContent = fileSStream.str();
        
    }
    catch (const char* err)
    {
        std::cout<<err;
    }
    
    const char* shaderSource = shaderContent.c_str();
    return shaderSource;
}

void GLYUV:: loadVertex() {
    
    const GLfloat vertexArray[] = {
        -1.0, 1.0, 0.0,
         1.0, -1.0, 0.0,
         -1.0, -1.0, 0.0,
         
         -1.0, 1.0, 0.0,
         1.0, 1.0, 0.0,
         1.0, -1.0, 0.0
    };
    
    const GLfloat coordArray[] = {
        0.0,0.0,
        1.0,1.0,
        0.0,1.0,
        
        0.0,0.0,
        1.0,0.0,
        1.0,1.0
    };
    
    GLuint vertex = 0;
    glGenBuffers(1, &vertex);
    glBindBuffer(GL_ARRAY_BUFFER, vertex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);
    
    GLuint vertexLocation = glGetAttribLocation(program, "position");
    glEnableVertexAttribArray(vertexLocation);
    glVertexAttribPointer(vertexLocation, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, 0);
    
    GLuint coord = 0;
    glGenBuffers(1, &coord);
    glBindBuffer(GL_ARRAY_BUFFER, coord);
    glBufferData(GL_ARRAY_BUFFER, sizeof(coordArray), coordArray, GL_STATIC_DRAW);
    
    GLuint coordLocation = glGetAttribLocation(program, "coord");
    glEnableVertexAttribArray(coordLocation);
    glVertexAttribPointer(coordLocation, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, 0);
    
    std:: cout << "YUV vertex id: " << vertex << "vertexLocation: " << vertexLocation << "coord: " << coord << "coordlocation: " << coordLocation << std::endl;
}



void GLYUV:: display(CVOpenGLESTextureCacheRef cache, CVImageBufferRef imageBuffer, int width, int height) {
    clear(width, height);

    if (program == -1) {
        return;
    }

    size_t bufferWidth = CVPixelBufferGetWidth(imageBuffer);
    size_t bufferHeight = CVPixelBufferGetHeight(imageBuffer);

    CVReturn ret = kCVReturnError;
    CVOpenGLESTextureRef sampleY;
    ret = CVOpenGLESTextureCacheCreateTextureFromImage(kCFAllocatorDefault, cache, imageBuffer, nil, GL_TEXTURE_2D, GL_LUMINANCE, (GLsizei)bufferWidth, (GLsizei)bufferHeight, GL_LUMINANCE, GL_UNSIGNED_BYTE, 0, &sampleY);

    if (ret != kCVReturnSuccess) {
        return;
    }

    CVOpenGLESTextureRef sampleUV;
    ret = CVOpenGLESTextureCacheCreateTextureFromImage(kCFAllocatorDefault, cache, imageBuffer, nil, GL_TEXTURE_2D, GL_LUMINANCE_ALPHA, (GLsizei)bufferWidth / 2, (GLsizei)bufferHeight / 2, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, 1, &sampleUV);

    if (ret != kCVReturnSuccess) {
        return;
    }

    glActiveTexture(GL_TEXTURE0);
    GLuint yName = CVOpenGLESTextureGetName(sampleY);
    glBindTexture(CVOpenGLESTextureGetTarget(sampleY), yName);
    glTexParameteri(CVOpenGLESTextureGetTarget(sampleY), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(CVOpenGLESTextureGetTarget(sampleY), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(CVOpenGLESTextureGetTarget(sampleY), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(CVOpenGLESTextureGetTarget(sampleY), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    GLuint yLocation = glGetUniformLocation(program, "sampleY");
    glUniform1i(yLocation, 0);

    glActiveTexture(GL_TEXTURE1);
    GLuint uvName = CVOpenGLESTextureGetName(sampleUV);
    glBindTexture(CVOpenGLESTextureGetTarget(sampleUV), uvName);
    glTexParameteri(CVOpenGLESTextureGetTarget(sampleUV), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(CVOpenGLESTextureGetTarget(sampleUV), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(CVOpenGLESTextureGetTarget(sampleUV), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(CVOpenGLESTextureGetTarget(sampleUV), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    GLuint uvLocation = glGetUniformLocation(program, "sampleUV");
    glUniform1i(uvLocation, 1);

    const GLfloat color601[] = {            1.164,  1.164, 1.164,
        0.0, -0.392, 2.017,
        1.596, -0.813,   0.0,};

    GLuint colorMtrix = glGetUniformLocation(program, "colorMtrix");
    glUniformMatrix3fv(colorMtrix, 1, GL_FALSE, color601);
    startRender();
    glDeleteTextures(1, &yName);
    glDeleteTextures(1, &uvName);
    CFRelease(sampleY);
    CFRelease(sampleUV);
    std:: cout << "render" << std::endl;
}

void GLYUV:: clear(int width, int height) {
    
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0, 1, 1, 1);
    glViewport(0, 0, width, height);
}

void GLYUV:: startRender() {
    
    glDrawArrays(GL_TRIANGLES, 0, 6);
//    std::cout << "render" << std::endl;
}

GLuint GLYUV:: getProgramID() {
    return this->program;
}


void GLYUV:: display(uint8_t *y, uint8_t *u, uint8_t *v, int width, int height, int viewPortW, int viewPortH) {
    clear(viewPortW, viewPortH);
    
    if (program == -1) {
        return;
    }
    
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, GLsizei(width), GLsizei(height), 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, y);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    GLuint yLocation = glGetUniformLocation(program, "sampleY");
    glUniform1i(yLocation, 0);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, GLsizei(width / 2), GLsizei(height / 2), 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, u);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    GLuint uLocation = glGetUniformLocation(program, "sampleU");
    glUniform1i(uLocation, 1);
    
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, GLsizei(width / 2), GLsizei(height / 2), 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, v);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    GLuint vLocation = glGetUniformLocation(program, "sampleV");
    glUniform1i(vLocation, 2);
    
    const GLfloat color601[] = {            1.164,  1.164, 1.164,
        0.0, -0.392, 2.017,
        1.596, -0.813,   0.0,};
    
    GLuint colorMtrix = glGetUniformLocation(program, "colorMtrix");
    glUniformMatrix3fv(colorMtrix, 1, GL_FALSE, color601);
    startRender();
    
    std::cout << "YUV textures[0] id: " << textures[0] << ", textures[1]: " << textures[1] << ", textures[2]: " << textures[2] << std::endl;

}


void GLYUV:: display(CVOpenGLESTextureRef sampleY, CVOpenGLESTextureRef sampleUV, int width, int height) {
    clear(width, height);

    if (program == -1) {
        return;
    }
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);

    glActiveTexture(GL_TEXTURE0);
    GLuint yName = CVOpenGLESTextureGetName(sampleY);
    glBindTexture(CVOpenGLESTextureGetTarget(sampleY), yName);
    glTexParameteri(CVOpenGLESTextureGetTarget(sampleY), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(CVOpenGLESTextureGetTarget(sampleY), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(CVOpenGLESTextureGetTarget(sampleY), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(CVOpenGLESTextureGetTarget(sampleY), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    GLuint yLocation = glGetUniformLocation(program, "sampleY");
    glUniform1i(yLocation, 0);

    glActiveTexture(GL_TEXTURE1);
    GLuint uvName = CVOpenGLESTextureGetName(sampleUV);
    glBindTexture(CVOpenGLESTextureGetTarget(sampleUV), uvName);
    glTexParameteri(CVOpenGLESTextureGetTarget(sampleUV), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(CVOpenGLESTextureGetTarget(sampleUV), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(CVOpenGLESTextureGetTarget(sampleUV), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(CVOpenGLESTextureGetTarget(sampleUV), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    GLuint uvLocation = glGetUniformLocation(program, "sampleUV");
    glUniform1i(uvLocation, 1);

    const GLfloat color601[] = {            1.164,  1.164, 1.164,
        0.0, -0.392, 2.017,
        1.596, -0.813,   0.0,};

    GLuint colorMtrix = glGetUniformLocation(program, "colorMtrix");
    glUniformMatrix3fv(colorMtrix, 1, GL_FALSE, color601);
    startRender();
    glDeleteTextures(1, &yName);
    glDeleteTextures(1, &uvName);
    std:: cout << "render" << std::endl;
}

void GLYUV:: display(GLuint program, CVOpenGLESTextureRef sampleY, CVOpenGLESTextureRef sampleUV, int width, int height) {
    clear(width, height);

    if (program == -1) {
        return;
    }
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    glUseProgram(program);
    
    glActiveTexture(GLenum(GL_TEXTURE0));
    GLuint YName = CVOpenGLESTextureGetName(sampleY);
    glBindTexture(CVOpenGLESTextureGetTarget(sampleY), YName);
    glTexParameteri(CVOpenGLESTextureGetTarget(sampleY), GLenum(GL_TEXTURE_MAG_FILTER), GL_LINEAR);
    glTexParameteri(CVOpenGLESTextureGetTarget(sampleY), GLenum(GL_TEXTURE_MIN_FILTER), GL_LINEAR);
    glTexParameteri(CVOpenGLESTextureGetTarget(sampleY), GLenum(GL_TEXTURE_WRAP_S), GL_CLAMP_TO_EDGE);
    glTexParameteri(CVOpenGLESTextureGetTarget(sampleY), GLenum(GL_TEXTURE_WRAP_T), GL_CLAMP_TO_EDGE);
    GLuint Ylocation = glGetUniformLocation(program, "sampleY");
    glUniform1i(Ylocation, 0);
    

    glActiveTexture(GLenum(GL_TEXTURE1));
    GLuint UVName = CVOpenGLESTextureGetName(sampleUV);
    glBindTexture(CVOpenGLESTextureGetTarget(sampleUV), UVName);
    glTexParameteri(CVOpenGLESTextureGetTarget(sampleUV), GLenum(GL_TEXTURE_MAG_FILTER), GL_LINEAR);
    glTexParameteri(CVOpenGLESTextureGetTarget(sampleUV), GLenum(GL_TEXTURE_MIN_FILTER), GL_LINEAR);
    glTexParameteri(CVOpenGLESTextureGetTarget(sampleUV), GLenum(GL_TEXTURE_WRAP_S), GL_CLAMP_TO_EDGE);
    glTexParameteri(CVOpenGLESTextureGetTarget(sampleUV), GLenum(GL_TEXTURE_WRAP_T), GL_CLAMP_TO_EDGE);
    GLuint UVlocation = glGetUniformLocation(program, "sampleUV");
    glUniform1i(UVlocation, 1);
    
        const GLfloat color601[] = {            1.164,  1.164, 1.164,
            0.0, -0.392, 2.017,
            1.596, -0.813,   0.0,};
    GLuint colorLocation = glGetUniformLocation(program, "colorMtrix");
    glUniformMatrix3fv(colorLocation, 1, GLboolean(GL_FALSE), color601);

//    loadVertex(width: 1, height: 1);
    startRender();
    glDeleteTextures(1, &YName);
    glDeleteTextures(1, &UVName);

//    glActiveTexture(GL_TEXTURE0);
//    GLuint yName = CVOpenGLESTextureGetName(sampleY);
//    glBindTexture(CVOpenGLESTextureGetTarget(sampleY), yName);
//    glTexParameteri(CVOpenGLESTextureGetTarget(sampleY), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(CVOpenGLESTextureGetTarget(sampleY), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(CVOpenGLESTextureGetTarget(sampleY), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(CVOpenGLESTextureGetTarget(sampleY), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    GLuint yLocation = glGetUniformLocation(program, "sampleY");
//    glUniform1i(yLocation, CVOpenGLESTextureGetTarget(sampleY));
//
//    glActiveTexture(GL_TEXTURE1);
//    GLuint uvName = CVOpenGLESTextureGetName(sampleUV);
//    glBindTexture(CVOpenGLESTextureGetTarget(sampleUV), uvName);
//    glTexParameteri(CVOpenGLESTextureGetTarget(sampleUV), GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//    glTexParameteri(CVOpenGLESTextureGetTarget(sampleUV), GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//    glTexParameteri(CVOpenGLESTextureGetTarget(sampleUV), GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(CVOpenGLESTextureGetTarget(sampleUV), GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//    GLuint uvLocation = glGetUniformLocation(program, "sampleUV");
//    glUniform1i(uvLocation, CVOpenGLESTextureGetTarget(sampleUV));
//
//    const GLfloat color601[] = {            1.164,  1.164, 1.164,
//        0.0, -0.392, 2.017,
//        1.596, -0.813,   0.0,};
//
//    GLuint colorMtrix = glGetUniformLocation(program, "colorMtrix");
//    glUniformMatrix3fv(colorMtrix, 1, GL_FALSE, color601);
//    startRender();
//    glDeleteTextures(1, &yName);
//    glDeleteTextures(1, &uvName);
//    std:: cout << "render" << std::endl;
}

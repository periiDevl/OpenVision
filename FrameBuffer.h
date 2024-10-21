#ifndef FRAMEBUFFER_CLASS_H
#define FRAMEBUFFER_CLASS_H

#include <iostream>
#include "glad/glad.h"

class Framebuffer {
public:
    GLuint FBO;
    GLuint textureColorBuffer;
    GLuint RBO;
    int width, height;
    unsigned int rectVAO, rectVBO;
    Framebuffer(int w, int h) : width(w), height(h) {
        glGenFramebuffers(1, &FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);

        glGenTextures(1, &textureColorBuffer);
        glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

        glGenRenderbuffers(1, &RBO);
        glBindRenderbuffer(GL_RENDERBUFFER, RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "Framebuffer not complete!" << std::endl;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    ~Framebuffer() {
        glDeleteFramebuffers(1, &FBO);
        glDeleteTextures(1, &textureColorBuffer);
        glDeleteRenderbuffers(1, &RBO);
    }

    void bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    }

    void unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void resize(int newWidth, int newHeight) {
        width = newWidth;
        height = newHeight;

        glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

        glBindRenderbuffer(GL_RENDERBUFFER, RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    }
    void setUpRectangle() {
        
        glGenVertexArrays(1, &rectVAO);
        glGenBuffers(1, &rectVBO);
        glBindVertexArray(rectVAO);
        glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    }

    void DRAW_SCENE_AS_MAIN_FRAMEBUFFER(Shader frameBufferShader) {
        unbind();
        frameBufferShader.Activate();
        glBindVertexArray(rectVAO);
        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D,textureColorBuffer);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    GLuint getTexture() const {
        return textureColorBuffer;
    }
};
#endif
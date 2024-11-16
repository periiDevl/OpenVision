#ifndef MOUSE_DETECT_CLASS_H
#define MOUSE_DETECT_CLASS_H

#include "FrameBuffer.h"
#include "Shader.h"
#include "GameObject.h"
#include "TextureRenderer.h"
#include "Window.h"

class MouseDetection {
public:
    

    int ID_OVER_OBJECT(Window& window, Framebuffer& detectionFramebuffer, Shader& colorIdShader, Camera2D& camera, std::vector<std::unique_ptr<GameObject>>& detectableObjects) {
        // No copy happens, objects are passed by reference.

        // Bind framebuffer and clear it
        detectionFramebuffer.bind();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activate shader for color picking
        colorIdShader.activate();

        // Render each detectable object with a unique color
        for (int i = 0; i < detectableObjects.size(); i++) {
            float redValue = float((i / (256 * 256)) % 256) / 255.0f;
            float greenValue = float((i / 256) % 256) / 255.0f;
            float blueValue = float(i % 256) / 255.0f;

            // Set color for object picking
            glUniform3f(glGetUniformLocation(colorIdShader.ID, "color"), redValue, greenValue, blueValue);

            // Get a reference to the TextureRenderer instead of copying it
            detectableObjects[i]->getComponent<TextureRenderer>().setShader(colorIdShader);
            detectableObjects[i]->getComponent<TextureRenderer>().draw(camera, glm::vec3(detectableObjects[i]->transform->position,0), glm::vec3(detectableObjects[i]->transform->scale + glm::vec2(0.1f), 0), colorIdShader);
            //models[i].Draw(colorIdShader, cam3d);

        }

        // Get mouse position in the window
        double xpos, ypos;
        glfwGetCursorPos(window.getWindow(), &xpos, &ypos);

        // Get window size and calculate pixel position
        int windowWidth, windowHeight;
        glfwGetWindowSize(window.getWindow(), &windowWidth, &windowHeight);

        // Read the pixel color under the mouse cursor
        glReadPixels((int)xpos, windowHeight - (int)ypos, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, pixel);

        // Unbind framebuffer
        detectionFramebuffer.unbind();

        // Convert pixel color to an object index
        if (pixel[0] * 256 * 256 + pixel[1] * 256 + pixel[2] == 16744447)
        {
            return -1;
        }
        return pixel[0] * 256 * 256 + pixel[1] * 256 + pixel[2];
    }


private:
    unsigned char pixel[3];  // Holds the color read from the framebuffer
};

#endif

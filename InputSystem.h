#ifndef INPUT_SYSTEM_CLASS_H
#define INPUT_SYSTEM_CLASS_H

#include <unordered_map>
#include <GLFW/glfw3.h>

class InputSystem
{
public:
    InputSystem();
    void Update(GLFWwindow* window);
    bool GetKeyDown(int key);
    bool GetKeyUp(int key);
    bool GetKey(int key);
    GLFWwindow* GetWindow()
    {
        return window;
    }
private:
    GLFWwindow* window;
    void InitializeKey(int key);
    std::unordered_map<int, bool> keyStates;
    std::unordered_map<int, bool> keyDownEvents;
    std::unordered_map<int, bool> keyUpEvents;
    
};
#endif
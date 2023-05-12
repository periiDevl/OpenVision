#include "InputSystem.h"

InputSystem::InputSystem()
{
    InitializeKey(GLFW_KEY_SPACE);
    InitializeKey(GLFW_KEY_APOSTROPHE);
    InitializeKey(GLFW_KEY_COMMA);
    InitializeKey(GLFW_KEY_MINUS);
    InitializeKey(GLFW_KEY_PERIOD);
    InitializeKey(GLFW_KEY_SLASH);
    InitializeKey(GLFW_KEY_0);
    InitializeKey(GLFW_KEY_1);
    InitializeKey(GLFW_KEY_2);
    InitializeKey(GLFW_KEY_3);
    InitializeKey(GLFW_KEY_4);
    InitializeKey(GLFW_KEY_5);
    InitializeKey(GLFW_KEY_6);
    InitializeKey(GLFW_KEY_7);
    InitializeKey(GLFW_KEY_8);
    InitializeKey(GLFW_KEY_9);
    InitializeKey(GLFW_KEY_SEMICOLON);
    InitializeKey(GLFW_KEY_EQUAL);
    InitializeKey(GLFW_KEY_A);
    InitializeKey(GLFW_KEY_B);
    InitializeKey(GLFW_KEY_C);
    InitializeKey(GLFW_KEY_D);
    InitializeKey(GLFW_KEY_E);
    InitializeKey(GLFW_KEY_G);
    InitializeKey(GLFW_KEY_H);
    InitializeKey(GLFW_KEY_I);
    InitializeKey(GLFW_KEY_J);
    InitializeKey(GLFW_KEY_K);
    InitializeKey(GLFW_KEY_L);
    InitializeKey(GLFW_KEY_M);
    InitializeKey(GLFW_KEY_N);
    InitializeKey(GLFW_KEY_O);
    InitializeKey(GLFW_KEY_P);
    InitializeKey(GLFW_KEY_Q);
    InitializeKey(GLFW_KEY_R);
    InitializeKey(GLFW_KEY_S);
    InitializeKey(GLFW_KEY_T);
    InitializeKey(GLFW_KEY_U);
    InitializeKey(GLFW_KEY_V);
    InitializeKey(GLFW_KEY_W);
    InitializeKey(GLFW_KEY_X);
    InitializeKey(GLFW_KEY_Y);
    InitializeKey(GLFW_KEY_Z);
    InitializeKey(GLFW_KEY_LEFT_BRACKET);
    InitializeKey(GLFW_KEY_BACKSLASH);
    InitializeKey(GLFW_KEY_RIGHT_BRACKET);
    InitializeKey(GLFW_KEY_GRAVE_ACCENT);
    InitializeKey(GLFW_KEY_WORLD_1);
    InitializeKey(GLFW_KEY_WORLD_2);
    InitializeKey(GLFW_KEY_ESCAPE);
    InitializeKey(GLFW_KEY_ENTER);
    InitializeKey(GLFW_KEY_TAB);
    InitializeKey(GLFW_KEY_BACKSPACE);
    InitializeKey(GLFW_KEY_INSERT);
    InitializeKey(GLFW_KEY_DELETE);
    InitializeKey(GLFW_KEY_RIGHT);
    InitializeKey(GLFW_KEY_LEFT);
    InitializeKey(GLFW_KEY_DOWN);
    InitializeKey(GLFW_KEY_UP);
    InitializeKey(GLFW_KEY_PAGE_UP);
    InitializeKey(GLFW_KEY_PAGE_DOWN);
    InitializeKey(GLFW_KEY_HOME);
    InitializeKey(GLFW_KEY_END);
    InitializeKey(GLFW_KEY_CAPS_LOCK);
    InitializeKey(GLFW_KEY_SCROLL_LOCK);
    InitializeKey(GLFW_KEY_NUM_LOCK);
    InitializeKey(GLFW_KEY_PRINT_SCREEN);
    InitializeKey(GLFW_KEY_PAUSE);
    InitializeKey(GLFW_KEY_F1);
    InitializeKey(GLFW_KEY_F2);
    InitializeKey(GLFW_KEY_F3);
    InitializeKey(GLFW_KEY_F4);
    InitializeKey(GLFW_KEY_F5);
    InitializeKey(GLFW_KEY_F6);
    InitializeKey(GLFW_KEY_F7);
    InitializeKey(GLFW_KEY_F8);
    InitializeKey(GLFW_KEY_F9);
    InitializeKey(GLFW_KEY_F10);
    InitializeKey(GLFW_KEY_F11);
    InitializeKey(GLFW_KEY_F12);
    InitializeKey(GLFW_KEY_F13);
    InitializeKey(GLFW_KEY_F14);
    InitializeKey(GLFW_KEY_F15);
    InitializeKey(GLFW_KEY_F16);
    InitializeKey(GLFW_KEY_F17);
    InitializeKey(GLFW_KEY_F18);
    InitializeKey(GLFW_KEY_F19);
    InitializeKey(GLFW_KEY_F20);
    InitializeKey(GLFW_KEY_F21);
    InitializeKey(GLFW_KEY_F22);
    InitializeKey(GLFW_KEY_F23);
    InitializeKey(GLFW_KEY_F24);
    InitializeKey(GLFW_KEY_F25);
    InitializeKey(GLFW_KEY_KP_0);
    InitializeKey(GLFW_KEY_KP_1);
    InitializeKey(GLFW_KEY_KP_2);
    InitializeKey(GLFW_KEY_KP_3);
    InitializeKey(GLFW_KEY_KP_4);
    InitializeKey(GLFW_KEY_KP_5);
    InitializeKey(GLFW_KEY_KP_6);
    InitializeKey(GLFW_KEY_KP_7);
    InitializeKey(GLFW_KEY_KP_8);
    InitializeKey(GLFW_KEY_KP_9);
    InitializeKey(GLFW_KEY_KP_DECIMAL);
    InitializeKey(GLFW_KEY_KP_DIVIDE);
    InitializeKey(GLFW_KEY_KP_MULTIPLY);
    InitializeKey(GLFW_KEY_KP_SUBTRACT);
    InitializeKey(GLFW_KEY_KP_ADD);
    InitializeKey(GLFW_KEY_KP_ENTER);
    InitializeKey(GLFW_KEY_KP_EQUAL);
    InitializeKey(GLFW_KEY_LEFT_SHIFT);
    InitializeKey(GLFW_KEY_LEFT_CONTROL);
    InitializeKey(GLFW_KEY_LEFT_ALT);
    InitializeKey(GLFW_KEY_LEFT_SUPER);
    InitializeKey(GLFW_KEY_RIGHT_SHIFT);
    InitializeKey(GLFW_KEY_RIGHT_CONTROL);
    InitializeKey(GLFW_KEY_RIGHT_ALT);
    InitializeKey(GLFW_KEY_RIGHT_SUPER);
    InitializeKey(GLFW_KEY_MENU);
}

void InputSystem::Update(GLFWwindow* window) {
    for (auto& [key, state] : keyStates) {
        bool keyDownEvent = glfwGetKey(window, key) == GLFW_PRESS && !state;
        bool keyUpEvent = glfwGetKey(window, key) == GLFW_RELEASE && state;

        if (keyDownEvent) {
            keyDownEvents[key] = true;
        }
        else {
            keyDownEvents[key] = false;
        }

        if (keyUpEvent) {
            keyUpEvents[key] = true;
        }
        else {
            keyUpEvents[key] = false;
        }

        state = glfwGetKey(window, key) == GLFW_PRESS;
    }
}

bool InputSystem::GetKeyDown(int key) {
    return keyDownEvents[key];
}

bool InputSystem::GetKeyUp(int key) {
    return keyUpEvents[key];
}

bool InputSystem::GetKey(int key) {
    return keyStates[key];
}

void InputSystem::InitializeKey(int key)
{
    keyStates[key] = false;
    keyDownEvents[key] = false;
    keyUpEvents[key] = false;
}

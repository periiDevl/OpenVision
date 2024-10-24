#include "InputSystem.h"


// define static vars
InputSystem* InputSystem::instance = nullptr;


InputSystem::InputSystem(GLFWwindow* window) :
    window(window)
{
    instance = this;

    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollWheelCallback);
}

bool InputSystem::getDown(const Inputs key) 
{
    assert(instance != nullptr, "The Input System has been terminated");
    
    return instance->curKeys[key] == InputState::DOWN && instance->prevKeys[key] != InputState::DOWN;
}

bool InputSystem::getHold(const Inputs key)
{
    assert(instance, "The Input System has been terminated");

    return instance->curKeys[key] == InputState::DOWN && instance->prevKeys[key] == InputState::DOWN;
}

bool InputSystem::getUp(const Inputs key)
{
    assert(instance, "The Input System has been terminated");
    
    return instance->curKeys[key] == InputState::UP && instance->prevKeys[key] != InputState::UP;
}

int InputSystem::getAxis(const Inputs negative, const Inputs positive)
{
    assert(instance, "The Input System has been terminated");
 
    signed char negativeValue = instance->curKeys[negative] == InputState::DOWN ? 1 : 0;
    signed char positiveValue = instance->curKeys[positive] == InputState::DOWN ? 1 : 0;

    return positiveValue - negativeValue;
}

void InputSystem::setCursorMode(const InputCursor cursor)
{
    assert(instance, "The Input System has been terminated");
    glfwSetInputMode(instance->window, GLFW_CURSOR, (long long)cursor);
}

glm::vec2 InputSystem::getMousePosition()
{
    assert(instance, "The Input System has been terminated");
 
    int width, height;
    glfwGetWindowSize(instance->window, &width, &height);

    double x = 0;
    double y = 0;

    glfwGetCursorPos(instance->window, &x, &y);

    return { x , y };
}

glm::vec2 InputSystem::getScrollWheel()
{
    return instance->scrollWheel;
}

void InputSystem::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
    assert(instance, "The Input System has been terminated");

    if (action == GLFW_REPEAT)
        return;
    
    instance->curKeys[(Inputs)key] = (InputState)action;
}

void InputSystem::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
    assert(instance, "The Input System has been terminated");

    if (action == GLFW_REPEAT)
        return;
    
    instance->curKeys[ (Inputs)(button + (int)Inputs::MOUSE) ] = (InputState)action;
}

void InputSystem::scrollWheelCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    instance->scrollWheel.x += xoffset;
    instance->scrollWheel.y += yoffset;
}

void InputSystem::update() 
{
    assert(instance, "The Input System has been terminated");

    instance->prevKeys = instance->curKeys;
    instance->scrollWheel.x = 0;
    instance->scrollWheel.y = 0;
}

InputSystem::~InputSystem()
{
    assert(instance, "The Input System has already been terminated");
    
    glfwSetKeyCallback(instance->window, nullptr);
    glfwSetMouseButtonCallback(instance->window, nullptr);
    glfwSetScrollCallback(instance->window, nullptr);


    instance->window = nullptr;

    instance->curKeys.clear();
    instance->prevKeys.clear();
}

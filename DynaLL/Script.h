#include <iostream>
#include <cmath>  // For sin and cos functions
#include <vector>


class Script {
public:
    float lerp(float a, float b, float f) {
        return a * (1.0f - f) + (b * f);
    }

    std::vector<Ov_Object> snakeObjects;
    int numObjects = 10;
    float targetX = 0.0f;  // Example target position
    float targetY = 0.0f;
    float moveSpeed = 0.1f;  // Speed of movement towards the target

    void Start() {
        std::cout << "Hello!" << std::endl;

        // Create and add `numObjects` snake segments to the list
        for (int i = 0; i < numObjects; ++i) {
            Ov_Object obj;
            obj.x = 0;
            obj.y = 0;
            obj.scale_x = 5;
            obj.scale_y = 5;
            CreateObject(obj);
            snakeObjects.push_back(obj);
            printf("Created");
        }
    }

    void Update() {
        // Get the target position (from OVObjects[1])
        targetX = OVObjects[1].x;  // Use the correct coordinates of the target object
        targetY = OVObjects[1].y;

        // Move the head (snakeObjects[0]) towards the target using lerp
        snakeObjects[0].x = lerp(snakeObjects[0].x, targetX, moveSpeed);
        snakeObjects[0].y = lerp(snakeObjects[0].y, targetY, moveSpeed);

        // Loop over all snake segments (except the head) to follow the previous one
        for (int i = 1; i < snakeObjects.size(); ++i) {
            float radius = 3.0f;

            // Calculate the angle between the current object and the previous one
            float angle = std::atan2(snakeObjects[i - 1].y - snakeObjects[i].y,
                snakeObjects[i - 1].x - snakeObjects[i].x);

            // Smoothly move the current object towards the previous one
            float targetX = snakeObjects[i - 1].x - radius * std::cos(angle);
            float targetY = snakeObjects[i - 1].y - radius * std::sin(angle);
            snakeObjects[i].x = targetX;  // Adjust speed if necessary
            snakeObjects[i].y = targetY;
        }

        // Optionally scale or perform other operations on the snake objects
        for (size_t i = 0; i < snakeObjects.size(); i++) {
            snakeObjects[i].scale_x = 5;
            snakeObjects[i].scale_y = 5;
            UseFromPool(snakeObjects[i]);
        }
    }

    void Exit() {
        // Cleanup if necessary
    }

};

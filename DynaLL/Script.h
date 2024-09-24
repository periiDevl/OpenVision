#include <iostream>
#include "pch.h"
#include "OVLIB.h"


class Script {
public:
    float lerp(float a, float b, float f)
    {
        return a * (1.0 - f) + (b * f);
    }
    Ov_Object nig1;  // Regular object
    Ov_Object nig2;  // Regular object
    bool start = false;

    void Start() {
        std::cout << "Hello!" << std::endl;

        // Initialize nig1
        nig1.x = 1;
        nig1.y = 0;
        nig1.scale_x = 5;
        nig1.scale_y = 5;

        // Initialize nig2
        nig2.x = 8;
        nig2.y = 0;
        nig2.scale_x = 5;
        nig2.scale_y = 5;


        // Create nig1 in the pool, passing it by reference
        CreateObject(nig1);  // Pass by reference
        CreateObject(nig2);  // Pass by reference
    }

    void Update() {
        // Update objects from the pool
        UseFromPool(nig1);                
        UseFromPool(nig2);
        int radius = 0.2;
        nig1.x = lerp(nig1.x - radius, OVObjects[1].x - radius, 0.1f);
        nig1.y = lerp(nig1.y - radius, OVObjects[1].y - radius, 0.1f);
        nig1.scale_x = 1;
        nig1.scale_y = 1;

        // Update nig2 in the pool
        nig2.x -= 0.1f;
        nig2.y = 0;
        nig2.scale_y = 5;
        nig2.scale_x = 10;
    }

    void Exit() {
        // Cleanup if necessary
    }
};

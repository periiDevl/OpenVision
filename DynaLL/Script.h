#include <iostream>
#include "pch.h"
#include "OVLIB.h"

// Assume OVObjects is a globally accessible vector of Ov_Object

class Script {
public:
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

        // Add nig2 to the pool
        AddToPool(nig2);

        // Create nig1 in the pool, passing it by reference
        CreateObject(nig1);  // Pass by reference
    }

    void Update() {
        // Update objects from the pool
        UseFromPool(nig1);                   // Use nig1 directly
        UseFromPool(OVObjects[nig2.index]);  // Use nig2 from the pool by index

        // Output values for debugging
        std::cout << "nig1.acc: " << nig1.acc << std::endl;
        std::cout << "OVObjects[nig1.index].acc: " << OVObjects[nig1.index].acc << std::endl;

        // Set acc to true for nig1 in the pool

        // Update nig1 properties
        nig1.x += 0.1;
        nig1.y = 0;
        nig1.scale_x = 10;
        nig1.scale_y = 10;

        // Update nig2 in the pool
        OVObjects[nig2.index].x = 8;
        OVObjects[nig2.index].y = 0;
        OVObjects[nig2.index].scale_y += 0.1;
        OVObjects[nig2.index].scale_x = 10;
    }

    void Exit() {
        // Cleanup if necessary
    }
};

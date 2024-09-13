#pragma once
#include <vector>
#include <memory>
#include "app/app.h"

//------------------------------------------------------------------------
// BackgroundObj
// Base class for all background objects (e.g., zookeepers, water bottles).
// Handles basic functionality such as adding objects, removing out-of-bounds
// objects, rendering, and updating their positions.
//------------------------------------------------------------------------
class BackgroundObj {
public:
    const float BACKGROUND_SPEED = 0.3f;  // Speed at which background objects move
    virtual ~BackgroundObj() = default;   // Virtual destructor for proper cleanup

    // Adds a new background object (e.g., zookeeper, water bottle) to the scene
    void AddBackgroundObj();

    // Removes objects that have moved out of the screen bounds
    void RemoveOutOfBoundObjs();

    // Renders all background objects in the scene
    void Render();

    // Updates the positions of all background objects based on the speed and delta time
    void UpdateBackgroundObjPos(float deltaTime, float speed);

    // Returns a reference to the vector of background sprites
    std::vector<std::unique_ptr<CSimpleSprite>>& GetBackgroundSprites();

protected:
    std::string imgName;  // The file path of the image used for the object
    std::vector<std::unique_ptr<CSimpleSprite>> backgroundSprites;  // Stores all background objects

private:
    // Clamps the value of n between min and max
    float clamp(float n, float min, float max);
};

//------------------------------------------------------------------------
// Zookeepers
// Derived class for Zookeepers, which are a type of background object.
//------------------------------------------------------------------------
class Zookeepers : public BackgroundObj {
public:
    // Constructor that initializes the zookeepers with a specified image
    Zookeepers(const std::string& imgName);

    const float ZOOKEEPER_SPAWN_PROB = 0.05f;  // Probability of spawning a zookeeper
    const int MAX_ZOOKEEPER_COUNT = 11;  // Maximum number of zookeepers allowed at once
};

//------------------------------------------------------------------------
// WaterBottles
// Derived class for Water Bottles, which are a type of background object.
//------------------------------------------------------------------------
class WaterBottles : public BackgroundObj {
public:
    // Constructor that initializes the water bottles with a specified image
    WaterBottles(const std::string& imgName);

    const float WATERBOTTLE_SPAWN_PROB = 0.008f;  // Probability of spawning a water bottle
    const int MAX_WATERBOTTLE_COUNT = 2;  // Maximum number of water bottles allowed at once
};

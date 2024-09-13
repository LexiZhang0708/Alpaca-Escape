#pragma once
#include <vector>
#include <memory>
#include "app/app.h"

// Base class for all background objects
class BackgroundObj {
public:
    const float BACKGROUND_SPEED = 0.3f; // the speed at which all background objects are moving 
    virtual ~BackgroundObj() = default;

    void AddBackgroundObj();
    void RemoveOutOfBoundObjs();
    void Render();
    void UpdateBackgroundObjPos(float deltaTime, float speed);

    // Getter for background sprites
    std::vector<std::unique_ptr<CSimpleSprite>>& GetBackgroundSprites();

protected:
    std::string imgName;
    std::vector<std::unique_ptr<CSimpleSprite>> backgroundSprites;

private:
    float clamp(float n, float min, float max);
};

// Derived class for Zookeepers
class Zookeepers : public BackgroundObj {
public:
    Zookeepers(const std::string& imgName);

    const float ZOOKEEPER_SPAWN_PROB = 0.05f;
    const int MAX_ZOOKEEPER_COUNT = 11;
};

// Derived class for WaterBottles
class WaterBottles : public BackgroundObj {
public:
    WaterBottles(const std::string& imgName);

    const float WATERBOTTLE_SPAWN_PROB = 0.008f;
    const int MAX_WATERBOTTLE_COUNT = 2;
};

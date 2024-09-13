#include "stdafx.h"
#include "Utils.h"
#include <random>
#include <cmath>

// Generate a random number between 0.0 and 1.0
float GenerateRandNum()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distribution(0.0f, 1.0f);
    return distribution(gen);
}

// Check for collision between two sprites
bool isColliding(CSimpleSprite* obj1, CSimpleSprite* obj2)
{
    float obj1Width = obj1->GetWidth() / 2.0f;
    float obj1Height = obj1->GetHeight() / 2.0f;
    float obj1X, obj1Y;
    obj1->GetPosition(obj1X, obj1Y);

    float obj2Width = obj2->GetWidth() / 2.0f;
    float obj2Height = obj2->GetHeight() / 2.0f;
    float obj2X, obj2Y;
    obj2->GetPosition(obj2X, obj2Y);

    // Detect collision based on the overlap of bounding boxes
    bool xCollision = std::abs(obj1X - obj2X) <= (obj1Width + obj2Width);
    bool yCollision = std::abs(obj1Y - obj2Y) <= (obj1Height + obj2Height);

    return (xCollision && yCollision);
}


